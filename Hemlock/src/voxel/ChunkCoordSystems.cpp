#include "stdafx.h"

#include "voxel\ChunkCoordSystems.h"

bool hvox::operator==(const ChunkID& a, const ChunkID& b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

ui64 hvox::getBlockIndex(BlockChunkPosition pos, ui16 size /*= 32*/) {
    return (ui64)pos.x + (ui64)pos.y *  (ui64)size + (ui64)pos.z * (ui64)size *  (ui64)size;
}

hvox::BlockChunkPosition hvox::getBlockChunkPosition(ui64 blockIndex, ui16 size /*= 32*/) {
    ui16 z = (ui16)glm::floor(blockIndex / ((ui64)size * (ui64)size));
    blockIndex %= ((ui64)size * (ui64)size);
    ui16 y = (ui16)glm::floor(blockIndex / (ui64)size);
    ui16 x = (ui16)(blockIndex % (ui64)size);
    return { x, y, z };
}

hvox::BlockRectilinearWorldPosition hvox::getRectilinearWorldPosition(ChunkGridPosition chunkPos, BlockChunkPosition blockPos /*= { 0, 0, 0 }*/, ui16 size /*= 32*/) {
    return { (i64)chunkPos.x * (i64)size + (i64)blockPos.x, (i64)chunkPos.y * (i64)size + (i64)blockPos.y, (i64)chunkPos.z * (i64)size + (i64)blockPos.z };
}
hvox::BlockRectilinearWorldPosition hvox::getRectilinearWorldPosition(ChunkGridPosition chunkPos, ui64 blockIndex /*= 0*/, ui16 size /*= 32*/) {
    return getRectilinearWorldPosition(chunkPos, getBlockChunkPosition(blockIndex));
}