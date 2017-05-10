#include "stdafx.h"

#include "voxel/ChunkCoordSystems.h"

// TODO(Matthew): Not just here, but especially so here, we shouldn't need so many casts.

bool hvox::operator==(const ChunkID& a, const ChunkID& b) {
    return (a.id == b.id);
}

ui64 hvox::getBlockIndex(BlockChunkPosition pos) {
    return (ui64)pos.x + (ui64)pos.y * CHUNK_SIZE + (ui64)pos.z * CHUNK_SIZE * CHUNK_SIZE;
}

hvox::BlockChunkPosition hvox::getBlockChunkPosition(ui64 blockIndex) {
    ui16 z = (ui16)glm::floor((f64)blockIndex / (f64)(CHUNK_SIZE * CHUNK_SIZE));
    blockIndex %= (CHUNK_SIZE * CHUNK_SIZE);
    ui16 y = (ui16)glm::floor((f64)blockIndex / (f64)CHUNK_SIZE);
    ui16 x = (ui16)(blockIndex % CHUNK_SIZE);
    return { x, y, z };
}

hvox::BlockRectilinearWorldPosition hvox::getRectilinearWorldPosition(ChunkGridPosition chunkPos, BlockChunkPosition blockPos /*= { 0, 0, 0 }*/) {
    return { (i64)chunkPos.x * (i64)CHUNK_SIZE + (i64)blockPos.x, (i64)chunkPos.y * (i64)CHUNK_SIZE + (i64)blockPos.y, (i64)chunkPos.z * (i64)CHUNK_SIZE + (i64)blockPos.z };
}
hvox::BlockRectilinearWorldPosition hvox::getRectilinearWorldPosition(ChunkGridPosition chunkPos, ui64 blockIndex) {
    return getRectilinearWorldPosition(chunkPos, getBlockChunkPosition(blockIndex));
}