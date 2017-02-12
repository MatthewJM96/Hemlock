#pragma once

#include "Chunk.h"
#include "IChunkGenerator.hpp"
#include "ChunkMesher.h"

namespace hemlock {
    namespace voxel {
        class ChunkGrid {
        public:
			void init(ui16 size, IChunkGenerator* generator, ChunkMesher* mesher);
			void dispose();

			void submitGenTask(ChunkLOD lod, ChunkGenType type, ChunkRectilinearWorldPosition pos);

			void update();

			void handleBlockChange(Sender sender, BlockChangeEvent event);
			void handleBulkBlockChange(Sender sender, BulkBlockChangeEvent event);
        private:
			ui16 m_size;

            IChunkGenerator* m_generator;
			ChunkMesher*	 m_mesher;

            std::queue<ChunkGenTask>  m_genTasks;
			std::queue<ChunkMeshTask> m_meshTasks;

            std::unordered_map<ChunkID, Chunk*> m_chunks;
        };
    }
}
namespace hvox = hemlock::voxel;
