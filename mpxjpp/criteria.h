#ifndef GRAPHICALINDICATOR_H
#define GRAPHICALINDICATOR_H

#include <array>
#include <unordered_map>
#include <memory>
#include <vector>

#include "fieldtype.h"
#include "mpxjpp-gens.h"

namespace mpxjpp {

class ProjectProperties;

class GenericCriteriaPrompt final {
private:
	DataType m_type;
	std::string m_prompt;
public:
	GenericCriteriaPrompt(DataType type, const std::string &prompt) :
		m_type(type), m_prompt(prompt)
	{}

	MPXJPP_GETTER(type, DataType)
	MPXJPP_GETTER(prompt, const std::string &)

	bool operator ==(const GenericCriteriaPrompt &other) const {
		return m_type == other.m_type && m_prompt == other.m_prompt;
	}
};

namespace common {
namespace anyimpl {
template<>
struct compare_to_func<GenericCriteriaPrompt> {
	int operator()(const GenericCriteriaPrompt &a, const GenericCriteriaPrompt &b) {
		return static_cast<int>(a.type()) - static_cast<int>(b.type());
	}
};
}
}
}

namespace std {
template<>
struct hash<mpxjpp::GenericCriteriaPrompt> {
	size_t operator()(const mpxjpp::GenericCriteriaPrompt &val) const {
		return (static_cast<size_t>(val.type()) * 0x61) ^ std::hash<std::string>()(val.prompt());
	}
};
}

namespace mpxjpp {
struct TestOperator final {
public:
	enum {
		IS_ANY_VALUE,
		IS_WITHIN,
		IS_GREATER_THAN,
		IS_LESS_THAN,
		IS_GREATER_THAN_OR_EQUAL_TO,
		IS_LESS_THAN_OR_EQUAL_TO,
		EQUALS,
		DOES_NOT_EQUAL,
		CONTAINS,
		IS_NOT_WITHIN,
		DOES_NOT_CONTAIN,
		CONTAINS_EXACTLY,
		AND,
		OR
	};
private:
	int m_value;
public:
	constexpr TestOperator(int type = IS_ANY_VALUE) :
		m_value(type)
	{ }

	bool evaluate(const common::any &lhs, const common::any &rhs) const;

	constexpr bool operator ==(const TestOperator &other) const {
		return other.m_value == m_value;
	}

	constexpr bool operator !=(const TestOperator &other) const {
		return other.m_value != m_value;
	}
};

class GenericCriteria {
	using ObjectArray = std::array<common::any, 2>;
private:
	ProjectProperties &m_properties;
	FieldType m_leftValue;
	TestOperator m_testOperator;
	ObjectArray m_definedRightValues;
	ObjectArray m_workingRightValues;
	bool m_symbolicValues = false;
	std::vector<std::unique_ptr<GenericCriteria>> m_criteriaList;
public:
	GenericCriteria(ProjectProperties &properties) :
		m_properties(properties)
	{ }

	MPXJPP_GETTER_SETTER(leftValue, FieldType)
	MPXJPP_GETTER_SETTER(testOperator, TestOperator)

	const common::any &getValue(int index) const {
		return (m_definedRightValues[index]);
	}

	void setRightValue(int index, common::any value);

	bool evaluate(FieldContainer &container, std::unordered_map<GenericCriteriaPrompt, common::any> promptValues) const;

	MPXJPP_GETTER(criteriaList, const std::vector<std::unique_ptr<GenericCriteria>> &)

	void addCriteria(std::unique_ptr<GenericCriteria> &&criteria) {
		m_criteriaList.push_back(std::move(criteria));
	}
private:
	bool evaluateLogicalOperator(FieldContainer &container, std::unordered_map<GenericCriteriaPrompt, common::any> promptValues) const;
	ObjectArray processSymbolicValues(const ObjectArray &oldValues, FieldContainer &container, std::unordered_map<GenericCriteriaPrompt, common::any> promptValues) const;
};

class GraphicalIndicatorCriteria final : public GenericCriteria {
private:
	int m_indicator;
public:
	GraphicalIndicatorCriteria(ProjectProperties &properties) :
		GenericCriteria(properties)
	{ }

	MPXJPP_GETTER_SETTER(indicator, int)

	int evaluate(FieldContainer &container);
};

class GraphicalIndicator final {
	using CriteriaList = std::vector<std::unique_ptr<GraphicalIndicatorCriteria>>;
private:
	FieldType m_fieldType;

	CriteriaList m_nonSummaryRowCriteria;
	CriteriaList m_summaryRowCriteria;
	CriteriaList m_projectSummaryCriteria;

	unsigned char m_displayGraphicalIndicators:1;
	unsigned char m_summaryRowsInheritFromNonSummaryRows:1;
	unsigned char m_projectSummaryInheritsFromSummaryRows:1;
	unsigned char m_showDataValuesInToolTips:1;
public:
	int evaluate(FieldContainer *container);

	MPXJPP_GETTER_SETTER(fieldType, FieldType)
	MPXJPP_GETTER_SETTER(displayGraphicalIndicators, bool)
	MPXJPP_GETTER_SETTER(summaryRowsInheritFromNonSummaryRows, bool)
	MPXJPP_GETTER_SETTER(projectSummaryInheritsFromSummaryRows, bool)
	MPXJPP_GETTER_SETTER(showDataValuesInToolTips, bool)

	MPXJPP_GETTER(nonSummaryRowCriteria, const CriteriaList &)
	MPXJPP_GETTER(summaryRowCriteria, const CriteriaList &)
	MPXJPP_GETTER(projectSummaryCriteria, const CriteriaList &)

	void addNonSummaryRowCriteria(CriteriaList::value_type &&criteria) {
		m_nonSummaryRowCriteria.push_back(std::move(criteria));
	}
	void addSummaryRowCriteria(CriteriaList::value_type &&criteria) {
		m_summaryRowCriteria.push_back(std::move(criteria));
	}
	void addProjectSummaryCriteria(CriteriaList::value_type &&criteria) {
		m_projectSummaryCriteria.push_back(std::move(criteria));
	}
};


}

#endif // GRAPHICALINDICATOR_H
