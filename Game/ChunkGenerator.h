#pragma once

#include <voxel/Chunk.h>
#include <voxel/IChunkGenerator.hpp>

class ChunkGenerator : public hvox::IChunkGenerator {
public:
    void runGenTask(hvox::ChunkGenTask task) override;
};
