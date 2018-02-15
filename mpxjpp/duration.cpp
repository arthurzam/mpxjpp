#include "duration.h"
#include "projectproperties.h"

using namespace mpxjpp;

Duration Duration::convertUnits(double duration, TimeUnit fromUnits, TimeUnit toUnits, ProjectProperties &defaults) {
    return (convertUnits(duration, fromUnits, toUnits,
                         defaults.minutesPerDay(), defaults.minutesPerWeek(),
                         defaults.daysPerMonth()));
}

Duration Duration::add(Duration a, Duration b, ProjectProperties &defaults) {
    if (b.m_duration == 0)
        return a;
    if (a.m_duration == 0)
        return b;
    double bDuration = a.m_units != b.m_units ? b.convertUnits(a.m_units, defaults).m_duration : b.m_duration;
    return Duration(a.m_duration + bDuration, a.m_units);
}
