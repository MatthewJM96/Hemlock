#include "stdafx.h"

#include "procedural\Noise.h"
#include "maths\Fractal.h"

template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX doOp(hproc::Noise::Operation op, FXX a, FXX b) {
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
inline FXX getRawNoiseValue(glm::tvec2<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    return hm::Fractal::genSimplexWithOctaves(data.octaves, data.persistence, data.frequency, coords);
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getRawNoiseValue(glm::tvec3<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    return hm::Fractal::genSimplexWithOctaves(data.octaves, data.persistence, data.frequency, coords);
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getRawNoiseValue(glm::tvec4<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    return hm::Fractal::genSimplexWithOctaves(data.octaves, data.persistence, data.frequency, coords);
}

template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getRidgedNoiseValue(glm::tvec2<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += (((FXX)1.0 - glm::abs(hm::Fractal::genSimplex(coords * frequency))) * (FXX)2.0 - (FXX)1.0) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getRidgedNoiseValue(glm::tvec3<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += (((FXX)1.0 - glm::abs(hm::Fractal::genSimplex(coords * frequency))) * (FXX)2.0 - (FXX)1.0) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getRidgedNoiseValue(glm::tvec4<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += (((FXX)1.0 - glm::abs(hm::Fractal::genSimplex(coords * frequency))) * (FXX)2.0 - (FXX)1.0) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}

template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getBillowNoiseValue(glm::tvec2<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += (glm::abs(hm::Fractal::genSimplex(coords * frequency)) * (FXX)2.0 - (FXX)1.0) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getBillowNoiseValue(glm::tvec3<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += (glm::abs(hm::Fractal::genSimplex(coords * frequency)) * (FXX)2.0 - (FXX)1.0) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getBillowNoiseValue(glm::tvec4<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += (glm::abs(hm::Fractal::genSimplex(coords * frequency)) * (FXX)2.0 - (FXX)1.0) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}

template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getAbsNoiseValue(glm::tvec2<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += glm::abs(hm::Fractal::genSimplex(coords * frequency)) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getAbsNoiseValue(glm::tvec3<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += glm::abs(hm::Fractal::genSimplex(coords * frequency)) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getAbsNoiseValue(glm::tvec4<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += glm::abs(hm::Fractal::genSimplex(coords * frequency)) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}

template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getCellularNoiseValue(glm::tvec2<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += hm::Fractal::genCellular(coords * frequency) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getCellularNoiseValue(glm::tvec3<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += hm::Fractal::genCellular(coords * frequency) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template<typename FXX, typename = std::enable_if_t<std::is_floating_point<FXX>::value>>
inline FXX getCellularNoiseValue(glm::tvec4<FXX, glm::highp> coords, hproc::Noise::NoiseData<FXX> data) {
    FXX total = (FXX)0.0;
    FXX frequency = scale;
    FXX amplitude = (FXX)1.0;

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    FXX maxAmplitude = (FXX)0.0;

    for (ui8 i = 0; i < octaves; ++i) {
        total += hm::Fractal::genCellular(coords * frequency) * amplitude;

        frequency *= (FXX)2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}

template<typename FXX, typename>
FXX hproc::Noise::getNoiseValue(glm::tvec2<FXX, glm::highp> coords, NoiseData<FXX> data, const FXX* start /*= nullptr*/) {
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
template<typename FXX, typename>
FXX hproc::Noise::getNoiseValue(glm::tvec3<FXX, glm::highp> coords, NoiseData<FXX> data, const FXX* start /*= nullptr*/) {
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
template<typename FXX, typename>
FXX hproc::Noise::getNoiseValue(glm::tvec4<FXX, glm::highp> coords, NoiseData<FXX> data, const FXX* start /*= nullptr*/) {
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
