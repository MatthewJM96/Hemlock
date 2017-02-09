#pragma once

#include "Chunk.h"
#include "ChunkCoordSystems.hpp"

namespace hemlock {
    namespace voxel {
        // TODO(Matthew): Let games specify LOD choices.
        enum class ChunkLOD {
            FULL,
            HALF,
            MIN
        };

        // TODO(Matthew): Let games specify generation type choices.
        enum class ChunkGenType {
            TERRAIN
        };

        template <int size>
        struct ChunkGenTask {
            ChunkLOD      lod;
            ChunkGenType  type;
            ChunkPosition pos;
            Chunk<size>*  chunk;
        };

        template <int size>
        class IChunkGenerator {
        public:
            virtual void runGenTask(ChunkGenTask<size> task) = 0;
        };
    }
}
namespace hvox = hemlock::voxel;
