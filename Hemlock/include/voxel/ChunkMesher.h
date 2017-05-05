#pragma once

#include "voxel/ChunkMesh.hpp"
#include "graphics/Mesh.hpp"

namespace hemlock {
	namespace voxel {		
		struct ChunkMeshTask {
			union {
				ChunkID						  id;
				ChunkRectilinearWorldPosition pos;
			};
			Chunk*  chunk;
		};

		class ChunkMesher {
		public:
			void runMeshTask(ChunkMeshTask task, ui64 size);
		};
	}
}
namespace hvox = hemlock::voxel;
