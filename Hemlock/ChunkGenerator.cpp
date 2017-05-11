#include "stdafx.h"

#include "voxel/Chunk.h"
#include "voxel/ChunkCoordSystems.hpp"

#include "voxel/ChunkGenerator.h"

void hvox::ChunkGenerator::runGenTask(ChunkGenTask task, GenCache& cache) {
    Chunk& chunk = *task.chunk;
    BlockRectilinearWorldPosition chunkPos = getRectilinearWorldPosition(chunk.pos, 0);
    // Iterate over the x-z plane, generating terrain for each point. (Right now just 2D noise).
    for (ui8 x = 0; x < CHUNK_SIZE; ++x) {
        for (ui8 z = 0; z < CHUNK_SIZE; ++z) {
            // Look for these x,z coords in cache.
            ColumnRectilinearWorldPosition columnPos = { chunkPos.x + x, chunkPos.z + z };
            auto it = cache.find(columnPos);

            // Get height for these x,z coords from cache first, from heightmapper second - adding to cache in this case.
            i32 yMax;
            if (it == cache.end()) {
                yMax = task.heightmapper(columnPos);
                cache[columnPos] = yMax;
            } else {
                yMax = it->second;
            }

            yMax -= chunkPos.y;

            // If the relative height is <= 0 then we can just go to the next column - nothing exists in it for this chunk.
            if (yMax <= 0) continue;

            // Set each block in this column up-to the relative height generated or the height of the chunk, whichever is smaller.
            for (ui8 y = 0; y < yMax && y < CHUNK_SIZE; ++y) {
                chunk.setBlock({ x, y, z }, { true });
            }
        }
    }
}