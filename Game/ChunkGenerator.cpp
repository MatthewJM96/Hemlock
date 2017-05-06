#include "stdafx.h"

#include "ChunkGenerator.h"

#include <procedural/Noise.hpp>

void ChunkGenerator::runGenTask(hvox::ChunkGenTask task, ui16 size) {

    // Set up noise data for small, medium and large-scale terrain details.
    hproc::Noise::NoiseData<f64> smallDetails;
    smallDetails.type = hproc::Noise::Type::ABS;
    smallDetails.octaves = 8;
    smallDetails.persistence = 0.85;
    smallDetails.frequency = 0.05;
    smallDetails.modifier = 0.0;
    smallDetails.op = hproc::Noise::Operation::ADD;
    smallDetails.multiplier = hproc::Noise::Multiplier::NONE;
    smallDetails.bound = { -2.0, 7.0 };
    smallDetails.clamp = { 0.0,  0.0 };

    hproc::Noise::NoiseData<f64> medDetails;
    medDetails.type = hproc::Noise::Type::CELLULAR_CUBIC;
    medDetails.octaves = 6;
    medDetails.persistence = 0.8;
    medDetails.frequency = 0.0025;
    medDetails.modifier = 1.0;
    medDetails.op = hproc::Noise::Operation::ADD;
    medDetails.multiplier = hproc::Noise::Multiplier::NONE;
    medDetails.bound = { -10.0, 10.0 };
    medDetails.clamp = { 0.0,  0.0 };

    hproc::Noise::NoiseData<f64> medDetails2;
    medDetails2.type = hproc::Noise::Type::RIDGED;
    medDetails2.octaves = 6;
    medDetails2.persistence = 0.8;
    medDetails2.frequency = 0.0025;
    medDetails2.modifier = 1.0;
    medDetails2.op = hproc::Noise::Operation::ADD;
    medDetails2.multiplier = hproc::Noise::Multiplier::NONE;
    medDetails2.bound = { -20.0, 10.0 };
    medDetails2.clamp = { 0.0,  0.0 };

    hproc::Noise::NoiseData<f64> bigDetails;
    bigDetails.type = hproc::Noise::Type::RIDGED;
    bigDetails.octaves = 6;
    bigDetails.persistence = 0.7;
    bigDetails.frequency = 0.0005;
    bigDetails.modifier = 1.0;
    bigDetails.op = hproc::Noise::Operation::ADD;
    bigDetails.multiplier = hproc::Noise::Multiplier::NONE;
    bigDetails.bound = { -30.0, 250.0 };
    bigDetails.clamp = { 0.0,  0.0 };

    // TODO(Matthew): 3D noise for overhangs etc.
    // TODO(Matthew): Multi-stage generation for heightmap, ore, water bodies (?), caves, etc.

    hvox::Chunk& chunk = *task.chunk;
    hvox::BlockRectilinearWorldPosition chunkBlockPos = hvox::getRectilinearWorldPosition(chunk.pos, 0, size);
    // Iterate over the x-z plane, generating terrain for each point. (Right now just 2D noise).
    for (i64 x = 0; x < size; ++x) {
        for (i64 z = 0; z < size; ++z) {
            // Get position of column of chunk.
			glm::f64vec2 pos = { chunkBlockPos.x + x, chunkBlockPos.z + z };
            // Calculate height of this column, taking off y-position of the base of this chunk to get relative height.
            i64 yMax = (i64)glm::floor(hproc::Noise::getNoiseValue(pos, smallDetails) + hproc::Noise::getNoiseValue(pos, medDetails) + hproc::Noise::getNoiseValue(pos, medDetails2) + hproc::Noise::getNoiseValue(pos, bigDetails));
            yMax -= chunkBlockPos.y;
            // If the relative height is <= 0 then we can just go to the next column - nothing exists in it for this chunk.
            if (yMax <= 0) continue;
            // Set each block in this column up-to the relative height generated or the height of the chunk, whichever is smaller.
            for (i64 y = 0; y < yMax && y < size; ++y) {
                chunk.setBlock({ (ui16)x, (ui16)y, (ui16)z }, { true });
            }
        }
    }
}
