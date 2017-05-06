#include "stdafx.h"

#include "voxel/Chunk.h"

void hvox::Chunk::init(ui16 size, ChunkGridPosition chunkPosition) {
    m_size          = size;
    pos = chunkPosition;

    blocks = new Block[size * size * size];
    std::fill_n(blocks, size * size * size, Block{ false });

    neighbours = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
}

void hvox::Chunk::dispose() {
    delete[] blocks;
    blocks = nullptr;

    neighbours = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
}

void hvox::Chunk::setBlock(BlockChunkPosition blockPos, Block block) {
    blocks[getBlockIndex(blockPos, m_size)] = block;
    onBlockChange({ block.present ? BlockChange::PLACE : BlockChange::DESTROY, blockPos, pos });
}

void hvox::Chunk::setContiguousBlocks(BlockChunkPosition start, ui32 count, Block* blocks) {
    std::memcpy(&blocks[getBlockIndex(start, m_size)], blocks, count);
    onBulkBlockChange({ blocks, start, count, pos });
}
