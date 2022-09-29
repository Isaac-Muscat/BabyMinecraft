#pragma once

#include <set>
#include <vector>
#include <unordered_map>
#include "Chunk.h"

struct ChunkMesh {
	unsigned int m_GLVBAID;
	unsigned int m_GLVBOID;
	unsigned int m_GLIBOID;
	int m_NumMeshTriangles;
};

class ChunkManager {
public:
	// X points right, Y points up, Z points into camera.
	enum class AdjacentChunk { FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT };
	ChunkManager(const glm::vec3& spawnPos);

	// Will create a new chunk.
	// Finds an available chunk mesh if one is unused.
	// Creates a new chunk mesh if non are in use.
	// Does not update the chunk.
	void LoadChunk(glm::vec3 chunkPosition);
	void UpdateChunkMesh(std::shared_ptr<Chunk> chunk);

	// Offloads chunk from m_ChunkMeshes.
	void UnloadChunk(std::shared_ptr<Chunk> chunk);
	std::shared_ptr<Chunk> GetChunkFromPosition(const glm::vec3& pos);
	glm::vec3 GetChunkPositionFromPosition(const glm::vec3& pos);


	inline std::unordered_map<glm::vec3, std::shared_ptr<Chunk>, ChunkPositionHashFn>& GetChunks() { return m_Chunks; }

	inline Blocks::Block* GetBlocksArrayFromChunk(glm::vec3 chunkPosition) {
		return m_Chunks.at(chunkPosition)->GetBlocksArray();
	}

private:
	std::shared_ptr<ChunkMesh> CreateChunkMesh();

	// Returns if air block in a neighbouring chunk.
	bool AirBlockInAdjacentChunk(std::shared_ptr<Chunk> chunk, const glm::vec3& blockPos, AdjacentChunk adjacentChunkType) const;
private:
	glm::vec3 m_SpawnPos;
	std::vector<std::shared_ptr<ChunkMesh>> m_ChunkMeshes;
	std::vector<std::shared_ptr<ChunkMesh>> m_FreeChunkMeshes; // Subset of m_ChunkMeshes.

	std::unordered_map<glm::vec3, std::shared_ptr<Chunk>, ChunkPositionHashFn> m_Chunks;
};