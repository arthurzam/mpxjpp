#include "viewstate.h"
#include "projectfile.h"

using namespace mpxjpp;

Filter *ViewState::filter() const {
    return m_mpx->filters().getFilterByID(m_filterID);
}
