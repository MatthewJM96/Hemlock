#include "stdafx.h"

#include "voxel/Chunk.h"
#include "voxel/ChunkMesher.h"
#include "voxel/ChunkCoordSystems.h"

// Won't need these once greedy merging is in, but for now let's just 
// figure out simple occlusion. As such don't worry about indexing for now.
static inline void addFrontQuad(hvox::BlockChunkPosition pos, hg::Vertex3D<f32>* vertexBuffer, ui64& vertexBufferSize) {
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
};
static inline void addBackQuad(hvox::BlockChunkPosition pos, hg::Vertex3D<f32>* vertexBuffer, ui64& vertexBufferSize) {
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
};
static inline void addLeftQuad(hvox::BlockChunkPosition pos, hg::Vertex3D<f32>* vertexBuffer, ui64& vertexBufferSize) {
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
};
static inline void addRightQuad(hvox::BlockChunkPosition pos, hg::Vertex3D<f32>* vertexBuffer, ui64& vertexBufferSize) {
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
};
static inline void addBottomQuad(hvox::BlockChunkPosition pos, hg::Vertex3D<f32>* vertexBuffer, ui64& vertexBufferSize) {
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x, -0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
};
static inline void addTopQuad(hvox::BlockChunkPosition pos, hg::Vertex3D<f32>* vertexBuffer, ui64& vertexBufferSize) {
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = {  0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x,  0.5f + pos.y,  0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    vertexBuffer[vertexBufferSize++] = { -0.5f + pos.x,  0.5f + pos.y, -0.5f + pos.z, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };
};

static inline bool isAtLeftFace(ui64 index) {
    return (index % CHUNK_SIZE) == 0;
}
static inline bool isAtRightFace(ui64 index) {
    return ((index + 1) % CHUNK_SIZE) == 0;
}
static inline bool isAtBottomFace(ui64 index) {
    return (index % (CHUNK_SIZE * CHUNK_SIZE)) < CHUNK_SIZE;
}
static inline bool isAtTopFace(ui64 index) {
    return (index % (CHUNK_SIZE * CHUNK_SIZE)) >= (CHUNK_SIZE * (CHUNK_SIZE - 1));
}
static inline bool isAtFrontFace(ui64 index) {
    return index < (CHUNK_SIZE * CHUNK_SIZE);
}
static inline bool isAtBackFace(ui64 index) {
    return index >= (CHUNK_SIZE * CHUNK_SIZE * (CHUNK_SIZE - 1));
}

static inline ui64 getIndexAtRightFace(ui64 index) {
    return index + CHUNK_SIZE - 1;
}
static inline ui64 getIndexAtLeftFace(ui64 index) {
    return index - CHUNK_SIZE + 1;
}
static inline ui64 getIndexAtTopFace(ui64 index) {
    return index + (CHUNK_SIZE * (CHUNK_SIZE - 1));
}
static inline ui64 getIndexAtBottomFace(ui64 index) {
    return index - (CHUNK_SIZE * (CHUNK_SIZE - 1));
}
static inline ui64 getIndexAtFrontFace(ui64 index) {
    return index - (CHUNK_SIZE * CHUNK_SIZE * (CHUNK_SIZE - 1));
}
static inline ui64 getIndexAtBackFace(ui64 index) {
    return index + (CHUNK_SIZE * CHUNK_SIZE * (CHUNK_SIZE - 1));
}

void hvox::ChunkMesher::runMeshTask(ChunkMeshTask task) {
    // TODO(Matthew): Make this more efficient...
    //                We should not be iterating over all blocks in a chunk.
    //                Octree would make this more efficient, as would simply 
    //                storing a vector of "occupied" blocks.
    Chunk&                        chunk    = *task.chunk;
    BlockRectilinearWorldPosition chunkPos = getRectilinearWorldPosition(chunk.pos, 0);

    // Not a fan of this: lot's of copying and still newing stuff.
    hg::Vertex3D<f32> chunkMesh[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6];
    ui64              meshSize  = 0;

    for (ui64 i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; ++i) {
        Block voxel = chunk.blocks[i];
        if (voxel.present) {
            BlockChunkPosition blockPos = getBlockChunkPosition(i);

            // Check its neighbours, to decide whether to add its quads.
            // LEFT
            if (isAtLeftFace(i)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtRightFace(i);
                if (chunk.neighbours.left == nullptr || !chunk.neighbours.left->blocks[j].present) {
                    addLeftQuad(blockPos, chunkMesh, meshSize);
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i - 1].present) {
                    addLeftQuad(blockPos, chunkMesh, meshSize);
                }
            }

            // RIGHT
            if (isAtRightFace(i)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtLeftFace(i);
                if (chunk.neighbours.right == nullptr || !chunk.neighbours.right->blocks[j].present) {
                    addRightQuad(blockPos, chunkMesh, meshSize);
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i + 1].present) {
                    addRightQuad(blockPos, chunkMesh, meshSize);
                }
            }

            // BOTTOM
            if (isAtBottomFace(i)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtTopFace(i);
                if (chunk.neighbours.bottom == nullptr || !chunk.neighbours.bottom->blocks[j].present) {
                    addBottomQuad(blockPos, chunkMesh, meshSize);
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i - CHUNK_SIZE].present) {
                    addBottomQuad(blockPos, chunkMesh, meshSize);
                }
            }

            // TOP
            if (isAtTopFace(i)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtBottomFace(i);
                if (chunk.neighbours.top == nullptr || !chunk.neighbours.top->blocks[j].present) {
                    addTopQuad(blockPos, chunkMesh, meshSize);
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i + CHUNK_SIZE].present) {
                    addTopQuad(blockPos, chunkMesh, meshSize);
                }
            }

            // FRONT
            if (isAtFrontFace(i)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtBackFace(i);
                if (chunk.neighbours.front == nullptr || !chunk.neighbours.front->blocks[j].present) {
                    addFrontQuad(blockPos, chunkMesh, meshSize);
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i - (CHUNK_SIZE * CHUNK_SIZE)].present) {
                    addFrontQuad(blockPos, chunkMesh, meshSize);
                }
            }

            // BACK
            if (isAtBackFace(i)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtFrontFace(i);
                if (chunk.neighbours.back == nullptr || !chunk.neighbours.back->blocks[j].present) {
                    addBackQuad(blockPos, chunkMesh, meshSize);
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i + (CHUNK_SIZE * CHUNK_SIZE)].present) {
                    addBackQuad(blockPos, chunkMesh, meshSize);
                }
            }
        }
    }
    hg::MeshData3D<f32> mesh{};
    mesh.vertexCount = meshSize;
    mesh.vertices	 = chunkMesh;

    glm::f32mat4 translationMatrix = glm::translate(glm::f32mat4(), glm::f32vec3(chunkPos.x, chunkPos.y, chunkPos.z));

    chunk.mesh = { hg::createVAO(mesh), meshSize, translationMatrix };
}
