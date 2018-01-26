#include "criteria.h"
#include "filter.h"
#include "task.h"

#include "common/strutils.h"

#include <typeinfo>

using namespace mpxjpp;

namespace {

static common::any &getSingleOperand(common::any &operand) {
	if (operand.isType<std::vector<common::any>>()) {
		return operand.cast<std::vector<common::any>>()[0];
	}
	return operand;
}

static int evaluateWithin(common::any &lhs, common::any &rhs) {
	if (!rhs.isType<std::vector<common::any>>())
		return false;
	std::vector<common::any> &rhsList = rhs.cast<std::vector<common::any>>();
	bool isRhsEmpty = rhsList[0].empty() || rhsList[1].empty();
	if (lhs.empty())
		return isRhsEmpty;
	if (!isRhsEmpty)
		return (lhs.compareTo(rhsList[0]) >= 0 && lhs.compareTo(rhsList[1]) <= 0) ||
			   (lhs.compareTo(rhsList[0]) <= 0 && lhs.compareTo(rhsList[1]) >= 0);
	return false;
}

static int evaluateCompareTo(common::any &lhs, common::any &rhs) {
	common::any &r = getSingleOperand(rhs);
	if (lhs.empty()) {
		if (r.empty())
			return 0;
		else
			return 1;
	} else if (r.empty())
		return -1;
	else
		return lhs.compareTo(r);
}

static bool evaluateContains(common::any &lhs, common::any &rhs) {
	common::any &r = getSingleOperand(rhs);
	if (lhs.isType<std::string>() && r.isType<std::string>())
		return common::str_toupper(lhs.cast<std::string>()).find(common::str_toupper(r.cast<std::string>())) != std::string::npos;
	return false;
}

static bool evaluateContainsExactly(common::any &lhs, common::any &rhs) {
	common::any &r = getSingleOperand(rhs);
	if (lhs.isType<std::string>() && r.isType<std::string>())
		return lhs.cast<std::string>().find(r.cast<std::string>()) != std::string::npos;
	return false;
}

}

bool TestOperator::evaluate(const common::any &_lhs, const common::any &_rhs) const {
	common::any &lhs = const_cast<common::any &>(_lhs);
	common::any &rhs = const_cast<common::any &>(_rhs);

	switch(m_value) {
		case IS_ANY_VALUE:
			return true;
		case IS_WITHIN:
			return evaluateWithin(lhs, rhs);
		case IS_GREATER_THAN:
			return evaluateCompareTo(lhs, rhs) > 0;
		case IS_LESS_THAN:
			return evaluateCompareTo(lhs, rhs) < 0;
		case IS_GREATER_THAN_OR_EQUAL_TO:
			return evaluateCompareTo(lhs, rhs) >= 0;
		case IS_LESS_THAN_OR_EQUAL_TO:
			return evaluateCompareTo(lhs, rhs) <= 0;
		case EQUALS:
			return evaluateCompareTo(lhs, rhs) == 0;
		case DOES_NOT_EQUAL:
			return evaluateCompareTo(lhs, rhs) != 0;
		case CONTAINS:
			return evaluateContains(lhs, rhs);
		case IS_NOT_WITHIN:
			return !evaluateWithin(lhs, rhs);
		case DOES_NOT_CONTAIN:
			return !evaluateContains(lhs, rhs);
		case CONTAINS_EXACTLY:
			return evaluateContainsExactly(lhs, rhs);
		case AND:
		case OR:
			throw std::exception(); // should not be run!
	}
	throw std::exception(); // should not be run!
}

void GenericCriteria::setRightValue(int index, common::any value) {
	m_definedRightValues[index] = value;
	if (value.isType<FieldType>())
		m_symbolicValues = true;
	else if (value.isType<Duration>()) {
		const Duration &v = value.cast<Duration>();
		if (v.units() != TimeUnit::HOURS)
			value = v.convertUnits(TimeUnit::HOURS, m_properties);
	}
	m_workingRightValues[index] = value;
}

bool GenericCriteria::evaluate(FieldContainer &container, std::unordered_map<GenericCriteriaPrompt, common::any> promptValues) const {
	if (m_testOperator == TestOperator::AND || m_testOperator == TestOperator::OR) {
		return evaluateLogicalOperator(container, promptValues);;
	}

	common::any lhs;
	if (m_leftValue) {
		lhs = container.getCachedValue(m_leftValue);
		switch (m_leftValue.dataType()) {
			case DataType::DATE:
				if (!lhs.empty())
					lhs = 0; // TODO: DateHelper.getDayStartDate((Date) lhs);
				break;
			case DataType::DURATION:
				lhs = (!lhs.empty() ? lhs.cast<Duration>().convertUnits(TimeUnit::HOURS, m_properties) : Duration(0, TimeUnit::HOURS));
				break;
			case DataType::STRING:
				if (lhs.empty())
					lhs = std::string();
				break;
			default:
				break;
		}
	}

	return m_testOperator.evaluate(lhs, (m_symbolicValues ? processSymbolicValues(m_workingRightValues, container, promptValues) : m_workingRightValues));
}

