#include "stdafx.h"

#include "ChunkGenerator.h"

#include <glm\glm.hpp>

#include <procedural\Noise.hpp>

#include <voxel\Chunk.h>
#include <voxel\ChunkCoordSystems.hpp>

void ChunkGenerator::runGenTask(hvox::ChunkGenTask<CHUNK_SIZE> task) {
    hvox::Chunk<CHUNK_SIZE>* chunk = task.chunk;
    hvox::BlockPosition blockPos{};
    glm::i64vec3 worldPos = hvox::getWorldPosition<CHUNK_SIZE>(task.pos, blockPos);

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

    for (ui32 x = 0; x < CHUNK_SIZE; ++x) {
        for (ui32 y = 0; y < CHUNK_SIZE; ++y) {
            glm::f64vec2 pos = glm::f64vec2(worldPos.x + x, worldPos.y + y);
            i64 z = (i64)glm::floor(hproc::Noise::getNoiseValue(pos, smallDetails) + hproc::Noise::getNoiseValue(pos, medDetails) + hproc::Noise::getNoiseValue(pos, bigDetails));
            z -= worldPos.z;
            if (z < 0) continue;
            if (z > CHUNK_SIZE) z = CHUNK_SIZE;
            for (i64 zP = 0; zP < z; ++zP) {
                hvox::BlockPosition blockPos = { x, y, zP };
                chunk->setBlock(blockPos, { true });
            }
        }
    }
}