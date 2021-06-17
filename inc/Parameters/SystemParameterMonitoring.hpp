#ifndef FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP
#define FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP

#include <Services/OnBoardMonitoringService.hpp>
#include <Services/OnBoardMonitoringChecks.hpp>
#include <ServicePool.hpp>

class SystemParameterMonitoring {
    template<class Check>
    using MonitoringDefinition = OnBoardMonitoringService::ParameterMonitoringDefinition<typename Check::type, Check>;

    MonitoringDefinition<ExpectedValueCheck<SystemParameters::TemperatureStatus>> sensor1timeoutCheck{
            0, 3, 500, 2, {
                    0b10, SystemParameters::TemperatureStatus::Nominal, 100
            }, {}
    };

    MonitoringDefinition<ExpectedValueCheck<SystemParameters::TemperatureStatus>> sensor2timeoutCheck{
            1, 4, 500, 2, {
                    0b10, SystemParameters::TemperatureStatus::Nominal, 101
            }, {}
    };

    MonitoringDefinition<LimitCheck<float>> sensor1check{
            4, 0, 200, 5, {
                    20, 104, 30, 104
            }, {{
                        3, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };

    MonitoringDefinition<LimitCheck<float>> sensor2check{
            5, 1, 1000, 1, {
                    10, 105, 40, 105
            }, {{
                        4, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };

    MonitoringDefinition<LimitCheck<float>> sensorDeltaCheck{
            6, 2, 200, 5, {
                    -5, 106, 5, 106
            }, {{
                        5, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };


public:
    SystemParameterMonitoring() {
        OnBoardMonitoringService &service = Services.onBoardMonitoring;

        service.addParameterMonitoringDefinition(sensor1timeoutCheck);
        service.addParameterMonitoringDefinition(sensor2timeoutCheck);
        service.addParameterMonitoringDefinition(sensor1check);
        service.addParameterMonitoringDefinition(sensor2check);
        service.addParameterMonitoringDefinition(sensorDeltaCheck);
    }
};

extern std::optional<SystemParameterMonitoring> systemParameterMonitoring;

#endif //FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP
