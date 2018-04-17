#ifndef VIEWSTATE_H
#define VIEWSTATE_H

#include <string>
#include <vector>
#include <functional>

#include "mpxjpp-gens.h"

namespace mpxjpp {
class ProjectFile;
class Filter;

class ViewState final {
    private:
        std::reference_wrapper<ProjectFile> m_mpx;
        std::string m_viewName;
        std::vector<int> m_uniqueIdList;
        int m_filterID;
    public:
        ViewState(ProjectFile &mpx, std::string &&viewName, std::vector<int> &&uniqueIdList, int filterID)
            : m_mpx(mpx), m_viewName(std::move(viewName)), m_uniqueIdList(std::move(uniqueIdList)), m_filterID(filterID)
        { }
        ViewState(const ViewState &) = default;
        ViewState(ViewState &&) = default;
        ViewState &operator =(const ViewState &) = default;

        MPXJPP_GETTER(viewName, const std::string &)
        MPXJPP_GETTER(uniqueIdList, const std::vector<int> &)

        Filter *filter() const;
};

}

#endif // VIEWSTATE_H
