#pragma once

#include "voxel/ChunkMesh.hpp"
#include "voxel/ChunkCoordSystems.h"

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
			ChunkRectilinearWorldPosition chunkPos;
			// Other info like block type etc.
		};

		struct BulkBlockChangeEvent {
			const Block* blocks;
			BlockChunkPosition startPos;
			ui32 count;
			ChunkRectilinearWorldPosition chunkPos;
		};

        class Chunk {
		public:
			void init(ui16 size);
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

			Event<BlockChangeEvent>		onBlockChange	  = Event<BlockChangeEvent>(this);
			Event<BulkBlockChangeEvent> onBulkBlockChange = Event<BulkBlockChangeEvent>(this);
        private:
			ChunkRectilinearWorldPosition m_chunkPosition;

			ui16 m_size;
		};
    }
}
namespace hvox = hemlock::voxel;
