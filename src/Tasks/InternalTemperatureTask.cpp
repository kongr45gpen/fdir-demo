#include <definitions.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Logger.hpp>
#include <ServicePool.hpp>
#include "Tasks/InternalTemperatureTask.hpp"

void InternalTemperatureTask::operator()() {
    AFEC0_ChannelsDisable(AFEC_CH10_MASK);
    AFEC0_ChannelGainSet(AFEC_CH11, AFEC_CHANNEL_GAIN_X1);
    AFEC0_ChannelOffsetSet(AFEC_CH11, 690);
    AFEC0_ChannelsEnable(AFEC_CH11_MASK);

    for(;;){
        AFEC0_ConversionStart();
        PIO_PinToggle(PIO_PIN_PA23);
        vTaskDelay(pdMS_TO_TICKS(100));

        uint16_t rawTemperature = AFEC0_ChannelResultGet(AFEC_CH11);
        float temperature = 30 + (rawTemperature - 4000.0f) / 46.27f;

        systemParameters.temperatureInternal.setValue(temperature);
    }
}

InternalTemperatureTask::InternalTemperatureTask() {

}
