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
                m_size(0), m_generator(nullptr), m_mesher(nullptr)
            { /* Empty */ }

			void init(ui16 size, IChunkGenerator* generator, ChunkMesher* mesher);
			void dispose();

			void submitGenTask(ChunkLOD lod, ChunkGenType type, ChunkRectilinearWorldPosition pos);

			void update();

			void handleBlockChange(Sender sender, BlockChangeEvent event);
			void handleBulkBlockChange(Sender sender, BulkBlockChangeEvent event);
        private:
            Chunk* createChunk(ChunkRectilinearWorldPosition pos);
            void establishChunkNeighbours(Chunk* chunk, ChunkRectilinearWorldPosition pos);

			ui16 m_size;

            IChunkGenerator* m_generator;
			ChunkMesher*	 m_mesher;

            GenTasks  m_genTasks;
			MeshTasks m_meshTasks;

            Chunks m_chunks;
        };
    }
}
namespace hvox = hemlock::voxel;
