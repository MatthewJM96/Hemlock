#include "stdafx.h"

#include "voxel\ChunkGrid.h"

void hvox::ChunkGrid::init(ui16 size, IChunkGenerator* generator, ChunkMesher* mesher) {
	m_size = size;
	m_generator = generator;
	m_mesher = mesher;
}

void hvox::ChunkGrid::dispose() {
	// Note generator and mesher should only be deleted by initialising code.
	m_generator = nullptr;
	m_mesher    = nullptr;

	std::queue<ChunkGenTask>().swap(m_genTasks);
	std::queue<ChunkMeshTask>().swap(m_meshTasks);
	std::unordered_map<ChunkID, Chunk*>().swap(m_chunks);
}

void hvox::ChunkGrid::submitGenTask(ChunkLOD lod, ChunkGenType type, ChunkRectilinearWorldPosition pos) {
	auto& it = m_chunks.find(pos);
	Chunk* chunk = nullptr;
	if (it == m_chunks.end()) {
		chunk = new Chunk();
		chunk->init(m_size);
		m_chunks[pos] = chunk;

		chunk->onBlockChange += makeDelegate(*this, &ChunkGrid::handleBlockChange);
		chunk->onBulkBlockChange += makeDelegate(*this, &ChunkGrid::handleBulkBlockChange);

		Chunk* temp = nullptr;
		ChunkRectilinearWorldPosition neighbourPos = pos;

		// Update neighbours with info of new chunk.
		// LEFT
		neighbourPos = pos;
		neighbourPos.x -= 1;
		it = m_chunks.find(neighbourPos);
		if (it != m_chunks.end()) {
			temp = (*it).second;
			chunk->neighbours.left = temp;
			temp->neighbours.right = chunk;
		} else {
			chunk->neighbours.left = nullptr;
		}

		// RIGHT
		neighbourPos = pos;
		neighbourPos.x += 1;
		it = m_chunks.find(neighbourPos);
		if (it != m_chunks.end()) {
			temp = (*it).second;
			chunk->neighbours.right = temp;
			temp->neighbours.left = chunk;
		} else {
			chunk->neighbours.right = nullptr;
		}

		// TOP
		neighbourPos = pos;
		neighbourPos.y += 1;
		it = m_chunks.find(neighbourPos);
		if (it != m_chunks.end()) {
			temp = (*it).second;
			chunk->neighbours.top = temp;
			temp->neighbours.bottom = chunk;
		} else {
			chunk->neighbours.top = nullptr;
		}

		// BOTTOM
		neighbourPos = pos;
		neighbourPos.y -= 1;
		it = m_chunks.find(neighbourPos);
		if (it != m_chunks.end()) {
			temp = (*it).second;
			chunk->neighbours.bottom = temp;
			temp->neighbours.top = chunk;
		} else {
			chunk->neighbours.bottom = nullptr;
		}

		// FRONT
		neighbourPos = pos;
		neighbourPos.z -= 1;
		it = m_chunks.find(neighbourPos);
		if (it != m_chunks.end()) {
			temp = (*it).second;
			chunk->neighbours.front = temp;
			temp->neighbours.back = chunk;
		} else {
			chunk->neighbours.front = nullptr;
		}

		// BACK
		neighbourPos = pos;
		neighbourPos.z += 1;
		it = m_chunks.find(neighbourPos);
		if (it != m_chunks.end()) {
			temp = (*it).second;
			chunk->neighbours.back = temp;
			temp->neighbours.front = chunk;
		} else {
			chunk->neighbours.back = nullptr;
		}
	} else {
		chunk = (*it).second;
	}

	m_genTasks.push({ lod, type, pos, chunk });
}

void hvox::ChunkGrid::update() {
	while (!m_genTasks.empty()) {
		m_generator->runGenTask(m_genTasks.front(), m_size);
		m_genTasks.pop();
	}
	while (!m_meshTasks.empty()) {
		m_mesher->runMeshTask(m_meshTasks.front(), m_size);
		m_meshTasks.pop();
	}
}

void hvox::ChunkGrid::handleBlockChange(h::Sender sender, BlockChangeEvent event) {
	m_meshTasks.push({
		event.chunkPos,
		(Chunk*)sender
	});
}

void hvox::ChunkGrid::handleBulkBlockChange(h::Sender sender, BulkBlockChangeEvent event) {
	m_meshTasks.push({
		event.chunkPos,
		(Chunk*)sender
	});
}
