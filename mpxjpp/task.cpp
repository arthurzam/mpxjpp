#include "task.h"
#include "projectfile.h"

using namespace mpxjpp;

Task::Task(ProjectFile &file, Task *parent) :
    FieldContainer (TaskField::FINISH + 1), ProjectEntity (file),
    m_parent(parent), m_expanded(true) {
    set_type(TaskType::FIXED_UNITS);
    set_constraintType(ConstraintType::AS_SOON_AS_POSSIBLE);
    set_taskMode(TaskMode::AUTO_SCHEDULED);
    set_active(true);

    ProjectConfig &config = file.projectConfig();

    if (config.autoTaskUniqueID())
       set_uniqueID(config.getNextTaskUniqueID());
    if (config.autoTaskID())
       set_id(config.getNextTaskID());
    if (config.autoWBS())
       generateWBS(parent);
    if (config.autoOutlineNumber())
       generateOutlineNumber(parent);
    if (config.autoOutlineLevel())
        set_outlineLevel(parent == nullptr ? 1 : parent->outlineLevel() + 1);
}

void Task::generateWBS(const Task *parent) {
    const size_t childTaskCount = parentFile().childTasks().size() + 1;
    if(!parent)
        set_wbs(uniqueID() == 0 ? "0" : std::to_string(childTaskCount));
    else {
        std::string wbs = parent->wbs();
        const std::string count = std::to_string(childTaskCount);
        if (wbs == "0")
            wbs = count;
        else {
            wbs.push_back('.');
            wbs += count;
        }
        set_wbs(wbs);
    }

}

void Task::generateOutlineNumber(const Task *parent) {
    const size_t childTaskCount = parentFile().childTasks().size() + 1;
    if(!parent)
        set_outlineNumber(uniqueID() == 0 ? "0" : std::to_string(childTaskCount));
    else {
        std::string outline = parent->outlineNumber();
        const std::string count = std::to_string(childTaskCount);

        const size_t index = outline.rfind(".0");
        if (index != std::string::npos)
            outline = outline.substr(0, index);

        if (outline == "0")
            outline = count;
        else {
            outline.push_back('.');
            outline += count;
        }
        set_outlineNumber(outline);
    }
}

TaskPtr Task::addTask() {
    auto task = std::make_shared<Task>(parentFile(), this);
    m_childTasks.push_back(task);
    parentFile().allTasks().emplace_back(task);
    set_summary(true);
    return task;
}

void Task::remove() {
    parentFile().removeTask(this);
}

ResourceAssignmentPtr Task::addResourceAssignment(Resource *resource) {
    ResourceAssignmentPtr assignment = getExistingResourceAssignment(resource);
    if (!assignment) {
        assignment = std::make_shared<ResourceAssignment>(parentFile(), shared_from_this());

        assignment->set_taskUniqueID(uniqueID());
        assignment->set_work(duration());
        assignment->set_units(ResourceAssignment::DEFAULT_UNITS);

        parentFile().allResourceAssignments().add(assignment);
        if (resource) {
            assignment->set_resourceUniqueID(resource->uniqueID());
            resource->addResourceAssignment(assignment);
        }
        m_assignments.push_back(assignment);
    }
    return assignment;
}

void Task::addResourceAssignment(ResourceAssignmentPtr assignment) {
    Resource *resource = assignment->resource().get();
    if (!resource)
        return;
    if (!getExistingResourceAssignment(resource)) {
        parentFile().allResourceAssignments().add(assignment);
        resource->addResourceAssignment(assignment);
        m_assignments.push_back(std::move(assignment));
    }
}

ResourceAssignmentPtr Task::getExistingResourceAssignment(const Resource *resource) {
    if (!resource)
        return {};
    const int resourceUniqueID = resource->uniqueID();
    auto iter = std::find_if(m_assignments.cbegin(), m_assignments.cend(), [resourceUniqueID] (const auto &assignment) {
        return assignment->resourceUniqueID() == resourceUniqueID;
    });
    return (iter == m_assignments.cend()) ? ResourceAssignmentPtr{} : *iter;
}

