#pragma once

#include "Chunk.h"
#include "IChunkGenerator.hpp"
#include "ChunkMesher.h"

namespace hemlock {
    namespace voxel {
        class ChunkGrid {
            using GenTasks  = std::queue<ChunkGenTask>;
            using MeshTasks = std::queue<ChunkMeshTask>;
            using Chunks    = std::unordered_map<ChunkID, Chunk*>;
        public:
            ChunkGrid() :
                m_size(0), m_generator(nullptr), m_mesher(nullptr), m_genTasks(GenTasks()), m_meshTasks(MeshTasks())
            { /* Empty */ }

			void init(ui16 size, IChunkGenerator* generator, ChunkMesher* mesher);
			void dispose();

			void submitGenTask(ChunkLOD lod, ChunkGenType type, ChunkRectilinearWorldPosition pos);

			void update();

			void handleBlockChange(Sender sender, BlockChangeEvent event);
			void handleBulkBlockChange(Sender sender, BulkBlockChangeEvent event);
        private:
			ui16 m_size = 0;

            IChunkGenerator* m_generator = nullptr;
			ChunkMesher*	 m_mesher = nullptr;

            GenTasks  m_genTasks;
			MeshTasks m_meshTasks;

            Chunks m_chunks;
        };
    }
}
namespace hvox = hemlock::voxel;
