#include "ChunkManager.h"
#include "Chunk.h"
#include "World.h"
#include <glad/glad.h>
#include <iostream>

ChunkManager::ChunkManager(const glm::vec3& spawnPos)
	: m_SpawnPos(spawnPos) {
	for (float z = -World::RENDER_DISTANCE; z <= World::RENDER_DISTANCE; z++) {
		for (float y = -World::RENDER_DISTANCE; y <= World::RENDER_DISTANCE; y++) {
			for (float x = -World::RENDER_DISTANCE; x <= World::RENDER_DISTANCE; x++) {
				glm::vec3 chunkPos = glm::vec3(x * Chunk::CHUNK_SIZE, y * Chunk::CHUNK_SIZE, z * Chunk::CHUNK_SIZE);
				LoadChunk(chunkPos);
			}
		}
	}

	for (float z = -World::RENDER_DISTANCE; z <= World::RENDER_DISTANCE; z++) {
		for (float y = -World::RENDER_DISTANCE; y <= World::RENDER_DISTANCE; y++) {
			for (float x = -World::RENDER_DISTANCE; x <= World::RENDER_DISTANCE; x++) {
				glm::vec3 chunkPos = glm::vec3(x * Chunk::CHUNK_SIZE, y * Chunk::CHUNK_SIZE, z * Chunk::CHUNK_SIZE);
				UpdateChunkMesh(m_Chunks.at(chunkPos));
			}
		}
	}
}


void ChunkManager::LoadChunk(glm::vec3 chunkPosition)
{
	// Return if chunk is already loaded.
	if (m_Chunks.find(chunkPosition) != m_Chunks.end()) return;

	std::shared_ptr<ChunkMesh> chunkMesh;
	if (m_FreeChunkMeshes.empty()) {
		chunkMesh = CreateChunkMesh();
		m_ChunkMeshes.push_back(chunkMesh);
	}
	else {
		chunkMesh = m_FreeChunkMeshes.back();
		m_FreeChunkMeshes.pop_back();
	}

	// Insert current chunk.
	std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(chunkPosition, chunkMesh);
	m_Chunks.insert({ chunkPosition, chunk });
}

void ChunkManager::UnloadChunk(std::shared_ptr<Chunk> chunk)
{
	m_FreeChunkMeshes.push_back(chunk->GetChunkMesh());
	m_Chunks.erase(chunk->GetPosition());
}

std::shared_ptr<Chunk> ChunkManager::GetChunkFromPosition(const glm::vec3& pos)
{
	glm::vec3 chunkPosition = GetChunkPositionFromPosition(pos);
	if (m_Chunks.find(chunkPosition) == m_Chunks.end())
	{
		return nullptr;
	}
	else
	{
		return m_Chunks.at(chunkPosition);
	}
}

