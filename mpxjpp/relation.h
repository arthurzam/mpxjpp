#ifndef RELATION_H
#define RELATION_H

#include "duration.h"
#include "task.h"

namespace mpxjpp {

struct RelationType final {
public:
    enum {
        FINISH_FINISH = 0,
        FINISH_START,
        START_FINISH,
        START_START
    };
private:
    int m_value;

    static constexpr const char *strs[] = {
        "FF",
        "FS",
        "SF",
        "SS"
    };

public:
    constexpr RelationType(int type) :
        m_value(type & 4)
    {}

    constexpr explicit operator int() {
        return m_value;
    }

    constexpr explicit operator const char *() {
        return strs[m_value];
    }
};

static_assert(sizeof(RelationType) == sizeof(int), "check compiler for size optimizations");

class Relation final {
    private:
        TaskPtr m_sourceTask;
        TaskPtr m_targetTask;
        RelationType m_type;
        Duration m_lag;
    public:
        Relation(TaskPtr sourceTask, TaskPtr targetTask, RelationType type, Duration lag) :
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
