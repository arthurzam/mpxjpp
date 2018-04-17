#ifndef PROJECTENTITY_H
#define PROJECTENTITY_H

#include <algorithm>
#include <map>
#include <memory>
#include <functional>

#include "listwithcallbacks.h"

namespace mpxjpp {
class ProjectFile;

class ProjectEntity {
    private:
        std::reference_wrapper<ProjectFile> m_mpx;
    protected:
        ProjectEntity(ProjectFile &mpx)
            : m_mpx(mpx)
        { }

        ProjectFile &parentFile() {
            return m_mpx;
        }
};

template <typename T>
class ProjectEntityContainer : public ListWithCallbacks<std::shared_ptr<T>> {
    static_assert (std::is_same<int, decltype (std::declval<T>().uniqueID())>::value, "uniqueID function should return int");
    static_assert (std::is_same<void, decltype (std::declval<T>().set_uniqueID(1))>::value, "set_uniqueID function should return void");
protected:
    ProjectFile &m_mpx;
    std::map<int, std::shared_ptr<T>> m_uniqueIDMap;

    virtual int firstUniqueID() {
        return 1;
    }
public:
    ProjectEntityContainer(ProjectFile &mpx) :
        m_mpx(mpx)
    {}

    void renumberUniqueIDs() {
        int uid = firstUniqueID();
        for (std::shared_ptr<T> &entity : *this) {
            entity->set_uniqueID(uid++);
        }
    }

    void validateUniqueIDsForMicrosoftProject() {
        static constexpr int MS_PROJECT_MAX_UNIQUE_ID = 0x1FFFFF;
        if (!this->empty())
            for (const std::shared_ptr<T> &entity : *this)
                if (entity->uniqueID() > MS_PROJECT_MAX_UNIQUE_ID)
                    return renumberUniqueIDs();
    }

    std::shared_ptr<T> getByUniqueID(int id) {
        auto search = m_uniqueIDMap.find(id);
        return (search == m_uniqueIDMap.cend() ? std::shared_ptr<T>() : search->second);
    }
    void unmapUniqueID(int id) {
       m_uniqueIDMap.erase(id);
    }
    void mapUniqueID(int id, std::shared_ptr<T> entity) {
        m_uniqueIDMap.emplace(id, entity);
    }
};

template <typename T>
class ProjectEntityWithIDContainer : public ProjectEntityContainer<T> {
    static_assert (std::is_same<int, decltype (std::declval<T>().id())>::value, "id function should return int");
    static_assert (std::is_same<void, decltype (std::declval<T>().set_id(1))>::value, "set_id function should return void");
protected:
    std::map<int, std::shared_ptr<T>> m_idMap;
public:
    ProjectEntityWithIDContainer(ProjectFile &mpx) :
        ProjectEntityContainer<T>(mpx)
    {}

    void renumberIDs() {
        if (!this->empty()) {
            std::sort(this->begin(), this->end(),
                      [] (const auto &a, const auto &b) { return a->id() < b->id(); });
            int id = this->front()->id();
            if (id != 0)
                id = 1;
            for (auto &entity : *this)
                entity->set_id(id++);
        }
    }

    std::shared_ptr<T> getByID(int id) const {
        auto search = m_idMap.find(id);
        return (search == m_idMap.cend() ? std::shared_ptr<T>() : search->second);
    }
    void unmapID(int id) {
       m_idMap.erase(id);
    }
    void mapID(int id, std::shared_ptr<T> entity) {
        m_idMap.emplace(id, entity);
    }

};

}

#endif // PROJECTENTITY_H
