#ifndef PROJECTENTITY_H
#define PROJECTENTITY_H

#include <algorithm>
#include <map>
#include <memory>

#include "listwithcallbacks.h"

namespace mpxjpp {
class ProjectFile;

class ProjectEntity {
    private:
        ProjectFile &m_mpx;
    protected:
        ProjectEntity(ProjectFile &mpx)
            : m_mpx(mpx)
        { }

        ProjectFile &parentFile() {
            return m_mpx;
        }
};

/**
 * Implemented by entities which can be identified by a Unique ID.
 */
class ProjectEntityWithUniqueID {
    public:
        /**
         * Retrieve the Unique ID value of the entity.
         *
         * @return Unique ID value
         */
        virtual int uniqueID() const = 0;

        /**
         * Set the Unique ID value of the entity.
         *
         * @param id Unique ID value
         */
        virtual void set_uniqueID(int id) = 0;
};

/**
 * Implemented by entities which can be identified by an ID.
 */
class ProjectEntityWithID : public ProjectEntityWithUniqueID {
public:
    /**
     * Retrieve the ID value of the entity.
     *
     * @return ID value
     */
    virtual int id() const = 0;

    /**
     * Set the ID value of the entity.
     *
     * @param id ID value
     */
    virtual void set_id(int id) = 0;
};

template <typename T>
class ProjectEntityContainer : public ListWithCallbacks<std::shared_ptr<T>> {
    static_assert(std::is_base_of<ProjectEntityWithUniqueID, T>::value, "T should derive from ProjectEntityWithUniqueID");
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
        return (search == m_uniqueIDMap.end() ? std::shared_ptr<T>() : search->second);
    }
    void unmapUniqueID(int id) {
       m_uniqueIDMap.erase(id);
    }
    void mapUniqueID(int id, std::shared_ptr<T> entity) {
        m_uniqueIDMap.insert({id, entity});
    }
};

template <typename T>
class ProjectEntityWithIDContainer : public ProjectEntityContainer<T> {
    static_assert(std::is_base_of<ProjectEntityWithID, T>::value, "T should derive from ProjectEntityWithID");
protected:
    std::map<int, std::shared_ptr<T>> m_idMap;
public:
    ProjectEntityWithIDContainer(ProjectFile &mpx) :
        ProjectEntityContainer<T>(mpx)
    {}

    void renumberIDs() {
        if (!this->empty()) {
            std::sort(this->begin(), this->end(),
                      [] (const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) { return *a < *b; });
            int id = this->front()->id();
            if (id != 0)
                id = 1;
            for (auto &entity : *this)
                entity->set_id(id++);
        }
    }

    std::shared_ptr<T> getByID(int id) const {
        auto search = m_idMap.find(id);
        return (search == m_idMap.end() ? std::shared_ptr<T>() : search->second);
    }
    void unmapID(int id) {
       m_idMap.erase(id);
    }
    void mapID(int id, std::shared_ptr<T> entity) {
        m_idMap.insert({id, entity});
    }

};

}

#endif // PROJECTENTITY_H
