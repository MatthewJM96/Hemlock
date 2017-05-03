#include "stdafx.h"

#include "ChunkGenerator.h"

#include <procedural/Noise.hpp>

void ChunkGenerator::runGenTask(hvox::ChunkGenTask task, ui16 size) {
    hvox::Chunk* chunk = task.chunk;

    hproc::Noise::NoiseData<f64> smallDetails;
    smallDetails.type = hproc::Noise::Type::RIDGED;
    smallDetails.octaves = 8;
    smallDetails.persistence = 0.85;
    smallDetails.frequency = 0.05;
    smallDetails.modifier = 0.0;
    smallDetails.op = hproc::Noise::Operation::ADD;
    smallDetails.multiplier = hproc::Noise::Multiplier::NONE;
    smallDetails.bound = { -2.0, 7.0 };
    smallDetails.clamp = { 0.0,  0.0 };

    hproc::Noise::NoiseData<f64> medDetails;
    medDetails.type = hproc::Noise::Type::RIDGED;
    medDetails.octaves = 6;
    medDetails.persistence = 0.8;
    medDetails.frequency = 0.0025;
    medDetails.modifier = 1.0;
    medDetails.op = hproc::Noise::Operation::ADD;
    medDetails.multiplier = hproc::Noise::Multiplier::NONE;
    medDetails.bound = { -50.0, 50.0 };
    medDetails.clamp = { 0.0,  0.0 };

    hproc::Noise::NoiseData<f64> bigDetails;
    bigDetails.type = hproc::Noise::Type::RIDGED;
    bigDetails.octaves = 6;
    bigDetails.persistence = 0.7;
    bigDetails.frequency = 0.0005;
    bigDetails.modifier = 1.0;
    bigDetails.op = hproc::Noise::Operation::ADD;
    bigDetails.multiplier = hproc::Noise::Multiplier::NONE;
    bigDetails.bound = { 0.0, 200.0 };
    bigDetails.clamp = { 0.0,  0.0 };

    for (i64 x = 0; x < size; ++x) {
        for (i64 z = 0; z < size; ++z) {
			glm::f64vec2 pos = { task.pos.x + x, task.pos.z + z };
            i64 y = (i64)glm::floor(hproc::Noise::getNoiseValue(pos, smallDetails) + hproc::Noise::getNoiseValue(pos, medDetails) + hproc::Noise::getNoiseValue(pos, bigDetails));
            y -= task.pos.y;
            if (y < 0) continue;
            for (i64 yP = 0; yP < z; ++yP) {
                hvox::BlockChunkPosition blockPos = { (ui16)x, (ui16)yP, (ui16)z };
                chunk->setBlock(blockPos, { true });
            }
        }
    }
}
