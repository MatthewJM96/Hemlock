#pragma once

#include "Chunk.h"
#include "ChunkGenerator.h"
#include "ChunkMesher.h"

namespace hemlock {
    namespace voxel {
        class ChunkGrid {
            using GenTasks  = std::queue<ChunkGenTask>;
            using MeshTasks = std::queue<ChunkMeshTask>;
            using Chunks    = std::unordered_map<ChunkID, Chunk*>;
        public:
            ChunkGrid() :
                m_generator(nullptr), m_mesher(nullptr)
            { /* Empty */ }

            void init(ChunkGenerator* generator, ChunkMesher* mesher);
            void dispose();

            void submitGenTask(ChunkLOD lod, ChunkGenType type, ChunkGridPosition pos, Heightmapper heightmapper);

            void update();

            void handleBlockChange(Sender sender, BlockChangeEvent event);
            void handleBulkBlockChange(Sender sender, BulkBlockChangeEvent event);

            const Chunks& getChunks() { return m_chunks; }
        private:
            Chunk* createChunk(ChunkGridPosition pos);
            void establishChunkNeighbours(Chunk* chunk, ChunkGridPosition pos);
            
            ChunkGenerator* m_generator;
            ChunkMesher*     m_mesher;

            GenTasks  m_genTasks;
            MeshTasks m_meshTasks;

            Chunks m_chunks;
        };
    }
}
namespace hvox = hemlock::voxel;
