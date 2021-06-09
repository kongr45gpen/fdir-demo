#ifndef FDIR_DEMO_UARTRXTASK_HPP
#define FDIR_DEMO_UARTRXTASK_HPP

#include "FreeRTOS.h"
#include <queue.h>
#include <atomic>
#include "etl/String.hpp"

class UARTRXTask {
public:
    static constexpr int MaxInputSize = 100;

    void operator()();

    UARTRXTask();
private:
    struct Message {
        char message[MaxInputSize];
    };

    static constexpr int Capacity = 10;

    char currentRXbyte;
    char currentRXbuffer[MaxInputSize];
    int currentReadLocation = 0;

    std::atomic<bool> overRun = false;

    QueueHandle_t rxQueue;

    Message buffer{};
    etl::string<MaxInputSize> cobsBuffer;
};

extern std::optional<UARTRXTask> uartRXtask;



#endif //FDIR_DEMO_UARTRXTASK_HPP
