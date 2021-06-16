/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <memory.h>
#include <Logger.hpp>
#include <Parameters/SystemParameters.hpp>
#include <ServicePool.hpp>
#include <Parameters/SystemParameterMonitoring.hpp>
#include <Tasks/UARTTask.hpp>
#include <Tasks/UARTRXTask.hpp>
#include <Tasks/ECSSTask.h>
#include <Peripherals/MCP9808.hpp>
#include <Tasks/TemperatureTask.hpp>
#include "definitions.h"                // SYS function prototypes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

volatile int xTask1 = 1;

_Noreturn void xTask1Code(void *pvParameters){
    AFEC0_ChannelsDisable(AFEC_CH10_MASK);
    AFEC0_ChannelGainSet(AFEC_CH11, AFEC_CHANNEL_GAIN_X1);
    AFEC0_ChannelOffsetSet(AFEC_CH11, 690);
    AFEC0_ChannelsEnable(AFEC_CH11_MASK);


    for(;;){
//        PIO_PinToggle(PIO_PIN_PA23);
        //pinval = PIO_PinRead(PIO_PIN_PA23);
        AFEC0_ConversionStart();
        PIO_PinToggle(PIO_PIN_PA23);
        vTaskDelay(pdMS_TO_TICKS(100));

        uint16_t rawTemperature = AFEC0_ChannelResultGet(AFEC_CH11);
        float temperature = 30 + (rawTemperature - 4000.0f) / 46.27f;

        if (!BTN0_Get()) {
            temperature += 80;
            LOG_DEBUG << "Fake input activated";
        }

        systemParameters.temperatureInternalValue.setValue(temperature);
    }

};

/**
 * Just calls the operator() function of a task
 * @param pvParameters Pointer to object of type Task
 */
template<class Task>
static void vClassTask(void *pvParameters) {
    (static_cast<Task *>(pvParameters))->operator()();
}

std::optional<TemperatureTask> temp1;
std::optional<TemperatureTask> temp2;

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    Logger::format.precision(2);

    systemParameterMonitoring.emplace();
    uartTask.emplace();
    uartRXtask.emplace();
    ecssTask.emplace();

    temp1.emplace(systemParameters.temperature1Value, 0);
    temp2.emplace(systemParameters.temperature2Value, 2);

    xTaskCreate(xTask1Code, "Task1",2500, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vClassTask<ECSSTask>, "ECSS",3000, &*ecssTask, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(vClassTask<UARTTask>, "UART_Tx", 1000, &*uartTask, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vClassTask<UARTRXTask>, "UART_Rx", 2500, &*uartRXtask, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(vClassTask<TemperatureTask>, "T1", 1000, &*temp1, tskIDLE_PRIORITY + 1, nullptr);
    xTaskCreate(vClassTask<TemperatureTask>, "T2", 2000, &*temp2, tskIDLE_PRIORITY + 1, nullptr);

    vTaskStartScheduler();
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while ( true )
    {


        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }
#pragma clang diagnostic pop

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}






/*******************************************************************************
 End of File
*/
