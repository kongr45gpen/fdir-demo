#ifndef FDIR_DEMO_MAIN_HPP
#define FDIR_DEMO_MAIN_HPP

#include <optional>
#include "Tasks/TemperatureTask.hpp"
#include "Tasks/InternalTemperatureTask.hpp"

extern std::optional<TemperatureTask> temp1task;
extern std::optional<TemperatureTask> temp2task;
extern std::optional<InternalTemperatureTask> tempInternal;

#endif //FDIR_DEMO_MAIN_HPP
