#ifndef FDIR_DEMO_UARTRXTASK_HPP
#define FDIR_DEMO_UARTRXTASK_HPP

#include "FreeRTOS.h"
#include <queue.h>
#include <atomic>
#include "etl/String.hpp"
#include <Services/Parameter.hpp>
#include <Peripherals/MCP9808.hpp>
#include "Parameters/SystemParameters.hpp"

class TemperatureTask {
public:
    void operator()();

    TemperatureTask(Parameter<float>& parameter, CallbackParameter<SystemParameters::TemperatureStatus>& statusParameter, uint8_t sensorI2c, PIO_PIN sensorPin, PIO_PIN buttonPin);

    void setOutput(bool output);
    void restart();
private:
    Parameter<float>& parameter;
    CallbackParameter<SystemParameters::TemperatureStatus>& statusParameter;
    MCP9808 mcp9808;
    const PIO_PIN sensorPin;
    const PIO_PIN buttonPin;

    TaskHandle_t taskHandle = nullptr;
};


#endif //FDIR_DEMO_UARTRXTASK_HPP
