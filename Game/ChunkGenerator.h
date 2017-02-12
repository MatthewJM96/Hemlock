#pragma once

#include <voxel\IChunkGenerator.hpp>

#include "Constants.hpp"

class ChunkGenerator : public hvox::IChunkGenerator<CHUNK_SIZE> {
public:
    void runGenTask(hvox::ChunkGenTask<CHUNK_SIZE> task);
};
