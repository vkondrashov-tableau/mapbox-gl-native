#pragma once

#include <mbgl/style/property_value.hpp>
#include <mbgl/renderer/property_evaluation_parameters.hpp>
#include <mbgl/renderer/possibly_evaluated_property_value.hpp>
#include <mbgl/renderer/cross_faded_property_evaluator.hpp>

namespace mbgl {

template <typename T>
class DataDrivenPropertyEvaluator {
public:
    using ResultType = PossiblyEvaluatedPropertyValue<T>;

    DataDrivenPropertyEvaluator(const PropertyEvaluationParameters& parameters_, T defaultValue_)
        : parameters(parameters_),
          defaultValue(std::move(defaultValue_)) {}

    ResultType operator()(const style::Undefined&) const {
        return ResultType(defaultValue);
    }

    ResultType operator()(const T& constant) const {
        return ResultType(constant);
    }

    ResultType operator()(const style::PropertyExpression<T>& expression) const {
        if (!expression.isFeatureConstant()) {
            auto returnExpression = expression;
            returnExpression.useIntegerZoom = parameters.useIntegerZoom;
            return ResultType(returnExpression);
        } else if (!parameters.useIntegerZoom) {
            return ResultType(expression.evaluate(parameters.z));
        } else {
            return ResultType(expression.evaluate(floor(parameters.z)));
        }
    }

private:
    const PropertyEvaluationParameters& parameters;
    T defaultValue;
};

template <typename T>
class DataDrivenPropertyEvaluator<Faded<T>> {
public:
    using ResultType = PossiblyEvaluatedPropertyValue<Faded<T>>;

    DataDrivenPropertyEvaluator(const PropertyEvaluationParameters& parameters_, T defaultValue_)
    : parameters(parameters_),
      defaultValue(std::move(defaultValue_)) {}

    ResultType operator()(const T& constant) const {
        return ResultType(calculate(constant, constant, constant));
    }

    ResultType operator()(const style::Undefined& ) const {
        return ResultType(calculate(defaultValue, defaultValue, defaultValue));
    }

    ResultType operator()(const style::CameraFunction<T>& function) const {
        const T evaluated = parameters.useIntegerZoom ? function.evaluate(floor(parameters.z)) : function.evaluate(parameters.z);
        return ResultType(calculate(evaluated, evaluated, evaluated));
    }

    template <class Function>
    ResultType operator()(const Function& function) const {
        return ResultType(function);
    }


private:
    Faded<T> calculate(const T& min, const T& mid, const T& max) const {
        const float z = parameters.z;
        const float fraction = z - std::floor(z);
        const std::chrono::duration<float> d = parameters.defaultFadeDuration;
        const float t =
            d != std::chrono::duration<float>::zero()
                ? std::min((parameters.now - parameters.zoomHistory.lastIntegerZoomTime) / d, 1.0f)
                : 1.0f;

        return z > parameters.zoomHistory.lastIntegerZoom
            ? Faded<T> { min, mid, 2.0f, 1.0f, fraction + (1.0f - fraction) * t }
            : Faded<T> { max, mid, 0.5f, 1.0f, 1 - (1 - t) * fraction };
    };

    const PropertyEvaluationParameters& parameters;
    T defaultValue;
};

} // namespace mbgl
