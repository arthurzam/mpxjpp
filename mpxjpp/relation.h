#ifndef RELATION_H
#define RELATION_H

#include "duration.h"
#include "task.h"

namespace mpxjpp {

struct RelationType final {
private:
	int m_value;
	const char *m_name;

	constexpr RelationType(int type, const char *name) :
		m_value(type), m_name(name)
	{}
public:
	static constexpr RelationType FINISH_FINISH() {
		return {0, "FF"};
	}
	static constexpr RelationType FINISH_START() {
		return {1, "FS"};
	}
	static constexpr RelationType START_FINISH() {
		return {2, "SF"};
	}

	static constexpr RelationType START_START() {
		return {3, "SS"};
	}

	constexpr explicit operator int() {
		return m_value;
	}

	constexpr explicit operator const char *() {
		return m_name;
	}
};

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
