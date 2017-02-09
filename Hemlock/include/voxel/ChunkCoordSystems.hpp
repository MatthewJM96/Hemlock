#pragma once

#include "Types.h"

namespace hemlock {
    namespace voxel {
        // Discrete coordinate system of blocks in a chunk.
        struct BlockPosition {
            //BlockPosition() {}
            //BlockPosition(ui32 _x, ui32 _y, ui32 _z) : x(_x), y(_y), z(_z) {}
            ui32 x, y, z;
        };

        // Discrete coordinate system of chunks.
        struct ChunkPosition {
            //ChunkPosition() {}
            //ChunkPosition(i32 _x, i32 _y, i32 _z) : x(_x), y(_y), z(_z) {}
            i32 x, y, z;
        };

        struct WorldPosition {
            //WorldPosition() {}
            //WorldPosition(i64 _x, i64 _y, i64 _z) : x(_x), y(_y), z(_z) {}
            i64 x, y, z;
        };

        template <int size = 32>
        glm::i64vec3 getWorldPosition(ChunkPosition chunkPos, BlockPosition blockPos /*= { glm::u32vec3(0) }*/) {
            return glm::i64vec3(chunkPos.x * size + (i32)blockPos.x, chunkPos.y * size + (i32)blockPos.y, chunkPos.z * size + (i32)blockPos.z);
        }
    }
}
namespace hvox = hemlock::voxel;