common::any &Task::getCurrentValue(const FieldType &field) {
    switch (field.value()) {
        case TaskField::PARENT_TASK_UNIQUE_ID:
            if (m_parent)
                return m_parent->getCachedValue(TaskField::PARENT_TASK_UNIQUE_ID);
            else
                _field_set<int>(TaskField::PARENT_TASK_UNIQUE_ID, -1);
            break;
        case TaskField::START_VARIANCE: startVariance(); break;
        case TaskField::FINISH_VARIANCE: finishVariance(); break;
        case TaskField::START_SLACK: startSlack(); break;
        case TaskField::FINISH_SLACK: finishSlack(); break;
        case TaskField::COST_VARIANCE: costVariance(); break;
        case TaskField::DURATION_VARIANCE: durationVariance(); break;
        case TaskField::WORK_VARIANCE: workVariance(); break;
        case TaskField::CV: cv(); break;
        case TaskField::SV: sv(); break;
        case TaskField::TOTAL_SLACK: totalSlack(); break;
        case TaskField::CRITICAL: critical(); break;
        case TaskField::COMPLETE_THROUGH: completeThrough(); break;
    }
    return getCachedValue(field);
}

void Task::addChildTask(const TaskPtr &child, int childOutlineLevel) {
    const int outlineLevel = this->outlineLevel();
    if ((outlineLevel + 1) == childOutlineLevel) {
       m_childTasks.push_back(child);
       set_summary(true);
    } else if (!m_childTasks.empty())
        m_childTasks.back()->addChildTask(child, childOutlineLevel);
}

void Task::addChildTask(TaskPtr child) {
    child->m_parent = this;
    if (parentFile().projectConfig().autoOutlineLevel())
       child->set_outlineLevel(outlineLevel() + 1);
    m_childTasks.push_back(std::move(child));
    set_summary(true);
}

void Task::addChildTaskBefore(TaskPtr child, const Task *previousSibling) {
    child->m_parent = this;
    if (parentFile().projectConfig().autoOutlineLevel())
       child->set_outlineLevel(outlineLevel() + 1);

    auto iter = std::find_if(m_childTasks.cbegin(), m_childTasks.cend(), FinderTask{previousSibling});
    if (iter == m_childTasks.cend())
        m_childTasks.push_back(std::move(child));
    else
        m_childTasks.insert(iter, std::move(child));
    set_summary(true);
}

Task::Date Task::completeThrough() {
    const common::any &value = getCachedValue(TaskField::COMPLETE_THROUGH);
    if(value.empty()) {
        const int percentComplete = static_cast<int>(this->percentageComplete());
        Date result;
        switch(percentComplete) {
            case 0:
                return {};
            case 100:
                result = actualFinish();
                _field_set<Date>(TaskField::COMPLETE_THROUGH, result);
                return result;
            default:
                const Date actualStart = this->actualStart();
                Duration duration = this->duration();
                double durationValue = (duration.duration() * percentComplete) / 100;
                duration = Duration{durationValue, duration.units()};
                const ProjectCalendar *calendar = effectiveCalendar();
//                value = calendar.getDate(actualStart, duration, true);
                // _field_set<Date>(TaskField::COMPLETE_THROUGH, value);
        }
    }
    return common::any_type_cast<Date>::get(value, {});
}

bool Task::critical() {
    const common::any &critical = getCachedValue(TaskField::CRITICAL);
    if(critical.empty()) {
        Duration totalSlack = this->totalSlack();
        ProjectProperties &props = parentFile().projectProperties();
        int criticalSlackLimit = props.criticalSlackLimit();
        if (criticalSlackLimit != 0 && totalSlack.duration() != 0.0 && totalSlack.units() != TimeUnit::DAYS)
           totalSlack = totalSlack.convertUnits(TimeUnit::DAYS, props);
        bool result = totalSlack.duration() <= criticalSlackLimit && percentageComplete() != 100.0 &&
                (taskMode() == TaskMode::AUTO_SCHEDULED || (durationText().empty() && startText().empty() && finishText().empty()));
        _field_set<bool>(TaskField::CRITICAL, result);
        return result;
    }
    return common::any_type_cast<bool>::get(critical, false);
}

