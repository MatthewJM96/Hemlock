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
            ui16 x, y, z;
        };
        using QuadPosition = BlockChunkPosition;
        // Block position in rectilinear world-space.
        struct BlockRectilinearWorldPosition {
            i64 x, y, z;
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

        bool operator==(const ChunkID& a, const ChunkID& b);

        ui64 getBlockIndex(BlockChunkPosition pos, ui16 size = 32);

        BlockChunkPosition getBlockChunkPosition(ui64 blockIndex, ui16 size = 32);

        BlockRectilinearWorldPosition getRectilinearWorldPosition(ChunkGridPosition chunkPos, BlockChunkPosition blockPos = { 0, 0, 0 }, ui16 size = 32);
        BlockRectilinearWorldPosition getRectilinearWorldPosition(ChunkGridPosition chunkPos, ui64 blockIndex, ui16 size = 32);
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
}
