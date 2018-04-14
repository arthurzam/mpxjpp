#ifndef RESOURCE_H
#define RESOURCE_H

#include "fieldtype.h"
#include "projectentity.h"
#include "duration.h"
#include "enums.h"
#include "availability.h"
#include "costeatetable.h"
#include "resourcefield.h"
#include "projectcalendar.h"

#include "common/uuid.h"

namespace mpxjpp {

class ResourceAssignment;
using ResourceAssignmentPtr = std::shared_ptr<ResourceAssignment>;

class Resource final : public FieldContainer, public ProjectEntity,
        public std::enable_shared_from_this<Resource> {
public:
    struct FinderResource {
        const Resource *ptr;

        bool operator() (const std::shared_ptr<Resource> &p) {
            return p.get() == ptr;
        }
    };
private:
    std::vector<ResourceAssignmentPtr> m_assignments;
    std::string m_activeDirectoryGUID;
    Duration m_actualOvertimeWorkProtected;
    Duration m_actualWorkProtected;

    std::array<CostRateTable, 5> m_costRateTables;
    AvailabilityTable m_availability;

    BookingType m_bookingType;

    unsigned char m_eventsEnabled:1;
    unsigned char m_null:1;
    unsigned char m_generic:1;
    unsigned char m_inactive:1;
    unsigned char m_enterprise:1;
public:
    Resource(ProjectFile &file);

    common::any &getCurrentValue(const FieldType &field) override;

    void remove();

    using Date = common::DateTime;

    CostRateTable &costRateTable(unsigned index) {
        return m_costRateTables[index];
    }
    void set_costRateTable(unsigned index, CostRateTable &&value) {
        m_costRateTables[index] = std::move(value);
    }
    AvailabilityTable &availability() {
        return m_availability;
    }

    void addResourceAssignment(ResourceAssignmentPtr assignment) {
       m_assignments.push_back(std::move(assignment));
    }
    void removeResourceAssignment(const ResourceAssignment *assignment);

    MPXJPP_GETTER_SETTER(activeDirectoryGUID, const std::string &)
    MPXJPP_GETTER_SETTER(actualOvertimeWorkProtected, Duration)
    MPXJPP_GETTER_SETTER(actualWorkProtected, Duration)
    MPXJPP_GETTER_SETTER(bookingType, BookingType)

    MPXJPP_GETTER_SETTER(null, bool)
    MPXJPP_GETTER_SETTER(generic, bool)
    MPXJPP_GETTER_SETTER(inactive, bool)
    MPXJPP_GETTER_SETTER(enterprise, bool)

    Date start() const;
    Date finish() const;

#define FIELDTYPE_CLASS ResourceField
    MPXJPP_FIELD_GETTER_SETTER(uniqueID, int, UNIQUE_ID)
    int id() const {
        return _field_get<int>(ResourceField::ID);
    }
    void set_id(int val);

    MPXJPP_FIELD_GETTER(resourceCalendar, const ProjectCalendar *, CALENDAR)
    void set_resourceCalendar(ProjectCalendar *calendar) {
        _field_set<const ProjectCalendar *>(ResourceField::CALENDAR, calendar);
        if (calendar) {
            set_resourceCalendarUniqueID(calendar->uniqueID());
            calendar->set_resource(shared_from_this());
        } else
            set_resourceCalendarUniqueID(-1);
    }

    MPXJPP_FIELD_GETTER_SETTER(name, const std::string &, NAME)
    MPXJPP_FIELD_GETTER_SETTER(type, ResourceType, TYPE)
    MPXJPP_FIELD_GETTER_SETTER(initials, const std::string &, INITIALS)
    MPXJPP_FIELD_GETTER_SETTER(phonetics, const std::string &, PHONETICS)
    MPXJPP_FIELD_GETTER_SETTER(ntAccount, const std::string &, WINDOWS_USER_ACCOUNT)
    MPXJPP_FIELD_GETTER_SETTER(materialLabel, const std::string &, MATERIAL_LABEL)
    MPXJPP_FIELD_GETTER_SETTER(code, const std::string &, CODE)
    MPXJPP_FIELD_GETTER_SETTER(group, const std::string &, GROUP)
    MPXJPP_FIELD_GETTER_SETTER(workGroup, WorkGroup, WORKGROUP)
    MPXJPP_FIELD_GETTER_SETTER(emailAddress, const std::string &, EMAIL_ADDRESS)
    MPXJPP_FIELD_GETTER_SETTER(hyperlink, const std::string &, HYPERLINK)
    MPXJPP_FIELD_GETTER_SETTER(hyperlinkAddress, const std::string &, HYPERLINK_ADDRESS)
    MPXJPP_FIELD_GETTER_SETTER(hyperlinkSubAddress, const std::string &, HYPERLINK_SUBADDRESS)
    MPXJPP_FIELD_GETTER_SETTER(maxUnits, double, MAX_UNITS)
    MPXJPP_FIELD_GETTER_SETTER(peakUnits, double, PEAK)
    MPXJPP_FIELD_GETTER_SETTER(availableFrom, Date, AVAILABLE_FROM)
    MPXJPP_FIELD_GETTER_SETTER(availableTo, Date, AVAILABLE_TO)
    MPXJPP_FIELD_GETTER_SETTER(canLevel, bool, CAN_LEVEL)
    MPXJPP_FIELD_GETTER_SETTER(accrueAt, AccrueType, ACCRUE_AT)
    MPXJPP_FIELD_GETTER_SETTER(work, Duration, WORK)
    MPXJPP_FIELD_GETTER_SETTER(regularWork, Duration, REGULAR_WORK)
    MPXJPP_FIELD_GETTER_SETTER(actualWork, Duration, ACTUAL_WORK)
    MPXJPP_FIELD_GETTER_SETTER(overtimeWork, Duration, OVERTIME_WORK)
    MPXJPP_FIELD_GETTER_SETTER(remainingWork, Duration, REMAINING_WORK)
    MPXJPP_FIELD_GETTER_SETTER(actualOvertimeWork, Duration, ACTUAL_OVERTIME_WORK)
    MPXJPP_FIELD_GETTER_SETTER(remainingOvertimeWork, Duration, REMAINING_OVERTIME_WORK)
    MPXJPP_FIELD_GETTER_SETTER(percentWorkComplete, double, PERCENT_WORK_COMPLETE)
    MPXJPP_FIELD_GETTER_SETTER(standardRate, Rate, STANDARD_RATE)
    MPXJPP_FIELD_GETTER_SETTER(standardRateUnits, TimeUnit, STANDARD_RATE_UNITS)
    MPXJPP_FIELD_GETTER_SETTER(cost, double, COST)
    MPXJPP_FIELD_GETTER_SETTER(overtimeRate, Rate, OVERTIME_RATE)
    MPXJPP_FIELD_GETTER_SETTER(overtimeRateUnits, TimeUnit, OVERTIME_RATE_UNITS)
    MPXJPP_FIELD_GETTER_SETTER(overtimeCost, double, OVERTIME_COST)
    MPXJPP_FIELD_GETTER_SETTER(costPerUse, double, COST_PER_USE)

    MPXJPP_FIELD_SELECT_GETTER_SETTER(text, const std::string &, 1, 30, [] (unsigned pos) {
        return pos < 6 ? (pos < 3  ? ResourceField::TEXT1 + (pos - 1) : ResourceField::TEXT3  + (pos - 3)) :
                         (pos < 11 ? ResourceField::TEXT6 + (pos - 6) : ResourceField::TEXT11 + (pos - 11)); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(start, Date, 1, 10, [] (unsigned pos) {
        return pos < 6 ? ResourceField::START1 + (pos - 1) : ResourceField::START6 + (pos - 6); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(finish, Date, 1, 10, [] (unsigned pos) {
        return pos < 6 ? ResourceField::FINISH1 + (pos - 1) : ResourceField::FINISH6 + (pos - 6); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(number, double, 1, 20, [] (unsigned pos) {
        return pos < 6 ? ResourceField::NUMBER1 + (pos - 1) : ResourceField::NUMBER6 + (pos - 6); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(duration, Duration, 1, 10, [] (unsigned pos) {
        return pos < 4 ? ResourceField::DURATION1 + (pos - 1) : ResourceField::DURATION4 + (pos - 4); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(cost, double, 1, 10, [] (unsigned pos) {
        return pos < 4 ? ResourceField::COST1 + (pos - 1) : ResourceField::COST4 + (pos - 4); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(flag, bool, 1, 20, [] (unsigned pos) {
        return pos == 10 ? ResourceField::FLAG10 + 0 :
               pos <  10 ? ResourceField::FLAG1 + (pos - 1) : ResourceField::FLAG11 + (pos - 11); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(date, Date, 1, 10, [] (unsigned pos) {
        return ResourceField::DATE1 + (pos - 1); })

    MPXJPP_FIELD_GETTER_SETTER(actualCost, double, ACTUAL_COST)
    MPXJPP_FIELD_GETTER_SETTER(actualOvertimeCost, double, ACTUAL_OVERTIME_COST)
    MPXJPP_FIELD_GETTER_SETTER(remainingCost, double, REMAINING_COST)
    MPXJPP_FIELD_GETTER_SETTER(remainingOvertimeCost, double, REMAINING_OVERTIME_COST)

    MPXJPP_FIELD_SETTER(overAllocated, bool, OVERALLOCATED)
    bool overAllocated() {
        const common::any &value = getCachedValue(ResourceField::OVERALLOCATED);
        if(value.empty()) {
            const bool result = peakUnits() > maxUnits();
            _field_set<double>(ResourceField::OVERALLOCATED, result);
            return result;
        }
        return common::any_type_cast<bool>::get(value, {});
    }
    MPXJPP_FIELD_SETTER(workVariance, Duration, WORK_VARIANCE)
    Duration workVariance();
    MPXJPP_FIELD_SETTER(costVariance, double, COST_VARIANCE)
    double costVariance() {
        const common::any &variance = getCachedValue(ResourceField::COST_VARIANCE);
        if(variance.empty()) {
            const double result = cost() - baselineCost();
            _field_set<double>(ResourceField::COST_VARIANCE, result);
            return result;
        }
        return common::any_type_cast<double>::get(variance, {});
    }
    MPXJPP_FIELD_SETTER(sv, double, CV)
    double sv() {
        const common::any &variance = getCachedValue(ResourceField::SV);
        if (variance.empty()) {
            const double result = bcwp() - bcws();
            _field_set<double>(ResourceField::SV, result);
            return result;
        }
        return common::any_type_cast<double>::get(variance, {});
    }
    MPXJPP_FIELD_SETTER(cv, double, CV)
    double cv() {
        const common::any &variance = getCachedValue(ResourceField::CV);
        if (variance.empty()) {
            const double result = bcwp() - acwp();
            _field_set<double>(ResourceField::CV, result);
            return result;
        }
        return common::any_type_cast<double>::get(variance, {});
    }
    MPXJPP_FIELD_GETTER(baseCalendar, const std::string &, BASE_CALENDAR)
    void set_baseCalendar(const std::string &value) {
        _field_set<const std::string &>(ResourceField::BASE_CALENDAR, value.empty() ? "Standard" : value);
    }

    MPXJPP_FIELD_GETTER_SETTER(acwp, double, ACWP)
    MPXJPP_FIELD_GETTER_SETTER(notes, const std::string &, NOTES)
    MPXJPP_FIELD_GETTER_SETTER(bcws, double, BCWS)
    MPXJPP_FIELD_GETTER_SETTER(bcwp, double, BCWP)
    MPXJPP_FIELD_GETTER_SETTER(creationDate, Date, CREATED)
    MPXJPP_FIELD_GETTER_SETTER(resourceCalendarUniqueID, int, CALENDAR_UNIQUE_ID)
    MPXJPP_FIELD_GETTER_SETTER(baselineCost, double, BASELINE_COST)
    MPXJPP_FIELD_GETTER_SETTER(baselineWork, Duration, BASELINE_WORK)

    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineCost, double, 1, 10, [] (unsigned pos) {
        return ResourceField::BASELINE1_COST + 2 * (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(baselineWork, Duration, 1, 10, [] (unsigned pos) {
        return ResourceField::BASELINE1_WORK + 2 * (pos - 1); })

    MPXJPP_FIELD_GETTER_SETTER(linkedFields, bool, LINKED_FIELDS)
    MPXJPP_FIELD_GETTER_SETTER(objects, int, OBJECTS)
    MPXJPP_FIELD_GETTER_SETTER(parentID, int, PARENT_ID)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode1, const std::string &, OUTLINE_CODE1)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode2, const std::string &, OUTLINE_CODE2)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode3, const std::string &, OUTLINE_CODE3)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode4, const std::string &, OUTLINE_CODE4)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode5, const std::string &, OUTLINE_CODE5)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode6, const std::string &, OUTLINE_CODE6)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode7, const std::string &, OUTLINE_CODE7)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode8, const std::string &, OUTLINE_CODE8)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode9, const std::string &, OUTLINE_CODE9)
    MPXJPP_FIELD_GETTER_SETTER(outlineCode10, const std::string &, OUTLINE_CODE10)
    MPXJPP_FIELD_GETTER_SETTER(subprojectResourceUniqueID, int, SUBPROJECT_RESOURCE_UNIQUE_ID)
    MPXJPP_FIELD_GETTER_SETTER(budget, bool, BUDGET)
    MPXJPP_FIELD_GETTER_SETTER(guid, common::UUID, GUID)

    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseCost, double, 1, 10, [] (unsigned pos) {
        return ResourceField::ENTERPRISE_COST1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseDate, Date, 1, 30, [] (unsigned pos) {
        return ResourceField::ENTERPRISE_DATE1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseDuration, Duration, 1, 10, [] (unsigned pos) {
        return ResourceField::ENTERPRISE_DURATION1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseFlag, bool, 1, 20, [] (unsigned pos) {
        return ResourceField::ENTERPRISE_FLAG1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseNumber, double, 1, 40, [] (unsigned pos) {
        return ResourceField::ENTERPRISE_NUMBER1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseText, const std::string &, 1, 40, [] (unsigned pos) {
        return ResourceField::ENTERPRISE_TEXT1 + (pos - 1); })
    MPXJPP_FIELD_SELECT_GETTER_SETTER(enterpriseCustomField, const std::string &, 1, 50, [] (unsigned pos) {
        return ResourceField::ENTERPRISE_CUSTOM_FIELD1 + (pos - 1); })

#undef FIELDTYPE_CLASS
};
using ResourcePtr = std::shared_ptr<Resource>;

class ResourceContainer final : public ProjectEntityWithIDContainer<Resource> {
protected:
    void removed(const ResourcePtr &task) override;
public:
    ResourceContainer(ProjectFile &mpx) :
        ProjectEntityWithIDContainer<Resource>(mpx)
    {}

    ResourcePtr create();
};

}

#endif // RESOURCE_H
