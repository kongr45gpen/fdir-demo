#include <Logger.hpp>
#include <ServicePool.hpp>
#include "Tasks/TemperatureTask.hpp"

TemperatureTask::TemperatureTask(Parameter<float> &parameter,
                                 Parameter<SystemParameters::TemperatureStatus> &statusParameter, uint8_t sensorI2c)
        : parameter(parameter), statusParameter(statusParameter), mcp9808(sensorI2c) {
}

void TemperatureTask::operator()() {
    LOG_WARNING << parameter.getValue();

    while (true) {
        float temperature = 0;
        bool status = mcp9808.getTemp(temperature);

        if (statusParameter.getValue() != SystemParameters::TemperatureStatus::Disabled) {
            if (status) {
                statusParameter.setValue(SystemParameters::TemperatureStatus::Nominal);
            } else {
                statusParameter.setValue(SystemParameters::TemperatureStatus::Timeout);
            }
        }

        if (!BTN0_Get()) {
            temperature += 80;
        }

        // TODO: Set parameter on timeout?
        parameter.setValue(temperature);

        LOG_DEBUG << "T [" << pcTaskGetName(nullptr) << "]: " << parameter.getValue();

        vTaskDelay(130);
    }
}
