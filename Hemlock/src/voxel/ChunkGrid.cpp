#include "stdafx.h"

#include "voxel/ChunkGrid.h"

void hvox::ChunkGrid::init(ui16 size, IChunkGenerator* generator, ChunkMesher* mesher) {
	m_size      = size;
	m_generator = generator;
	m_mesher    = mesher;
}

void hvox::ChunkGrid::dispose() {
	// Note generator and mesher should only be deleted by initialising code.
	m_generator = nullptr;
	m_mesher    = nullptr;

    GenTasks().swap(m_genTasks);
	MeshTasks().swap(m_meshTasks);
	Chunks().swap(m_chunks);
}

void hvox::ChunkGrid::submitGenTask(ChunkLOD lod, ChunkGenType type, ChunkRectilinearWorldPosition pos) {
	auto it = m_chunks.find(pos);
	Chunk* chunk = nullptr;
	if (it == m_chunks.end()) {
        chunk = createChunk(pos);
	} else {
		chunk = (*it).second;
	}

	m_genTasks.push({ lod, type, chunk, { pos } });
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
    // TODO(Matthew): We really shouldn't be submitting a mesh task for each block change...
    //                We want one mesh task per chunk per frame AT MOST regardless of number of blocks changed.
	m_meshTasks.push({
		{ event.chunkPos },
		(Chunk*)sender
	});
}

void hvox::ChunkGrid::handleBulkBlockChange(h::Sender sender, BulkBlockChangeEvent event) {
    // TODO(Matthew): We really shouldn't be submitting a mesh task for each block change...
    //                We want one mesh task per chunk per frame AT MOST regardless of number of blocks changed.
	m_meshTasks.push({
	 	{ event.chunkPos },
		(Chunk*)sender
	});
}

hvox::Chunk* hvox::ChunkGrid::createChunk(ChunkRectilinearWorldPosition pos) {
    Chunk* chunk = new Chunk();
    chunk->init(m_size, pos);
    m_chunks[pos] = chunk;

    chunk->onBlockChange += makeDelegate(*this, &ChunkGrid::handleBlockChange);
    chunk->onBulkBlockChange += makeDelegate(*this, &ChunkGrid::handleBulkBlockChange);

    establishChunkNeighbours(chunk, pos);

    return chunk;
}

void hvox::ChunkGrid::establishChunkNeighbours(Chunk* chunk, ChunkRectilinearWorldPosition pos) {
    Chunk* temp = nullptr;
    ChunkRectilinearWorldPosition neighbourPos;

    // Update neighbours with info of new chunk.
    // LEFT
    neighbourPos = pos;
    neighbourPos.x -= 1;
    auto it = m_chunks.find(neighbourPos);
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
}
