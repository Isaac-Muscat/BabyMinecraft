#pragma once
#include <memory>
#include <vector>

#include "Block.h"
#include <glm/glm.hpp>

// Forward declaration
struct ChunkMesh;

// Used for hashmap indexing using chunk coordinates.
struct ChunkPositionHashFn {
	std::size_t operator() (const glm::vec3& pos) const {
		std::size_t xHash = std::hash<float>()(pos.x);
		std::size_t yHash = std::hash<float>()(pos.y);
		std::size_t zHash = std::hash<float>()(pos.z);

		return ((xHash ^ (yHash << 1)) >> 1) ^ (zHash << 1);
	}
};

class Chunk {
public:

	static constexpr int CHUNK_SIZE = 16;
	static constexpr int CHUNK_SIZE_2 = CHUNK_SIZE * CHUNK_SIZE;
	static constexpr int CHUNK_SIZE_3 = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
	
	// Utility functions
	inline static int GetX(int i) { return i % CHUNK_SIZE; }
	inline static int GetY(int i) { return static_cast<int>((i % CHUNK_SIZE_2)/ CHUNK_SIZE); }
	inline static int GetZ(int i) { return static_cast<int>(i / CHUNK_SIZE_2); }


	Chunk(glm::vec3 pos, std::shared_ptr<ChunkMesh> chunkMesh);
	~Chunk();

	void Draw() const;
	inline Blocks::Block GetBlock(const glm::vec3& pos) { return m_Blocks[static_cast<int>(pos.x + pos.y * CHUNK_SIZE + pos.z * CHUNK_SIZE_2)]; }
	inline Blocks::Block* GetBlocksArray() { return m_Blocks; }
	inline const glm::vec3& GetPosition() const { return m_Pos; }
	inline std::shared_ptr<ChunkMesh> GetChunkMesh() { return m_ChunkMesh; }
	inline const bool& IsChunkMeshLoaded() { return m_ChunkMeshLoaded; }
	inline void SetChunkMeshLoaded(bool chunkMeshLoaded) { m_ChunkMeshLoaded = chunkMeshLoaded; }
	void ResetNumMeshTriangles();

	// Helpers to construct the mesh efficiently
	void AddFrontBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& pos, const Blocks::BlockFaceCoords& c);
	void AddBackBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& pos, const Blocks::BlockFaceCoords& c);
	void AddTopBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& pos, const Blocks::BlockFaceCoords& c);
	void AddBottomBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& pos, const Blocks::BlockFaceCoords& c);
	void AddLeftBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& pos, const Blocks::BlockFaceCoords& c);
	void AddRightBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& pos, const Blocks::BlockFaceCoords& c);
	void UpdateFaceIndices(std::vector<unsigned int>& indices);

private:
	bool m_ChunkMeshLoaded;
	std::shared_ptr<ChunkMesh> m_ChunkMesh;
	glm::vec3 m_Pos;
	Blocks::Block* m_Blocks;
};