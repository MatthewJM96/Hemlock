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

        using Heightmapper = Delegate<i32, ColumnRectilinearWorldPosition>;

        struct ChunkGenTask {
            ChunkLOD     lod;
            ChunkGenType type;
            Chunk*       chunk;
            Heightmapper heightmapper;
        };

        using GenCache = std::unordered_map<ColumnRectilinearWorldPosition, i32>;

        class ChunkGenerator {
        public:
            void runGenTask(ChunkGenTask task, GenCache& cache);
        };
    }
}
namespace hvox = hemlock::voxel;
