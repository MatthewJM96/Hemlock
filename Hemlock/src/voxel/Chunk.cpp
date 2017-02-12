#include "stdafx.h"

#include "voxel\Chunk.h"

void hvox::Chunk::init(ui16 size) {
	m_size = size;
	blocks = new Block[size * size * size];
}

void hvox::Chunk::dispose() {
	delete[] blocks;
	blocks = nullptr;

	neighbours = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
}

void hvox::Chunk::setBlock(BlockChunkPosition pos, Block block) {
	blocks[getBlockIndex(pos, m_size)] = block;
	onBlockChange({ block.present ? BlockChange::PLACE : BlockChange::DESTROY, pos, m_chunkPosition });
}

void hvox::Chunk::setContiguousBlocks(BlockChunkPosition start, ui32 count, Block* blocks) {
	std::memcpy(&blocks[getBlockIndex(start, m_size)], blocks, count);
	onBulkBlockChange({ blocks, start, count, m_chunkPosition });
}
