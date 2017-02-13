#pragma once

#include <voxel\IChunkGenerator.hpp>

class ChunkGenerator : public hvox::IChunkGenerator {
public:
    void runGenTask(hvox::ChunkGenTask task, ui16 size);
};
