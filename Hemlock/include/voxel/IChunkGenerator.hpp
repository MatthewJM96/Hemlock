#pragma once

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

        struct ChunkGenTask {
            ChunkLOD      lod;
            ChunkGenType  type;
			Chunk*        chunk;
			union {
				ChunkID						  id;
				ChunkRectilinearWorldPosition pos;
			};
        };

        class IChunkGenerator {
        public:
            virtual void runGenTask(ChunkGenTask task, ui16 size) = 0;
        };
    }
}
namespace hvox = hemlock::voxel;
