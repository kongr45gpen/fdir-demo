#include "FreeRTOS.h"
#include <task.h>
#include "Parameters/SystemParameters.hpp"

SystemParameters systemParameters;

uint32_t SystemParameters::getTick() {
    return xTaskGetTickCount();
}
