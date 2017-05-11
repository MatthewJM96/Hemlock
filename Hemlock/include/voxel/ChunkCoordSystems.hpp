#pragma once

namespace hemlock {
    namespace voxel {
        // Names verbose, but others may be necessary if we choose (or if games choose regardless) to support non-rectilinear topologies for chunkable spaces.

        /// Spaces:
        // World-space: any space in which the player interacts directly.
        //              -> contains a grid-space of chunks, which themselves contain a chunk-space of blocks.
        // Grid-space:  discrete space with units of chunk width (i.e. each chunk is at an integer coordinate).
        //              -> used to reference locations of chunks in the world relative to one another.
        // Chunk-space: discrete space with units of block width (i.e. each block is at an integer coordinate).
        //              -> used to reference locations of blocks in a chunk relative to one another.

        // Block position in chunk-space.
        struct BlockChunkPosition {
            ui8 x, y, z;
        };
        using QuadPosition = BlockChunkPosition;
        // Block position in rectilinear world-space.
        struct BlockRectilinearWorldPosition {
            i32 x, y, z;
        };

        // Column position in world-space.
        union ColumnRectilinearWorldPosition {
            struct {
                i32 x, z;
            };
            ui64 id;
        };

        // Chunk position in grid-space.
        union ChunkGridPosition {
            struct {
                i64 x : 24;
                i64 y : 16;
                i64 z : 24;
            };
            ui64 id;
        };
        using ChunkID = ChunkGridPosition;

        inline bool operator==(const ColumnRectilinearWorldPosition& a, const ColumnRectilinearWorldPosition& b) {
            return (a.id == b.id);
        }

        inline bool operator==(const ChunkID& a, const ChunkID& b) {
            return (a.id == b.id);
        }

        inline ui64 getBlockIndex(BlockChunkPosition pos) {
            return (ui64)pos.x + (ui64)pos.y * CHUNK_SIZE + (ui64)pos.z * CHUNK_SIZE * CHUNK_SIZE;
        }

        inline BlockChunkPosition getBlockChunkPosition(ui64 blockIndex) {
            ui8 z = (ui8)glm::floor((f64)blockIndex / (f64)(CHUNK_SIZE * CHUNK_SIZE));
            blockIndex %= (CHUNK_SIZE * CHUNK_SIZE);
            ui8 y = (ui8)glm::floor((f64)blockIndex / (f64)CHUNK_SIZE);
            ui8 x = (ui8)(blockIndex % CHUNK_SIZE);
            return { x, y, z };
        }

        inline BlockRectilinearWorldPosition getRectilinearWorldPosition(ChunkGridPosition chunkPos, BlockChunkPosition blockPos = { 0, 0, 0 }) {
            return { (i32)chunkPos.x * (i32)CHUNK_SIZE + (i32)blockPos.x, (i32)chunkPos.y * (i32)CHUNK_SIZE + (i32)blockPos.y, (i32)chunkPos.z * (i32)CHUNK_SIZE + (i32)blockPos.z };
        }
        inline BlockRectilinearWorldPosition getRectilinearWorldPosition(ChunkGridPosition chunkPos, ui64 blockIndex) {
            return getRectilinearWorldPosition(chunkPos, getBlockChunkPosition(blockIndex));
        }
    }
}
namespace hvox = hemlock::voxel;

namespace std {
    template<>
    struct hash<hvox::ChunkID> {
        std::size_t operator()(const hvox::ChunkID& id) const {
            std::hash<ui64> hash;
            return hash(id.id);
        }
    };

    template<>
    struct hash<hvox::ColumnRectilinearWorldPosition> {
        std::size_t operator()(const hvox::ColumnRectilinearWorldPosition& pos) const {
            std::hash<ui64> hash;
            return hash(pos.id);
        }
    };
}