Duration Task::durationVariance() {
    const common::any &variance = getCachedValue(TaskField::DURATION_VARIANCE);
    if(variance.empty()) {
        const Duration result = Duration::sub(duration(), baselineDuration(), parentFile().projectProperties());
        _field_set<Duration>(TaskField::DURATION_VARIANCE, result);
        return result;
    }
    return common::any_type_cast<Duration>::get(variance, {});
}

Duration Task::finishVariance() {
    // TODO: Task.java #2030
    return { };
}

Duration Task::workVariance() {
    const common::any &variance = getCachedValue(TaskField::WORK_VARIANCE);
    if(variance.empty()) {
        const Duration result = Duration::sub(work(), baselineWork(), parentFile().projectProperties());
        _field_set<Duration>(TaskField::WORK_VARIANCE, result);
        return result;
    }
    return common::any_type_cast<Duration>::get(variance, {});
}

Duration Task::totalSlack() {
    const common::any &variance = getCachedValue(TaskField::WORK_VARIANCE);
    if (variance.empty()) {
        const TimeUnit units = duration().units();

        const Duration startSlack = this->startSlack().convertUnits(units, parentFile().projectProperties());
        const Duration finishSlack = this->finishSlack().convertUnits(units, parentFile().projectProperties());

        const double startSlackDuration = startSlack.duration();
        const double finishSlackDuration = finishSlack.duration();

        Duration result;
        if (finishSlackDuration == 0.0)
            result = finishSlack;
        else if (startSlackDuration == 0.0 || startSlackDuration < finishSlackDuration)
            result = startSlack;
        else
            result = finishSlack;
        _field_set<Duration>(TaskField::TOTAL_SLACK, result);
        return result;
    }
    return common::any_type_cast<Duration>::get(variance, {});
}

void Task::set_id(int val) {
    const common::any &previous = getCachedValue(TaskField::ID);
    if(!previous.empty())
        parentFile().allTasks().unmapID(common::any_type_cast<int>::get(previous, 0));
    parentFile().allTasks().mapID(val, shared_from_this());
    _field_set<int>(TaskField::ID, val);
}

const ProjectCalendar *Task::effectiveCalendar() {
    const ProjectCalendar *result = calendar();
    if (!result)
        return parentFile().defaultCalendar().get();
    return result;
}

Relation Task::addPredecessor(TaskPtr targetTask, RelationType type, Duration lag) {
    // insert relation to targetTask->successors
    auto &successorList = targetTask->getCachedValue(TaskField::SUCCESSORS).get_assign<RelationList>({});
    int pos = -1;
    for(const auto &relation : successorList)
        if (relation.targetTask().get() == this && relation.type() == type && relation.lag() == lag) {
            pos = 0;
            break;
        }
    if (pos == -1)
        successorList.emplace_back(targetTask, shared_from_this(), type, lag);

    // insert relation to this->predecessors
    auto &predecessorList = getCachedValue(TaskField::PREDECESSORS).get_assign<RelationList>({});
    for(const auto &relation : predecessorList)
        if (relation.targetTask() == targetTask && relation.type() == type && relation.lag() == lag)
            return relation;
    predecessorList.emplace_back(shared_from_this(), targetTask, type, lag);
    return predecessorList.back();
}

static bool removeRelation(Task::RelationList &relationList, const Task *targetTask, RelationType type, Duration lag) {
    for (auto iter = relationList.begin(), end = relationList.end(); iter != end; ++iter)
        if (iter->targetTask().get() == targetTask && iter->type() == type && iter->lag() == lag)
        {
            relationList.erase(iter);
            return true;
        }
    return false;
}

