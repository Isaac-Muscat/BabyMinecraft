#include "Chunk.h"

#include <cmath>

#include <glad/glad.h>

#include "ChunkManager.h"
#include "WorldGenerator.h"

Chunk::Chunk(glm::vec3 pos, std::shared_ptr<ChunkMesh> chunkMesh)
	: m_Pos(pos), m_ChunkMeshLoaded(false), m_ChunkMesh(chunkMesh), m_Blocks(new Blocks::Block[CHUNK_SIZE_3]) {

	WorldGenerator::FillChunkList(m_Blocks, m_Pos);
}

Chunk::~Chunk()
{
	delete[] m_Blocks;
}

void Chunk::Draw() const
{
	assert(m_ChunkMesh != nullptr);
	
	glBindVertexArray(m_ChunkMesh->m_GLVBAID);
	glDrawElements(GL_TRIANGLES, m_ChunkMesh->m_NumMeshTriangles * 3, GL_UNSIGNED_INT, 0);
}


void Chunk::ResetNumMeshTriangles()
{
	m_ChunkMesh->m_NumMeshTriangles = 0;
}

void Chunk::AddFrontBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& p, const Blocks::BlockFaceCoords& c)
{
	float face[20] = {
		p.x,			p.y,			1.0f + p.z,	(c.SideX + 1.0f) * Blocks::SCALE,	c.SideY * Blocks::SCALE,			// Bottom left
		1.0f + p.x,	p.y,			1.0f + p.z,	c.SideX * Blocks::SCALE,			c.SideY * Blocks::SCALE,			// Bottom right
		1.0f + p.x,	1.0f + p.y,	1.0f + p.z,	c.SideX * Blocks::SCALE,			(c.SideY + 1.0f) * Blocks::SCALE,	// Top right
		p.x,			1.0f + p.y,	1.0f + p.z,	(c.SideX + 1.0f) * Blocks::SCALE,	(c.SideY + 1.0f) * Blocks::SCALE,	// Top left
	};
	vertices.insert(vertices.end(), face, face + 20);
	UpdateFaceIndices(indices);

}

void Chunk::AddBackBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& p, const Blocks::BlockFaceCoords& c)
{
	float face[20] = {
		1.0f + p.x, p.y, p.z,	(c.SideX + 1.0f)* Blocks::SCALE, c.SideY* Blocks::SCALE,		// Back Bottom right
		p.x, p.y, p.z,	c.SideX* Blocks::SCALE, c.SideY* Blocks::SCALE,			// Back Bottom left
		p.x,	1.0f + p.y, p.z,	c.SideX* Blocks::SCALE, (c.SideY + 1.0f)* Blocks::SCALE,		// Back Top left
		1.0f + p.x,	1.0f + p.y, p.z,	(c.SideX + 1.0f)* Blocks::SCALE, (c.SideY + 1.0f)* Blocks::SCALE,// Back Top right
	};
	vertices.insert(vertices.end(), face, face + 20);
	UpdateFaceIndices(indices);
}

void Chunk::AddTopBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& p, const Blocks::BlockFaceCoords& c)
{
	float face[20] = {
		p.x,	1.0f + p.y,  1.0f + p.z,	(c.TopX + 1.0f)* Blocks::SCALE, c.TopY* Blocks::SCALE,		// Top left
		1.0f + p.x,	1.0f + p.y,  1.0f + p.z,	c.TopX* Blocks::SCALE, c.TopY* Blocks::SCALE,			// Top right
		1.0f + p.x,	1.0f + p.y, p.z,	c.TopX* Blocks::SCALE, (c.TopY + 1.0f)* Blocks::SCALE,		// Back Top right
		p.x,	1.0f + p.y, p.z,	(c.TopX + 1.0f)* Blocks::SCALE, (c.TopY + 1.0f)* Blocks::SCALE,// Back Top left
	};
	vertices.insert(vertices.end(), face, face + 20);
	UpdateFaceIndices(indices);
}

void Chunk::AddBottomBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& p, const Blocks::BlockFaceCoords& c)
{
	float face[20] = {
		1.0f + p.x, p.y,  1.0f + p.z,	(c.BotX + 1.0f)* Blocks::SCALE, c.BotY* Blocks::SCALE,		// Bottom right
		p.x, p.y,  1.0f + p.z,	c.BotX* Blocks::SCALE, c.BotY* Blocks::SCALE,			// Bottom left
		p.x, p.y, p.z,	c.BotX* Blocks::SCALE, (c.BotY + 1.0f)* Blocks::SCALE,		// Back Bottom left
		1.0f + p.x, p.y, p.z,	(c.BotX + 1.0f)* Blocks::SCALE, (c.BotY + 1.0f)* Blocks::SCALE,// Back Bottom right
	};
	vertices.insert(vertices.end(), face, face + 20);
	UpdateFaceIndices(indices);
}

void Chunk::AddLeftBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& p, const Blocks::BlockFaceCoords& c)
{
	float face[20] = {
		p.x, p.y, p.z,	(c.SideX + 1.0f) * Blocks::SCALE, c.SideY * Blocks::SCALE,		// Back Bottom left
		p.x, p.y,  1.0f + p.z,	c.SideX * Blocks::SCALE, c.SideY * Blocks::SCALE,				// Bottom left
		p.x,	1.0f + p.y,  1.0f + p.z,	c.SideX * Blocks::SCALE, (c.SideY + 1.0f) * Blocks::SCALE,		// Top left
		p.x,	1.0f + p.y, p.z,	(c.SideX + 1.0f) * Blocks::SCALE, (c.SideY + 1.0f) * Blocks::SCALE,	// Back Top left
	};
	vertices.insert(vertices.end(), face, face + 20);
	UpdateFaceIndices(indices);
}

void Chunk::AddRightBlockFace(std::vector<float>& vertices, std::vector<unsigned int>& indices, const glm::vec3& p, const Blocks::BlockFaceCoords& c)
{
	float face[20] = {
		1.0f + p.x, p.y,  1.0f + p.z,	(c.SideX + 1.0f) * Blocks::SCALE, c.SideY * Blocks::SCALE,				// Bottom right
		1.0f + p.x, p.y, p.z,	c.SideX * Blocks::SCALE, c.SideY * Blocks::SCALE,		// Back Bottom right
		1.0f + p.x,	1.0f + p.y, p.z,	c.SideX * Blocks::SCALE, (c.SideY + 1.0f) * Blocks::SCALE,	// Back Top right
		1.0f + p.x,	1.0f + p.y,  1.0f + p.z,	(c.SideX + 1.0f) * Blocks::SCALE, (c.SideY + 1.0f) * Blocks::SCALE,		// Top right
	};
	vertices.insert(vertices.end(), face, face + 20);
	UpdateFaceIndices(indices);
}

void Chunk::UpdateFaceIndices(std::vector<unsigned int>& indices)
{
	int offset = m_ChunkMesh->m_NumMeshTriangles * 2;
	unsigned int blockIndices[] = { 0u + offset, 1u + offset, 2u + offset, 0u + offset, 2u + offset, 3u + offset };
	indices.insert(indices.end(), blockIndices, blockIndices + 6);
	m_ChunkMesh->m_NumMeshTriangles += 2;
}
