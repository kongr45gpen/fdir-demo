#ifndef FDIR_DEMO_SYSTEMPARAMETERS_HPP
#define FDIR_DEMO_SYSTEMPARAMETERS_HPP

#include <cstdint>
#include <Services/Parameter.hpp>

class SystemParameters {
public:
    enum class TemperatureStatus : uint8_t {
        Nominal,
        Timeout,
        Disabled
    };

    Parameter<float> temperature1Value{0.0};
    Parameter<float> temperature2Value{0.0};
    FunctionParameter<float> temperatureDelta{[this]() -> float {
        return temperature2Value.getValue() - temperature1Value.getValue();
    }};
    Parameter<float> temperatureInternalValue{0.0};

    Parameter<TemperatureStatus> temperature1Status{ TemperatureStatus::Nominal };
    Parameter<TemperatureStatus> temperature2Status{ TemperatureStatus::Nominal };
    Parameter<TemperatureStatus> temperature12Status{ TemperatureStatus::Nominal };

    /**
     * The key of the array is the ID of the parameter as specified in PUS
     */
    etl::array<std::reference_wrapper<ParameterBase>, ECSS_PARAMETER_COUNT> parametersArray = {
        std::ref<ParameterBase>(temperature1Value),
        std::ref<ParameterBase>(temperature2Value),
        std::ref<ParameterBase>(temperatureDelta),
        std::ref<ParameterBase>(temperature1Status),
        std::ref<ParameterBase>(temperature2Status),
        std::ref<ParameterBase>(temperature12Status),
        std::ref<ParameterBase>(temperatureInternalValue),
    };

    SystemParameters() = default;

    template<typename Value>
    Value getParameterValue(uint16_t parameterId) {
        return reinterpret_cast<Parameter<Value>*>(&(parametersArray[parameterId].get()))->getValue();
    }
};

extern SystemParameters systemParameters;

#endif //FDIR_DEMO_SYSTEMPARAMETERS_HPP
