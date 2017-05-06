#include "stdafx.h"

#include "voxel/Chunk.h"
#include "voxel/ChunkMesher.h"

static inline bool isAtLeftFace(ui64 index, ui64 size) {
    return (index % size) == 0;
}
static inline bool isAtRightFace(ui64 index, ui64 size) {
    return ((index + 1) % size) == 0;
}
static inline bool isAtBottomFace(ui64 index, ui64 size) {
    return (index % (size * size)) < size;
}
static inline bool isAtTopFace(ui64 index, ui64 size) {
    return (index % (size * size)) >= (size * (size - 1));
}
static inline bool isAtFrontFace(ui64 index, ui64 size) {
    return index < (size * size);
}
static inline bool isAtBackFace(ui64 index, ui64 size) {
    return index >= (size * size * (size - 1));
}

static inline ui64 getIndexAtRightFace(ui64 index, ui64 size) {
    return index + size - 1;
}
static inline ui64 getIndexAtLeftFace(ui64 index, ui64 size) {
    return index - size + 1;
}
static inline ui64 getIndexAtTopFace(ui64 index, ui64 size) {
    return index + (size * (size - 1));
}
static inline ui64 getIndexAtBottomFace(ui64 index, ui64 size) {
    return index - (size * (size - 1));
}
static inline ui64 getIndexAtFrontFace(ui64 index, ui64 size) {
    return index - (size * size * (size - 1));
}
static inline ui64 getIndexAtBackFace(ui64 index, ui64 size) {
    return index + (size * size * (size - 1));
}

void hvox::ChunkMesher::runMeshTask(ChunkMeshTask task, ui64 size) {
    // TODO(Matthew): Make this more efficient...
    //                We should not be iterating over all blocks in a chunk.
    //                Octree would make this more efficient, as would simply 
    //                storing a vector of "occupied" blocks.
    Chunk& chunk = *task.chunk;
    for (ui64 i = 0; i < size * size * size; ++i) {
        Block voxel = chunk.blocks[i];
        if (voxel.present) {
            BlockRectilinearWorldPosition blockPos = getRectilinearWorldPosition(chunk.pos, i, size);
            glm::f32mat4 translationMatrix = glm::translate(glm::f32mat4(), glm::f32vec3(blockPos.x, blockPos.y, blockPos.z));

            // Check its neighbours, to decide whether to add its quads.
            // LEFT
            if (isAtLeftFace(i, size)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtRightFace(i, size);
                if (chunk.neighbours.left == nullptr || !chunk.neighbours.left->blocks[j].present) {
                    chunk.mesh.quads.push_back({ Face::LEFT, translationMatrix });
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i - 1].present) {
                    chunk.mesh.quads.push_back({ Face::LEFT, translationMatrix });
                }
            }

            // RIGHT
            if (isAtRightFace(i, size)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtLeftFace(i, size);
                if (chunk.neighbours.right == nullptr || !chunk.neighbours.right->blocks[j].present) {
                    chunk.mesh.quads.push_back({ Face::RIGHT, translationMatrix });
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i + 1].present) {
                    chunk.mesh.quads.push_back({ Face::RIGHT, translationMatrix });
                }
            }

            // BOTTOM
            if (isAtBottomFace(i, size)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtTopFace(i, size);
                if (chunk.neighbours.bottom == nullptr || !chunk.neighbours.bottom->blocks[j].present) {
                    chunk.mesh.quads.push_back({ Face::BOTTOM, translationMatrix });
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i - size].present) {
                    chunk.mesh.quads.push_back({ Face::BOTTOM, translationMatrix });
                }
            }

            // TOP
            if (isAtTopFace(i, size)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtBottomFace(i, size);
                if (chunk.neighbours.top == nullptr || !chunk.neighbours.top->blocks[j].present) {
                    chunk.mesh.quads.push_back({ Face::TOP, translationMatrix });
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i + size].present) {
                    chunk.mesh.quads.push_back({ Face::TOP, translationMatrix });
                }
            }

            // FRONT
            if (isAtFrontFace(i, size)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtBackFace(i, size);
                if (chunk.neighbours.front == nullptr || !chunk.neighbours.front->blocks[j].present) {
                    chunk.mesh.quads.push_back({ Face::FRONT, translationMatrix });
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i - (size * size)].present) {
                    chunk.mesh.quads.push_back({ Face::FRONT, translationMatrix });
                }
            }

            // BACK
            if (isAtBackFace(i, size)) {
                // Get corresponding neighbour index in neighbour chunk and check.
                ui64 j = getIndexAtFrontFace(i, size);
                if (chunk.neighbours.back == nullptr || !chunk.neighbours.back->blocks[j].present) {
                    chunk.mesh.quads.push_back({ Face::BACK, translationMatrix });
                }
            } else {
                // Get corresponding neighbour index in this chunk and check.
                if (!chunk.blocks[i + (size * size)].present) {
                    chunk.mesh.quads.push_back({ Face::BACK, translationMatrix });
                }
            }
        }
    }
}