bool Task::removePredecessor(const Task *targetTask, RelationType type, Duration lag) {
    auto &predecessorList = getCachedValue(TaskField::PREDECESSORS).get_assign<RelationList>({});
    if (removeRelation(predecessorList, targetTask, type, lag)) {
        auto &successorList = targetTask->getCachedValue(TaskField::SUCCESSORS).get_assign<RelationList>({});
        return removeRelation(successorList, this, type, lag);
    }
    return false;
}

bool Task::isRelated(const Task *task, const RelationList &list) {
    const int taskUniqueID = task->uniqueID();
    for (const auto &relation : list)
        if(relation.targetTask()->uniqueID() == taskUniqueID)
            return true;
    return false;
}

void TaskContainer::removed(const TaskPtr &task) {
    m_uniqueIDMap.erase(task->uniqueID());
    m_idMap.erase(task->id());

    Task *parentTask = task->parentTask();
    if (parentTask)
        parentTask->removeChildTask(task.get());
    else {
        std::vector<TaskPtr> &children = m_mpx.childTasks();
        children.erase(std::find_if(children.cbegin(), children.cend(), Task::FinderTask{task.get()}));
    }

    auto &assignments = m_mpx.allResourceAssignments();
    for (auto iter = assignments.cbegin(), end = assignments.cend(); iter != end; ++iter) {
        if ((*iter)->task() == task.get()) {
            if (Resource *resource = (*iter)->resource().get())
                resource->removeResourceAssignment((*iter).get());
            iter = assignments.remove(iter);
        }
    }

    while (true) {
        auto &children = task->childTasks();
        if (children.empty())
            break;
        remove(std::find_if(children.cbegin(), children.cend(), Task::FinderTask{children.front().get()}));
    }
}

TaskPtr TaskContainer::create() {
    auto task = std::make_shared<Task>(m_mpx);
    add(task);
    m_mpx.childTasks().push_back(task);
    return task;
}

void TaskContainer::updateStructure() {
    if (size() == 0)
        return;
    std::sort(begin(), end());
    m_mpx.childTasks().clear();

    Task *lastTask = nullptr;
    int lastLevel = -1;
    const bool autoWbs = m_mpx.projectConfig().autoWBS();
    const bool autoOutlineNumber = m_mpx.projectConfig().autoOutlineNumber();

    for (const auto &task : *this) {
        task->clearChildTasks();
        Task *parent = nullptr;
        if (!task->null()) {
            int level = task->outlineLevel();
            if (lastTask != nullptr) {
                if (level == lastLevel) {
                   parent = lastTask->parentTask();
                } else if (level > lastLevel)
                    parent = lastTask;
                else {
                    while (level <= lastLevel) {
                        parent = lastTask->parentTask();
                        if (!parent)
                            break;
                        lastLevel = parent->outlineLevel();
                        lastTask = parent;
                    }
                }
            }
            lastTask = task.get();
            lastLevel = level;

            if (autoWbs || task->wbs().empty())
               task->generateWBS(parent);
            if (autoOutlineNumber)
               task->generateOutlineNumber(parent);
        }
        if (!parent)
           m_mpx.childTasks().push_back(task);
        else
           parent->addChildTask(task);
    }
}

void TaskContainer::synchronizeTaskIDToHierarchy() {
    clear();
    const int currentID = (!getByID(0) ? 1 : 0);
    synchroizeTaskIDToHierarchy(m_mpx.childTasks(), currentID);
}

int TaskContainer::synchroizeTaskIDToHierarchy(const std::vector<TaskPtr> &tasks, int currentID) {
    for (auto &task : tasks) {
        task->set_id(currentID++);
        add(task);
        currentID = synchroizeTaskIDToHierarchy(task->childTasks(), currentID);
    }
    return currentID;
}