glm::vec3 ChunkManager::GetChunkPositionFromPosition(const glm::vec3& pos)
{

	glm::vec3 chunkPosRelToSpawn = pos - m_SpawnPos;

	return glm::vec3(
		floor(chunkPosRelToSpawn.x / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE,
		floor(chunkPosRelToSpawn.y / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE,
		floor(chunkPosRelToSpawn.z / Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE
	);
}

void ChunkManager::UpdateChunkMesh(std::shared_ptr<Chunk> chunk)
{
	if (chunk == nullptr || chunk->IsChunkMeshLoaded()) return;
	chunk->SetChunkMeshLoaded(true);

	// Reset number of triangles in the mesh
	chunk->ResetNumMeshTriangles();
	
	// Fetch data.
	Blocks::Block* chunkBlocks = chunk->GetBlocksArray();
	std::shared_ptr<ChunkMesh> chunkMesh= chunk->GetChunkMesh();
	glm::vec3 chunkPosition = glm::vec3(chunk->GetPosition());

	// Define chunk offsets.
	glm::vec3 frontOffset (0, 0, Chunk::CHUNK_SIZE);
	glm::vec3 rightOffset = glm::vec3(Chunk::CHUNK_SIZE, 0, 0);
	glm::vec3 topOffset = glm::vec3(0, Chunk::CHUNK_SIZE, 0);


	// Initialize buffer data
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int offset = 0;

	// Chunk::CHUNK_SIZE_3 is size of block array.
	for (int i = 0; i < Chunk::CHUNK_SIZE_3; i++) {

		if (chunkBlocks[i] == Blocks::Block::Air) continue;

		glm::vec3 blockPos { Chunk::GetX(i), Chunk::GetY(i), Chunk::GetZ(i) };
		Blocks::BlockFaceCoords c = Blocks::GetBlockFaceCoords(chunkBlocks[i]);


		// Check to add front face to chunk mesh.
		if ((blockPos.z >= Chunk::CHUNK_SIZE - 1 && AirBlockInAdjacentChunk(chunk, blockPos, AdjacentChunk::FRONT)) ||
			(blockPos.z < Chunk::CHUNK_SIZE - 1 && chunkBlocks[i + Chunk::CHUNK_SIZE_2] == Blocks::Block::Air))
		{
			chunk->AddFrontBlockFace(vertices, indices, blockPos, c);
		}

		// Check to add back face to chunk mesh.
		if ((blockPos.z < 1 && AirBlockInAdjacentChunk(chunk, blockPos, AdjacentChunk::BACK)) ||
			(blockPos.z >= 1 && chunkBlocks[i - Chunk::CHUNK_SIZE_2] == Blocks::Block::Air))
		{
			chunk->AddBackBlockFace(vertices, indices, blockPos, c);
		}

		// Check to add top face to chunk mesh.
		if ((blockPos.y >= Chunk::CHUNK_SIZE - 1 && AirBlockInAdjacentChunk(chunk, blockPos, AdjacentChunk::TOP)) ||
			(blockPos.y < Chunk::CHUNK_SIZE - 1 && chunkBlocks[i + Chunk::CHUNK_SIZE] == Blocks::Block::Air))
		{
			chunk->AddTopBlockFace(vertices, indices, blockPos, c);
		}

		// Check to add bottom face to chunk mesh.
		if ((blockPos.y < 1 && AirBlockInAdjacentChunk(chunk, blockPos, AdjacentChunk::BOTTOM)) ||
			(blockPos.y >= 1 && chunkBlocks[i - Chunk::CHUNK_SIZE] == Blocks::Block::Air))
		{
			chunk->AddBottomBlockFace(vertices, indices, blockPos, c);
		}

		// Check to add left face to chunk mesh.
		if ((blockPos.x < 1 && AirBlockInAdjacentChunk(chunk, blockPos, AdjacentChunk::LEFT)) ||
			(blockPos.x >= 1 && chunkBlocks[i - 1] == Blocks::Block::Air))
		{
			chunk->AddLeftBlockFace(vertices, indices, blockPos, c);
		}

		// Check to add right face to chunk mesh.
		if ((blockPos.x >= Chunk::CHUNK_SIZE - 1 && AirBlockInAdjacentChunk(chunk, blockPos, AdjacentChunk::RIGHT)) ||
			(blockPos.x < Chunk::CHUNK_SIZE - 1 && chunkBlocks[i + 1] == Blocks::Block::Air))
		{
			chunk->AddRightBlockFace(vertices, indices, blockPos, c);
		}
	}

	glBindVertexArray(chunkMesh->m_GLVBAID);
	glBindBuffer(GL_ARRAY_BUFFER, chunkMesh->m_GLVBOID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh->m_GLIBOID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());
}

std::shared_ptr<ChunkMesh> ChunkManager::CreateChunkMesh()
{
	std::shared_ptr<ChunkMesh> chunkMesh = std::make_shared<ChunkMesh>();

	// Initialize vertex array
	glCreateVertexArrays(1, &chunkMesh->m_GLVBAID);
	glBindVertexArray(chunkMesh->m_GLVBAID);

	// Initialize vertex buffer
	glCreateBuffers(1, &chunkMesh->m_GLVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, chunkMesh->m_GLVBOID);
	glBufferData(GL_ARRAY_BUFFER, 4 * 5 * 6 * Chunk::CHUNK_SIZE_3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	// Initialize index buffer
	glCreateBuffers(1, &chunkMesh->m_GLIBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunkMesh->m_GLIBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 6 * Chunk::CHUNK_SIZE_3 * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

	// Setup vertex array attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexArrayAttrib(chunkMesh->m_GLVBAID, 0);
	glEnableVertexArrayAttrib(chunkMesh->m_GLVBAID, 1);

	// Unbind vertex array
	glBindVertexArray(0);

	return chunkMesh;
}

bool ChunkManager::AirBlockInAdjacentChunk(std::shared_ptr<Chunk> chunk, const glm::vec3& blockPos, AdjacentChunk adjacentChunkType) const
{
	// Define chunk offsets.
	static const glm::vec3 FRONT_OFFSET(0, 0, Chunk::CHUNK_SIZE);
	static const glm::vec3 TOP_OFFSET = glm::vec3(0, Chunk::CHUNK_SIZE, 0);
	static const glm::vec3 RIGHT_OFFSET = glm::vec3(Chunk::CHUNK_SIZE, 0, 0);

	std::unordered_map<glm::vec3, std::shared_ptr<Chunk>, ChunkPositionHashFn>::const_iterator adjacentChunk;
	
	switch (adjacentChunkType) {
	case AdjacentChunk::FRONT:
		adjacentChunk = m_Chunks.find(chunk->GetPosition() + FRONT_OFFSET);
		if (adjacentChunk == m_Chunks.end()) return true;
		else if (adjacentChunk->second->GetBlock(glm::vec3(blockPos.x, blockPos.y, 0)) == Blocks::Block::Air) return true;

		break;
	case AdjacentChunk::BACK:
		adjacentChunk = m_Chunks.find(chunk->GetPosition() - FRONT_OFFSET);
		if (adjacentChunk == m_Chunks.end()) return true;
		else if (adjacentChunk->second->GetBlock(glm::vec3(blockPos.x, blockPos.y, Chunk::CHUNK_SIZE - 1)) == Blocks::Block::Air) return true;

		break;
	case AdjacentChunk::TOP:
		adjacentChunk = m_Chunks.find(chunk->GetPosition() + TOP_OFFSET);
		if (adjacentChunk == m_Chunks.end()) return true;
		else if (adjacentChunk->second->GetBlock(glm::vec3(blockPos.x, 0, blockPos.z)) == Blocks::Block::Air) return true;

		break;
	case AdjacentChunk::BOTTOM:
		adjacentChunk = m_Chunks.find(chunk->GetPosition() - TOP_OFFSET);
		if (adjacentChunk == m_Chunks.end()) return true;
		else if (adjacentChunk->second->GetBlock(glm::vec3(blockPos.x, Chunk::CHUNK_SIZE - 1, blockPos.z)) == Blocks::Block::Air) return true;

		break;
	case AdjacentChunk::LEFT:
		adjacentChunk = m_Chunks.find(chunk->GetPosition() - RIGHT_OFFSET);
		if (adjacentChunk == m_Chunks.end()) return true;
		else if (adjacentChunk->second->GetBlock(glm::vec3(Chunk::CHUNK_SIZE - 1, blockPos.y, blockPos.z)) == Blocks::Block::Air) return true;

		break;
	case AdjacentChunk::RIGHT:
		adjacentChunk = m_Chunks.find(chunk->GetPosition() + RIGHT_OFFSET);
		if (adjacentChunk == m_Chunks.end()) return true;
		else if (adjacentChunk->second->GetBlock(glm::vec3(0, blockPos.y, blockPos.z)) == Blocks::Block::Air) return true;

		break;
	
	}

	return false;
}

