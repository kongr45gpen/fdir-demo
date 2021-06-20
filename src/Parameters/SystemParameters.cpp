#include "FreeRTOS.h"
#include <task.h>
#include <main.hpp>
#include "Parameters/SystemParameters.hpp"

SystemParameters systemParameters;

uint32_t SystemParameters::getTick() {
    return xTaskGetTickCount();
}

void SystemParameters::temperature1StatusCallback(SystemParameters::TemperatureStatus &status) {
    if (temp1task) {
        temp1task->setOutput(status != TemperatureStatus::Disabled);
    }
}

void SystemParameters::temperature2StatusCallback(SystemParameters::TemperatureStatus &status) {
    if (temp2task) {
        temp2task->setOutput(status != TemperatureStatus::Disabled);
    }
}

SystemParameters::SystemParameters() {
    uint16_t i = 0;
    for (auto& it : parametersArray) {
        it.get().id = i++;
    }
}
