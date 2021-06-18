#ifndef FDIR_DEMO_UARTRXTASK_HPP
#define FDIR_DEMO_UARTRXTASK_HPP

#include "FreeRTOS.h"
#include <queue.h>
#include <atomic>
#include "etl/String.hpp"
#include <Services/Parameter.hpp>
#include <Peripherals/MCP9808.hpp>

class TemperatureTask {
public:
    void operator()();

    TemperatureTask(Parameter<float>& parameter, Parameter<SystemParameters::TemperatureStatus>& statusParameter, uint8_t sensorI2c, PIO_PIN buttonPin);
private:
    Parameter<float>& parameter;
    Parameter<SystemParameters::TemperatureStatus>& statusParameter;
    MCP9808 mcp9808;
    const PIO_PIN buttonPin;
};


#endif //FDIR_DEMO_UARTRXTASK_HPP
