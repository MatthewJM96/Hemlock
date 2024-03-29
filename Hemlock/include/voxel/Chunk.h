#pragma once

#include "voxel/ChunkMesh.hpp"
#include "voxel/ChunkCoordSystems.hpp"

namespace hemlock {
    namespace voxel {
        struct Block { // TODO(Matthew): Move elsewhere as this will get big.
            bool present;
        };

        enum class BlockChange {
            PLACE,
            DESTROY
        };
        struct BlockChangeEvent {
            BlockChange change;
            BlockChunkPosition blockPos;
            ChunkGridPosition chunkPos;
            // Other info like block type etc.
        };

        struct BulkBlockChangeEvent {
            const Block* blocks;
            BlockChunkPosition startPos;
            ui32 count;
            ChunkGridPosition chunkPos;
        };

        class Chunk {
        public:
            Chunk() :
                neighbours({ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }),
                blocks(nullptr), flags({ false, false })
            { /* EMPTY */ }

            void init(ChunkGridPosition chunkPosition);
            void dispose();
            
            // TODO(Matthew): Schedule chunk meshing task when blocks are set.
            void setBlock(BlockChunkPosition pos, Block block);
            void setContiguousBlocks(BlockChunkPosition start, ui32 count, Block* blocks);
            
            struct {
                Chunk* left;
                Chunk* right;
                Chunk* top;
                Chunk* bottom;
                Chunk* front;
                Chunk* back;
            } neighbours;

            Block* blocks; // TODO(Matthew): Compression via RLE?

            ChunkMesh mesh;

            ChunkGridPosition pos;

            Event<BlockChangeEvent>     onBlockChange     = Event<BlockChangeEvent>(this);
            Event<BulkBlockChangeEvent> onBulkBlockChange = Event<BulkBlockChangeEvent>(this);

            struct {
                bool genTaskActive : 1;
                bool hasMeshTask   : 1;
            } flags;
        };
    }
}
namespace hvox = hemlock::voxel;
