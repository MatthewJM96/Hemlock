#include "stdafx.h"

#include "voxel/Chunk.h"

void hvox::Chunk::init(ChunkGridPosition chunkPosition) {
    pos = chunkPosition;

    blocks = new Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
    std::fill_n(blocks, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, Block{ false });

    neighbours = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
}

void hvox::Chunk::dispose() {
    delete[] blocks;
    blocks = nullptr;

    neighbours = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
}

void hvox::Chunk::setBlock(BlockChunkPosition blockPos, Block block) {
    blocks[getBlockIndex(blockPos)] = block;
    if (!flags.genTaskActive) {
        onBlockChange({ block.present ? BlockChange::PLACE : BlockChange::DESTROY, blockPos, pos });
    }
}

void hvox::Chunk::setContiguousBlocks(BlockChunkPosition start, ui32 count, Block* blocks) {
    std::memcpy(&blocks[getBlockIndex(start)], blocks, count);
    if (!flags.genTaskActive) {
        onBulkBlockChange({ blocks, start, count, pos });
    }
}
