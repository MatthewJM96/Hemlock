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
        };

        class ChunkGenerator {
        public:
            void runGenTask(ChunkGenTask task) {
                // TODO(Matthew): Implement a way to chain noise data options. Pass the chained noise data object into this task and then iterate through it - being sure to
                //                utilise caching to no longer do the noise calculation for a given point on the x-z plane multiple times.
                //                    Caching should be sufficient vs iterating over x-z plane first as the latter puts more complexity into setting up the chunk updates and
                //                    the cache should grow to at most 2MB in size.
                //                    TODO(Matthew): Figure out appropriate cache eviction methodology.
            }
        };
    }
}
namespace hvox = hemlock::voxel;
