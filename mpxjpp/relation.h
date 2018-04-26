#ifndef RELATION_H
#define RELATION_H

#include <memory>
#include "duration.h"

namespace mpxjpp {

class Task;
using TaskPtr = std::shared_ptr<Task>;

enum class RelationType {
    FINISH_FINISH = 0,
    FINISH_START,
    START_FINISH,
    START_START
};

class Relation final {
    private:
        TaskPtr m_sourceTask;
        TaskPtr m_targetTask;
        RelationType m_type;
        Duration m_lag;
    public:
        Relation(const TaskPtr &sourceTask, const TaskPtr &targetTask, RelationType type, Duration lag) :
            m_sourceTask(sourceTask), m_targetTask(targetTask),
            m_type(type), m_lag(lag)
        {}

        MPXJPP_GETTER(type, RelationType)
        MPXJPP_GETTER(lag, Duration)
        MPXJPP_GETTER(sourceTask, TaskPtr)
        MPXJPP_GETTER(targetTask, TaskPtr)
};

}

#endif // RELATION_H
