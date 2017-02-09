#pragma once

#include "Chunk.h"
#include "IChunkGenerator.h"

namespace hemlock {
    namespace voxel {
        template <int size = 32>
        class ChunkGrid {
        public:
            void init(IChunkGenerator<size>* generator) {
                m_generator = generator;
            }
            void dispose() {
                // Note generator should be cleaned up by initialising code.
                std::queue<ChunkGenTask<size>>().swap(m_genTasks);
                std::unordered_map<ChunkPosition, Chunk<size>*>.swap(m_chunks);
            }

            void submitGenTask(ChunkLOD lod, ChunkGenType type, ChunkPosition pos) {
                char* hash = new char[256];
                snprintf(hash, 256, "%d|%d|%d", pos.x, pos.y, pos.z);
                auto& it = m_chunks.find(/*pos*/std::string(hash));
                Chunk<size>* chunk = nullptr;
                if (it == m_chunks.end()) {
                    chunk = new Chunk<size>();
                    m_chunks[/*pos*/std::string(hash)] = chunk;
                } else {
                    chunk = (*it).second;
                }

                m_genTasks.push({ lod, type, pos, chunk });
            }

            void update() {
#define MAX_QUERIES 1000
                for (ui32 i = 0; i < MAX_QUERIES; ++i) {
                    if (m_genTasks.empty()) break;

                    m_generator->runGenTask(m_genTasks.front());

                    m_genTasks.pop();
                }
#undef MAX_QUERIES
            }

            // TEMPORARY, DELETE!
            std::unordered_map<std::string, Chunk<size>*> getChunks() { return m_chunks; }
        private:
            IChunkGenerator<size>* m_generator;

            std::queue<ChunkGenTask<size>> m_genTasks;

            // TODO(Matthew): Implement hashing function for ChunkPosition.
            std::unordered_map<std::string, Chunk<size>*> m_chunks;
        };
    }
}
namespace hvox = hemlock::voxel;
