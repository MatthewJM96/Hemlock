#pragma once

namespace hemlock {
    namespace procedural {
        namespace Noise {
            // TODO(Matthew): Should this and Fractal.h be in the same namespace?

            enum class Operation {
                ADD,
                SUBTRACT,
                MULTIPLY,
                DIVIDE
            };
            enum class Type {
                NONE,
                RAW,
                RIDGED,
                BILLOW,
                ABS,
                CELLULAR
            };
            enum class Multiplier {
                NONE,
                SQUARE,
                CUBIC,
                QUARTIC,
                QUINTIC
            };

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            struct Bound {
                FXX hi, lo;
            };
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            struct Clamp {
                FXX hi, lo;
            };

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            struct NoiseData {
                Type                        type;        // The type of noise to use.
                Multiplier                  multiplier;  // Whether (and to what extent) to exponentiate the noise value.
                Operation                   op;          // How to apply this stage's noise value to the previous value. NOTE: The operations are only applied in cases where an original value is provided.
                Bound<FXX>                  bound;       // The bounds of the fractal noise generator (defaults to -1.0 to 1.0).
                Clamp<FXX>                  clamp;       // The high and low values to clamp the final noise value to (a value of 0.0 for either means that clamping is not applied).
                ui8                         octaves;     // The number of iterations of the fractal noise generator to apply (higher number = more detail).
                FXX                         persistence; // How much of each octave contributes to overall noise value, Nth octave is multiplied by persistence^N (should take a value between 0 and 1).
                FXX                         frequency;   // Multiplies the coordinates, each subsequent octave than the first has double the previous octave's frequency.
                FXX                         modifier;    // Modifies the noise value (before all else including multiplier) using the same operation as specified for the noise itself.
            };

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            NoiseData<FXX> getDefaultNoiseData() {
                return {
                    Type::NONE,
                    Multiplier::NONE,
                    Operation::MULTIPLY,
                    { (FXX)1.0, (FXX)-1.0 },
                    { (FXX)0.0, (FXX) 0.0 },
                    1,
                    (FXX)0.0,
                    (FXX)0.0,
                    (FXX)1.0
                };
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX getNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data, const FXX* start = nullptr);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX getNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data, const FXX* start = nullptr);
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX getNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data, const FXX* start = nullptr);
        }
    }
}
namespace hproc = hemlock::procedural;
