#include "stdafx.h"

#include "voxel/ChunkGrid.h"

// TODO(Matthew): Do we need this. If the generator & mesher are well designed they could be data-driven hence no need for multiple instances.
void hvox::ChunkGrid::init(ChunkGenerator* generator, ChunkMesher* mesher) {
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

void hvox::ChunkGrid::submitGenTask(ChunkLOD lod, ChunkGenType type, ChunkGridPosition pos, Heightmapper heightmapper) {
    auto it = m_chunks.find(pos);
    Chunk* chunk = nullptr;
    if (it == m_chunks.end()) {
        chunk = createChunk(pos);
    } else {
        chunk = (*it).second;
    }
    m_genTasks.push({ lod, type, chunk, heightmapper });
}

void hvox::ChunkGrid::update() {
    GenCache cache;
    while (!m_genTasks.empty()) {
        ChunkGenTask& chunkGenTask = m_genTasks.front();

        // Mark a gen task as active: this will stop the chunk triggering block change events.
        chunkGenTask.chunk->flags.genTaskActive = true;

        m_generator->runGenTask(chunkGenTask, cache);

        // Reenable the triggering of block change events.
        chunkGenTask.chunk->flags.genTaskActive = false;

        // Add a mesh task for this chunk.
        if (!chunkGenTask.chunk->flags.hasMeshTask) {
            m_meshTasks.push(ChunkMeshTask{
                chunkGenTask.chunk
            });
            chunkGenTask.chunk->flags.hasMeshTask = true;
        }

        m_genTasks.pop();
    }
    // TODO(Matthew): This may not be the most appropriate way to handle eviction in the cache.
    GenCache().swap(cache);

    while (!m_meshTasks.empty()) {
        m_mesher->runMeshTask(m_meshTasks.front());
        m_meshTasks.pop();
    }
}

void hvox::ChunkGrid::handleBlockChange(h::Sender sender, BlockChangeEvent event) {
    Chunk* chunk = (Chunk*)sender;
    
    if (!chunk->flags.hasMeshTask) {
        m_meshTasks.push(ChunkMeshTask{
            chunk
        });
        chunk->flags.hasMeshTask = true;
    }
}

void hvox::ChunkGrid::handleBulkBlockChange(h::Sender sender, BulkBlockChangeEvent event) {
    Chunk* chunk = (Chunk*)sender;

    if (!chunk->flags.hasMeshTask) {
        m_meshTasks.push(ChunkMeshTask{
            chunk
        });
        chunk->flags.hasMeshTask = true;
    }
}

hvox::Chunk* hvox::ChunkGrid::createChunk(ChunkGridPosition pos) {
    Chunk* chunk = new Chunk();
    chunk->init(pos);
    m_chunks[pos] = chunk;

    chunk->onBlockChange     += makeDelegate(this, &ChunkGrid::handleBlockChange);
    chunk->onBulkBlockChange += makeDelegate(this, &ChunkGrid::handleBulkBlockChange);

    establishChunkNeighbours(chunk, pos);

    return chunk;
}

void hvox::ChunkGrid::establishChunkNeighbours(Chunk* chunk, ChunkGridPosition pos) {
    Chunk* temp = nullptr;
    ChunkGridPosition neighbourPos;

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
