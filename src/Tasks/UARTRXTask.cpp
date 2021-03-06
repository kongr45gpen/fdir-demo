#include <optional>
#include "Tasks/UARTRXTask.hpp"
#include <Logger.hpp>
#include <definitions.h>
#include <MessageParser.hpp>
#include <COBS.hpp>

using ECSSMessage = Message;

std::optional<UARTRXTask> uartRXtask;
UARTRXTask::Message UARTRXTask::buffer1;

UARTRXTask::UARTRXTask() {
    rxQueue = xQueueCreate(Capacity, sizeof(Message));

    USART1_Read(&currentRXbyte, 1);

    USART1_ReadCallbackRegister([](uintptr_t object) {
        // This function is called whenever a single byte arrives

        auto rxTask = reinterpret_cast<UARTRXTask*>(object); // Work-around through library API to pass object

        if (USART1_ReadCountGet() == 0) {
            // Some error occurred
        } else {
            rxTask->ingress();
        }

        // Initiate next read
        USART1_Read(&(rxTask->currentRXbyte), 1);
    }, reinterpret_cast<uintptr_t>(this));
}

void UARTRXTask::operator()() {
    while (true) {
        xQueueReceive(rxQueue, static_cast<void*>(&buffer2), portMAX_DELAY);

        if (overRun) {
            overRun = false;
            LOG_ERROR << "RX too large message";
        }

        cobsBuffer = COBSdecode<MaxInputSize>(reinterpret_cast<uint8_t*>(buffer2.message), MaxInputSize);

        ECSSMessage ecss = MessageParser::parseECSSTC(reinterpret_cast<const uint8_t*>(cobsBuffer.c_str()));

        LOG_INFO << "Received new [" << ecss.serviceType << "," << ecss.messageType << "] TC";

        MessageParser::execute(ecss);
    }
}


