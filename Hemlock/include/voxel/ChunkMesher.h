#pragma once

#include "voxel/ChunkMesh.hpp"
#include "graphics/Mesh.hpp"

namespace hemlock {
    namespace voxel {        
        struct ChunkMeshTask {
            Chunk*  chunk;
        };

        class ChunkMesher {
        public:
            void runMeshTask(ChunkMeshTask task);
        };
    }
}
namespace hvox = hemlock::voxel;
