#include <Logger.hpp>
#include <ServicePool.hpp>
#include "Tasks/TemperatureTask.hpp"

TemperatureTask::TemperatureTask(Parameter<float> &parameter, uint8_t sensorI2c) : parameter(parameter), mcp9808(sensorI2c)
{
    parameter.setValue(10.0f);
}

void TemperatureTask::operator()() {
    LOG_WARNING << parameter.getValue();

    while (true) {
        float temperature = 0;
        mcp9808.getTemp(temperature);

        parameter.setValue(temperature);

        LOG_DEBUG << "T [" << pcTaskGetName(nullptr) << "]: " << parameter.getValue();

        vTaskDelay(130);
    }
}
