#include <Logger.hpp>
#include <ServicePool.hpp>
#include "Tasks/TemperatureTask.hpp"

TemperatureTask::TemperatureTask(Parameter<float> &parameter,
                                 CallbackParameter<SystemParameters::TemperatureStatus> &statusParameter, uint8_t sensorI2c, PIO_PIN sensorPin, PIO_PIN buttonPin)
        : parameter(parameter), statusParameter(statusParameter), mcp9808(sensorI2c), sensorPin(sensorPin), buttonPin(buttonPin) {
    PIO_PinWrite(sensorPin, true);
}

void TemperatureTask::operator()() {
    taskHandle = xTaskGetCurrentTaskHandle();

    while (true) {
        vTaskDelay(130);

        float temperature = 0;
        bool status = mcp9808.isIDok();

        if (status) {
            status = mcp9808.getTemp(temperature);
        }

        if (status) {
            statusParameter.setValue(SystemParameters::TemperatureStatus::Nominal);
        } else {
            statusParameter.setValue(SystemParameters::TemperatureStatus::Timeout);
        }

        if (!PIO_PinRead(buttonPin)) {
            temperature += 80;
        }

        // TODO: Set parameter on timeout?
        parameter.setValue(temperature);

        LOG_DEBUG << "T [" << pcTaskGetName(nullptr) << "]: " << parameter.getValue();
    }
}

void TemperatureTask::setOutput(bool output) {
    PIO_PinWrite(sensorPin, output);

    if (taskHandle != nullptr) {
        if (output) {
            vTaskResume(taskHandle);
        } else {
            vTaskSuspend(taskHandle);
        }
    }
}
