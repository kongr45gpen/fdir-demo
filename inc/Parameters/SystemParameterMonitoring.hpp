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

    MonitoringDefinition<ExpectedValueCheck<SystemParameters::TemperatureStatus>> sensor1permanentTimeoutCheck{
            2, 3, 500, 5, {
                    0b10, SystemParameters::TemperatureStatus::Nominal, 102
            }, {}
    };

    MonitoringDefinition<ExpectedValueCheck<SystemParameters::TemperatureStatus>> sensor2permanentTimeoutCheck{
            3, 4, 500, 5, {
                    0b10, SystemParameters::TemperatureStatus::Nominal, 103
            }, {}
    };

    MonitoringDefinition<LimitCheck<float>> sensor1check{
            4, 0, 500, 2, {
                    -40, 104, 100, 104
            }, {{
                        3, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };

    MonitoringDefinition<LimitCheck<float>> sensor2check{
            5, 1, 500, 2, {
                    -40, 105, 100, 105
            }, {{
                        4, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };

    MonitoringDefinition<LimitCheck<float>> sensor1permamentCheck{
            7, 0, 500, 5, {
                    -40, 107, 100, 107
            }, {{
                        3, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };

    MonitoringDefinition<LimitCheck<float>> sensor2permanentCheck{
            8, 1, 500, 5, {
                    -40, 108, 100, 108
            }, {{
                        4, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };

    MonitoringDefinition<LimitCheck<float>> sensorDeltaCheck{
            6, 2, 500, 6, {
                    -5, 106, 5, 106
            }, {{
                        5, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };

    MonitoringDefinition<LimitCheck<float>> sensorDeltaPermanentCheck{
            9, 2, 500, 11, {
                    -5, 109, 5, 109
            }, {{
                        5, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };

    MonitoringDefinition<LimitCheck<float>> subsystemSafety1check{
            10, 0, 500, 12, {
                    -std::numeric_limits<float>::infinity(), 110, 50, 110
            }, {{
                        3, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };

    MonitoringDefinition<LimitCheck<float>> subsystemSafety2check{
            11, 1, 500, 12, {
                    -std::numeric_limits<float>::infinity(), 111, 50, 111
            }, {{
                        4, 0xff, static_cast<uint64_t>(SystemParameters::TemperatureStatus::Nominal)
                }}
    };


public:
    SystemParameterMonitoring() {
        OnBoardMonitoringService &service = Services.onBoardMonitoring;

        service.addParameterMonitoringDefinition(sensor1timeoutCheck);
        service.addParameterMonitoringDefinition(sensor2timeoutCheck);
        service.addParameterMonitoringDefinition(sensor1permanentTimeoutCheck);
        service.addParameterMonitoringDefinition(sensor2permanentTimeoutCheck);
        service.addParameterMonitoringDefinition(sensor1check);
        service.addParameterMonitoringDefinition(sensor2check);
        service.addParameterMonitoringDefinition(sensorDeltaCheck);
        service.addParameterMonitoringDefinition(sensor1permamentCheck);
        service.addParameterMonitoringDefinition(sensor2permanentCheck);
        service.addParameterMonitoringDefinition(sensorDeltaPermanentCheck);
        service.addParameterMonitoringDefinition(subsystemSafety1check);
        service.addParameterMonitoringDefinition(subsystemSafety2check);
    }
};

extern std::optional<SystemParameterMonitoring> systemParameterMonitoring;

#endif //FDIR_DEMO_SYSTEMPARAMETERMONITORING_HPP
