#pragma once

#include "maths\Fractal.hpp"

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
                CELLULAR,
                CELLULAR_SQUARED,
                CELLULAR_CUBIC
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
                FXX                         persistence; // How much of each octave contributes to overall noise value, Nth octave is multiplied by data.persistence^N (should take a value between 0 and 1).
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
            static inline FXX doOp(Operation op, FXX a, FXX b) {
                using namespace hproc::Noise;
                switch (op) {
                    case Operation::ADD:
                        return a + b;
                    case Operation::DIVIDE:
                        return a / b;
                    case Operation::MULTIPLY:
                        return a * b;
                    case Operation::SUBTRACT:
                        return a - b;
                    default:
                        return 0.0;
                }
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getRawNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data) {
                return hm::Fractal::genSimplexWithOctaves(data.octaves, data.persistence, data.frequency, coords);
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getRawNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data) {
                return hm::Fractal::genSimplexWithOctaves(data.octaves, data.persistence, data.frequency, coords);
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getRawNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data) {
                return hm::Fractal::genSimplexWithOctaves(data.octaves, data.persistence, data.frequency, coords);
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getRidgedNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += (((FXX)1.0 - glm::abs(hm::Fractal::genSimplex(coords * frequency))) * (FXX)2.0 - (FXX)1.0) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getRidgedNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += (((FXX)1.0 - glm::abs(hm::Fractal::genSimplex(coords * frequency))) * (FXX)2.0 - (FXX)1.0) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getRidgedNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += (((FXX)1.0 - glm::abs(hm::Fractal::genSimplex(coords * frequency))) * (FXX)2.0 - (FXX)1.0) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getBillowNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += (glm::abs(hm::Fractal::genSimplex(coords * frequency)) * (FXX)2.0 - (FXX)1.0) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getBillowNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += (glm::abs(hm::Fractal::genSimplex(coords * frequency)) * (FXX)2.0 - (FXX)1.0) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getBillowNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += (glm::abs(hm::Fractal::genSimplex(coords * frequency)) * (FXX)2.0 - (FXX)1.0) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getAbsNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += glm::abs(hm::Fractal::genSimplex(coords * frequency)) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getAbsNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += glm::abs(hm::Fractal::genSimplex(coords * frequency)) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getAbsNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += glm::abs(hm::Fractal::genSimplex(coords * frequency)) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getCellularNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    glm::tvec2<FXX, glm::highp> ff = hm::Fractal::genCellular(coords * frequency);
                    total += (ff.y - ff.x) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getCellularNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += hm::Fractal::genCellular(coords * frequency) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getCellularNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    total += hm::Fractal::genCellular(coords * frequency) * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getCellularSquaredNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    glm::tvec2<FXX, glm::highp> ff = hm::Fractal::genCellular(coords * frequency);
                    FXX temp = (ff.y - ff.x);
                    total += temp * temp * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getCellularSquaredNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    glm::tvec2<FXX, glm::highp> ff = hm::Fractal::genCellular(coords * frequency);
                    FXX temp = (ff.y - ff.x);
                    total += temp * temp * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getCellularSquaredNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    glm::tvec2<FXX, glm::highp> ff = hm::Fractal::genCellular(coords * frequency);
                    FXX temp = (ff.y - ff.x);
                    total += temp * temp * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getCellularCubicNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    glm::tvec2<FXX, glm::highp> ff = hm::Fractal::genCellular(coords * frequency);
                    FXX temp = (ff.y - ff.x);
                    total += temp * temp * temp * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getCellularCubicNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    glm::tvec2<FXX, glm::highp> ff = hm::Fractal::genCellular(coords * frequency);
                    FXX temp = (ff.y - ff.x);
                    total += temp * temp * temp * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            static inline FXX getCellularCubicNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data) {
                FXX total = (FXX)0.0;
                FXX frequency = data.frequency;
                FXX amplitude = (FXX)1.0;

                // We have to keep track of the largest possible amplitude,
                // because each octave adds more, and we need a value in [-1, 1].
                FXX maxAmplitude = (FXX)0.0;

                for (ui8 i = 0; i < data.octaves; ++i) {
                    glm::tvec2<FXX, glm::highp> ff = hm::Fractal::genCellular(coords * frequency);
                    FXX temp = (ff.y - ff.x);
                    total += temp * temp * temp * amplitude;

                    frequency *= (FXX)2.0;
                    maxAmplitude += amplitude;
                    amplitude *= data.persistence;
                }

                return total / maxAmplitude;
            }

            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX getNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data, const FXX* start = nullptr) {
                FXX res;

                Operation modOp = data.op;
                // If no start point is provided, make one.
                if (start == nullptr) {
                    res = (FXX)1.0;
                    data.op = Operation::MULTIPLY;
                } else {
                    res = *start;
                }

                switch (data.type) {
                    case Type::RAW:
                        res = getRawNoiseValue(coords, data);
                        break;
                    case Type::RIDGED:
                        res = getRidgedNoiseValue(coords, data);
                        break;
                    case Type::BILLOW:
                        res = getBillowNoiseValue(coords, data);
                        break;
                    case Type::ABS:
                        res = getAbsNoiseValue(coords, data);
                        break;
                    case Type::CELLULAR:
                        res = getCellularNoiseValue(coords, data);
                        break;
                    case Type::CELLULAR_SQUARED:
                        res = getCellularSquaredNoiseValue(coords, data);
                        break;
                    case Type::CELLULAR_CUBIC:
                        res = getCellularCubicNoiseValue(coords, data);
                        break;
                }

                doOp(modOp, res, data.modifier);

                switch (data.multiplier) {
                    case Multiplier::SQUARE:
                        res = res * res;
                    case Multiplier::CUBIC:
                        res = res * res * res;
                    case Multiplier::QUARTIC:
                        res = res * res * res * res;
                    case Multiplier::QUINTIC:
                        res = res * res * res * res * res;
                }

                res = res * (data.bound.hi - data.bound.lo) * (FXX)0.5 + (data.bound.hi + data.bound.lo) * (FXX)0.5;

                if (data.clamp.hi != (FXX)0.0 || data.clamp.lo != (FXX)0.0) {
                    if (res > data.clamp.hi) {
                        res = data.clamp.hi;
                    } else if (res < data.clamp.lo) {
                        res = data.clamp.lo;
                    }
                }

                return res;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX getNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data, const FXX* start = nullptr) {
                FXX res;

                Operation modOp = data.op;
                // If no start point is provided, make one.
                if (start == nullptr) {
                    res = (FXX)1.0;
                    data.op = Operation::MULTIPLY;
                } else {
                    res = *start;
                }

                switch (type) {
                    case Type::RAW:
                        res = getRawNoiseValue(coords, data);
                        break;
                    case Type::RIDGED:
                        res = getRidgedNoiseValue(coords, data);
                        break;
                    case Type::BILLOW:
                        res = getBillowNoiseValue(coords, data);
                        break;
                    case Type::ABS:
                        res = getAbsNoiseValue(coords, data);
                        break;
                    case Type::CELLULAR:
                        res = getCellularNoiseValue(coords, data);
                        break;
                }

                doOp(modOp, res, modifier);

                switch (data.multiplier) {
                    case Multiplier::SQUARE:
                        res = res * res;
                    case Multiplier::CUBIC:
                        res = res * res * res;
                    case Multiplier::QUARTIC:
                        res = res * res * res * res;
                    case Multiplier::QUINTIC:
                        res = res * res * res * res * res;
                }

                res = res * (data.bound.hi - data.bound.lo) * (FXX)0.5 + (data.bound.hi + data.bound.lo) * (FXX)0.5;

                if (data.clamp.hi != (FXX)0.0 || data.clamp.lo != (FXX)0.0) {
                    if (res > data.clamp.hi) {
                        res = data.clamp.hi;
                    } else if (res < data.clamp.lo) {
                        res = data.clamp.lo;
                    }
                }

                return res;
            }
            template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
            FXX getNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data, const FXX* start = nullptr) {
                FXX res;

                Operation modOp = data.op;
                // If no start point is provided, make one.
                if (start == nullptr) {
                    res = (FXX)1.0;
                    data.op = Operation::MULTIPLY;
                } else {
                    res = *start;
                }

                switch (type) {
                    case Type::RAW:
                        res = getRawNoiseValue(coords, data);
                        break;
                    case Type::RIDGED:
                        res = getRidgedNoiseValue(coords, data);
                        break;
                    case Type::BILLOW:
                        res = getBillowNoiseValue(coords, data);
                        break;
                    case Type::ABS:
                        res = getAbsNoiseValue(coords, data);
                        break;
                    case Type::CELLULAR:
                        res = getCellularNoiseValue(coords, data);
                        break;
                }

                doOp(modOp, res, modifier);

                switch (data.multiplier) {
                    case Multiplier::SQUARE:
                        res = res * res;
                    case Multiplier::CUBIC:
                        res = res * res * res;
                    case Multiplier::QUARTIC:
                        res = res * res * res * res;
                    case Multiplier::QUINTIC:
                        res = res * res * res * res * res;
                }

                res = res * (data.bound.hi - data.bound.lo) * (FXX)0.5 + (data.bound.hi + data.bound.lo) * (FXX)0.5;

                if (data.clamp.hi != (FXX)0.0 || data.clamp.lo != (FXX)0.0) {
                    if (res > data.clamp.hi) {
                        res = data.clamp.hi;
                    } else if (res < data.clamp.lo) {
                        res = data.clamp.lo;
                    }
                }

                return res;
            }
        }
    }
}
namespace hproc = hemlock::procedural;
