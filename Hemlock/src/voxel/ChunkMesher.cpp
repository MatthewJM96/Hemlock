#include "stdafx.h"

#include "voxel\Chunk.h"
#include "voxel\ChunkMesher.h"

void hvox::ChunkMesher::init() {
	hg::MeshData3D<f32> vData = {};
	vData.vertexCount = 6;
	vData.indices = nullptr;

	vData.vertices = FRONT_QUAD;
	m_frontVAO = hg::createVAO(vData);

	vData.vertices = BACK_QUAD;
	m_backVAO = hg::createVAO(vData);

	vData.vertices = LEFT_QUAD;
	m_leftVAO = hg::createVAO(vData);

	vData.vertices = RIGHT_QUAD;
	m_rightVAO = hg::createVAO(vData);

	vData.vertices = BOTTOM_QUAD;
	m_bottomVAO = hg::createVAO(vData);

	vData.vertices = TOP_QUAD;
	m_topVAO = hg::createVAO(vData);
}

static inline bool isAtLeftFace(ui64 index, ui16 size) {
	return (index % size) == 0;
}
static inline bool isAtRightFace(ui64 index, ui16 size) {
	return ((index + 1) % size) == 0;
}
static inline bool isAtFrontFace(ui64 index, ui16 size) {
	return index < (size * size);
}
static inline bool isAtBackFace(ui64 index, ui16 size) {
	return index >= (size * size * (size - 1));
}
static inline bool isAtTopFace(ui64 index, ui16 size) {
	return (index % (size * size)) >= (size * (size - 1));
}
static inline bool isAtBottomFace(ui64 index, ui16 size) {
	return (index % (size * size)) < size;
}

void hvox::ChunkMesher::runMeshTask(ChunkMeshTask task, ui16 size) {
	// TODO(Matthew): Make this more efficient...
	//                We should not be iterating over all blocks in a chunk.
	//                Octree would make this more efficient, as would simply 
	//                storing a vector of "occupied" blocks.
	for (ui64 i = 0; i < size * size * size; ++i) {
		Block voxel = task.chunk->blocks[i];
		if (voxel.present) {
			glm::f32mat4 translationMatrix = glm::translate(glm::f32mat4(), glm::f32vec3(task.pos.x, task.pos.y, task.pos.z));

			// Check its neighbours, to decide whether to add its quads.
			// LEFT
			if (isAtLeftFace(i, size)) {
				// Get corresponding neighbour index in neighbour chunk and check.
				ui64 j = i + size - 1;
				if (!task.chunk->neighbours.left->blocks[j].present) {
					task.chunk->mesh.quads.push_back({ Face::LEFT, translationMatrix });
				}
			} else {
				// Get corresponding neighbour index in this chunk and check.
				if (!task.chunk->blocks[i - 1].present) {
					task.chunk->mesh.quads.push_back({ Face::LEFT, translationMatrix });
				}
			}

			// RIGHT
			if (isAtRightFace(i, size)) {
				// Get corresponding neighbour index in neighbour chunk and check.
				ui64 j = i - size + 1;
				if (!task.chunk->neighbours.right->blocks[j].present) {
					task.chunk->mesh.quads.push_back({ Face::RIGHT, translationMatrix });
				}
			} else {
				// Get corresponding neighbour index in this chunk and check.
				if (!task.chunk->blocks[i + 1].present) {
					task.chunk->mesh.quads.push_back({ Face::RIGHT, translationMatrix });
				}
			}

			// TOP
			if (isAtTopFace(i, size)) {
				// Get corresponding neighbour index in neighbour chunk and check.
				ui64 j = i - (size * (size - 1));
				if (!task.chunk->neighbours.right->blocks[j].present) {
					task.chunk->mesh.quads.push_back({ Face::TOP, translationMatrix });
				}
			} else {
				// Get corresponding neighbour index in this chunk and check.
				if (!task.chunk->blocks[i + size].present) {
					task.chunk->mesh.quads.push_back({ Face::TOP, translationMatrix });
				}
			}

			// BOTTOM
			if (isAtBottomFace(i, size)) {
				// Get corresponding neighbour index in neighbour chunk and check.
				ui64 j = i + (size * (size - 1));
				if (!task.chunk->neighbours.right->blocks[j].present) {
					task.chunk->mesh.quads.push_back({ Face::BOTTOM, translationMatrix });
				}
			} else {
				// Get corresponding neighbour index in this chunk and check.
				if (!task.chunk->blocks[i - size].present) {
					task.chunk->mesh.quads.push_back({ Face::BOTTOM, translationMatrix });
				}
			}

			// FRONT
			if (isAtFrontFace(i, size)) {
				// Get corresponding neighbour index in neighbour chunk and check.
				ui64 j = i + (size * size * (size - 1));
				if (!task.chunk->neighbours.right->blocks[j].present) {
					task.chunk->mesh.quads.push_back({ Face::FRONT, translationMatrix });
				}
			} else {
				// Get corresponding neighbour index in this chunk and check.
				if (!task.chunk->blocks[i - (size * size)].present) {
					task.chunk->mesh.quads.push_back({ Face::FRONT, translationMatrix });
				}
			}

			// BACK
			if (isAtBackFace(i, size)) {
				// Get corresponding neighbour index in neighbour chunk and check.
				ui64 j = i - (size * size * (size - 1));
				if (!task.chunk->neighbours.right->blocks[j].present) {
					task.chunk->mesh.quads.push_back({ Face::BACK, translationMatrix });
				}
			} else {
				// Get corresponding neighbour index in this chunk and check.
				if (!task.chunk->blocks[i + (size * size)].present) {
					task.chunk->mesh.quads.push_back({ Face::BACK, translationMatrix });
				}
			}
		}
	}
}
