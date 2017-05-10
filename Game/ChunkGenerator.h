#pragma once

#include <voxel/Chunk.h>
#include <voxel/ChunkGeneratorBase.h>

class ChunkGenerator : public hvox::ChunkGeneratorBase {
public:
    void runGenTask(hvox::ChunkGenTask task) override;
};
