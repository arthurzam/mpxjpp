#ifndef FILTER_H
#define FILTER_H

#include "criteria.h"

namespace mpxjpp {

class Filter final {
    using CriteriaList = std::vector<std::unique_ptr<GraphicalIndicatorCriteria>>;
private:
    int m_id;
    std::string m_name;
    std::unique_ptr<GenericCriteria> m_criteria;
    CriteriaList m_prompts;
    unsigned char m_isTaskFilter:1;
    unsigned char m_isResourceFilter:1;
    unsigned char m_showRelatedSummaryRows:1;
public:
    MPXJPP_GETTER_SETTER(id, int)
    MPXJPP_GETTER_SETTER(name, const std::string &)
    MPXJPP_GETTER_SETTER(showRelatedSummaryRows, bool)
    MPXJPP_GETTER_SETTER(isTaskFilter, bool)
    MPXJPP_GETTER_SETTER(isResourceFilter, bool)

    CriteriaList &prompts() {
        return m_prompts;
    }

    const GenericCriteria *criteria() const {
        return m_criteria.get();
    }
    void set_criteria(std::unique_ptr<GenericCriteria> &&criteria) {
        m_criteria = std::move(criteria);
    }

    bool evaluate(FieldContainer &container, std::unordered_map<GenericCriteriaPrompt, common::any> promptValues) const;
};

class FilterContainer final {
    using FilterList = std::vector<std::unique_ptr<Filter>>;
private:
    FilterList m_taskFilters;
    FilterList m_resourceFilters;

    // CHECK: hold map/unordered_map for indexing by name or id
    // Map<String, Filter> m_filtersByName
    // Map<Integer, Filter> m_filtersByID
public:

    void addFilter(std::unique_ptr<Filter> &&filter);
    void removeFilter(const std::string &filterName);

    const FilterList &resourceFilters() const {
        return m_resourceFilters;
    }
    const FilterList &taskFilters() const {
        return m_taskFilters;
    }
    Filter *getFilterByName(const std::string &name) const;
    Filter *getFilterByID(int id) const;
};

}

#endif // FILTER_H
