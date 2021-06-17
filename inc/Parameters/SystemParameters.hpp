#ifndef FDIR_DEMO_SYSTEMPARAMETERS_HPP
#define FDIR_DEMO_SYSTEMPARAMETERS_HPP

#include <cstdint>
#include <Services/Parameter.hpp>

class SystemParameters {
public:
    enum class TemperatureStatus : uint8_t {
        Nominal = 0b01,
        Timeout = 0b10,
        Disabled = 0b00
    };

    Parameter<float> temperature1{0.0};
    Parameter<float> temperature2{0.0};
    FunctionParameter<float> temperatureDelta{[this]() -> float {
        return temperature2.getValue() - temperature1.getValue();
    }};
    Parameter<float> temperatureInternal{0.0};

    Parameter<TemperatureStatus> temperature1Status{ TemperatureStatus::Nominal };
    Parameter<TemperatureStatus> temperature2Status{ TemperatureStatus::Nominal };
    FunctionParameter<TemperatureStatus> temperature12Status{[this]() {
        if (temperature1Status.getValue() == TemperatureStatus::Nominal && temperature2Status.getValue() == TemperatureStatus::Nominal) {
            return TemperatureStatus::Nominal;
        } else {
            return TemperatureStatus::Disabled;
        }
    }};

    /**
     * The key of the array is the ID of the parameter as specified in PUS
     */
    etl::array<std::reference_wrapper<ParameterBase>, ECSS_PARAMETER_COUNT> parametersArray = {
        std::ref<ParameterBase>(temperature1),
        std::ref<ParameterBase>(temperature2),
        std::ref<ParameterBase>(temperatureDelta),
        std::ref<ParameterBase>(temperature1Status),
        std::ref<ParameterBase>(temperature2Status),
        std::ref<ParameterBase>(temperature12Status),
        std::ref<ParameterBase>(temperatureInternal),
    };

    SystemParameters() = default;

    template<typename Value>
    Value getParameterValue(uint16_t parameterId) {
        return static_cast<ReadableParameter<Value>*>(&(parametersArray[parameterId].get()))->getValue();
    }

    uint64_t convertParameterValue(uint16_t parameterId) {
        return parametersArray[parameterId].get().convertValue();
    }
};

extern SystemParameters systemParameters;

#endif //FDIR_DEMO_SYSTEMPARAMETERS_HPP
