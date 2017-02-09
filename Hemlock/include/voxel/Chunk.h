#pragma once

#include "ChunkCoordSystems.hpp"

namespace hemlock {
    namespace voxel {
        struct Block {
            bool present;
        };

        template <int size = 32>
        class Chunk {
            friend class ChunkMesher;
        public:
            void setBlock(BlockPosition pos, Block block) {
                m_blocks[pos.x + pos.y * size + pos.z * size * size] = block;
            }
            void setContiguousBlocks(BlockPosition start, ui32 count, Block* blocks) {
                std::memcpy(&m_blocks[start.x + start.y * size + start.z * size * size], blocks, count);
            }

            // TEMPORARY, DELETE!
            Block* getBlocks() { return &m_blocks[0]; }
        private:
            ChunkPosition m_chunkPosition;

            Block m_blocks[size * size * size]; // TODO(Matthew): Compression via RLE?
        };
    }
}
namespace hvox = hemlock::voxel;