bool GenericCriteria::evaluateLogicalOperator(FieldContainer &container, std::unordered_map<GenericCriteriaPrompt, common::any> promptValues) const {
	if (m_criteriaList.empty())
		return true;
	for (const std::unique_ptr<GenericCriteria> &criteria : m_criteriaList) {
		bool result = criteria->evaluate(container, promptValues);
		if ((m_testOperator == TestOperator::AND && !result) || (m_testOperator == TestOperator::OR && result))
			return result;
	}
	return false;
}

GenericCriteria::ObjectArray GenericCriteria::processSymbolicValues(const ObjectArray &oldValues, FieldContainer &container, std::unordered_map<GenericCriteriaPrompt, common::any> promptValues) const {
	ObjectArray newValues;
	for (unsigned i = 0; i < oldValues.size(); ++i) {
		const common::any &value = oldValues[i];
		common::any &dst = newValues[i];
		if (value.empty())
			continue;
		if (value.isType<FieldType>()) {
			const FieldType &type = value.cast<FieldType>();
			const common::any &val = container.getCachedValue(type);
			switch (type.dataType()) {
				case DataType::DATE:
					dst = val.empty() ? val : 0; // TODO: DateHelper.getDayStartDate((Date) lhs);
					break;
				case DataType::DURATION:
					dst = (!val.empty() ? val.cast<Duration>().convertUnits(TimeUnit::HOURS, m_properties) : Duration(0, TimeUnit::HOURS));
					break;
				case DataType::STRING:
					dst = (!val.empty() ? val : std::string());
					break;
				default:
					dst = val;
					break;
			}
		} else if (value.isType<GenericCriteriaPrompt>()){
			dst = promptValues.at(value.cast<GenericCriteriaPrompt>());
		} else
			dst = value;
	}
	return newValues;
}

int GraphicalIndicatorCriteria::evaluate(FieldContainer &container) {
	return GenericCriteria::evaluate(container, {}) ? m_indicator : -1;
}

int GraphicalIndicator::evaluate(FieldContainer &container) {
	const CriteriaList *criteria = nullptr;
	static_assert(std::is_final<Task>::value, "For this part we need Task class as final");
	if (typeid(container) == typeid(Task)) {
		Task &task = static_cast<Task &>(container);
		if (task.uniqueID() == 0) {
			if (!m_projectSummaryInheritsFromSummaryRows)
				criteria = &m_projectSummaryCriteria;
			else if (!m_summaryRowsInheritFromNonSummaryRows)
				criteria = &m_summaryRowCriteria;
			else
				criteria = &m_nonSummaryRowCriteria;
		} else {
			if (!task.summary())
				criteria = &m_nonSummaryRowCriteria;
			else if (!m_summaryRowsInheritFromNonSummaryRows)
				criteria = &m_summaryRowCriteria;
			else
				criteria = &m_nonSummaryRowCriteria;
		}
	} else {
		criteria = &m_nonSummaryRowCriteria;
	}

	int result;
	for (const auto &gic : *criteria)
		if ((result = gic->evaluate(container)) != -1)
			return result;
	return 0;
}

bool Filter::evaluate(FieldContainer &container, std::unordered_map<GenericCriteriaPrompt, common::any> promptValues) const {
	if (!m_criteria)
		return true; // CHECK: maybe false - error in upstream
	if (m_criteria->evaluate(container, promptValues))
		return true;
	static_assert(std::is_final<Task>::value, "For this part we need Task class as final");
	if (m_showRelatedSummaryRows)
		if (typeid(container) == typeid(Task))
			for (const TaskPtr &t : static_cast<Task &>(container).childTasks())
				if (Filter::evaluate(*t, promptValues))
					return true;
	return true; // CHECK: maybe false - error in upstream
}

void FilterContainer::addFilter(std::unique_ptr<Filter> &&filter) {
	if (filter->isTaskFilter())
		m_taskFilters.push_back(std::move(filter));
	else if (filter->isResourceFilter())
		m_resourceFilters.push_back(std::move(filter));
	else
		throw std::invalid_argument("filter isn't task or resource filter --> bad algorithms comming");
}

void FilterContainer::removeFilter(const std::string &filterName) {
	for (auto iter = m_taskFilters.begin(), end = m_taskFilters.end(); iter != end; ++iter)
		if ((*iter)->name() == filterName) {
			m_taskFilters.erase(iter);
			return;
		}
	for (auto iter = m_resourceFilters.begin(), end = m_resourceFilters.end(); iter != end; ++iter)
		if ((*iter)->name() == filterName) {
			m_resourceFilters.erase(iter);
			return;
		}
}

Filter *FilterContainer::getFilterByName(const std::string &name) const {
	for (const auto &filter : m_resourceFilters)
		if (filter->name() == name)
			return filter.get();
	for (const auto &filter : m_taskFilters)
		if (filter->name() == name)
			return filter.get();
	return nullptr;
}

Filter *FilterContainer::getFilterByID(int id) const {
	for (const auto &filter : m_resourceFilters)
		if (filter->id() == id)
			return filter.get();
	for (const auto &filter : m_taskFilters)
		if (filter->id() == id)
			return filter.get();
	return nullptr;
}
