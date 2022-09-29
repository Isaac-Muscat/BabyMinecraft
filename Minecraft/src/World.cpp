#include "World.h"
#include <iostream>
#include <JoyGL/stb_image/stb_image.h>
#include <glad/glad.h>

World::World(const glm::vec3& spawnPos)
{
	// Create basic shader
	std::string vertexString = JoyGL::Shader::LoadShaderString("resources/Shaders/Basic.vert");
	std::string fragString = JoyGL::Shader::LoadShaderString("resources/Shaders/Basic.frag");
	m_BlockShader = std::make_shared<JoyGL::Shader>(vertexString.c_str(), fragString.c_str());
	m_ChunkManager = std::make_unique<ChunkManager>(spawnPos);

	// Initialize texture atlas
	glGenTextures(1, &m_GLTextureAtlasID);
	glBindTexture(GL_TEXTURE_2D, m_GLTextureAtlasID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("resources/Textures/JoyMinecraftTextureAtlas.png", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

World::~World()
{

}

void World::Draw(const glm::mat4& projection, const glm::mat4& view) const
{
	glEnable(GL_DEPTH_TEST);

	// Bind shaders and texture atlas
	m_BlockShader->Bind();
	glBindTexture(GL_TEXTURE_2D, m_GLTextureAtlasID);

	// Upload camera uniforms
	m_BlockShader->UploadUniformMatrix4fv("u_View", glm::value_ptr(view));
	m_BlockShader->UploadUniformMatrix4fv("u_Projection", glm::value_ptr(projection));

	// Draw the chunks
	for (const auto& chunkEntry : m_ChunkManager->GetChunks())
	{
		const std::shared_ptr<Chunk> chunk = chunkEntry.second;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, chunk->GetPosition());
		m_BlockShader->UploadUniformMatrix4fv("u_Model", glm::value_ptr(model));

		chunk->Draw();
	}
}

void World::Update(const glm::vec3 playerPos)
{
	
}

void World::UpdateChunksFromPlayerPos(const glm::vec3& playerPos, const glm::vec3& prevPlayerPos)
{
	glm::vec3 playerChunkPos = m_ChunkManager->GetChunkPositionFromPosition(playerPos);

	// Loop over x, y, z and update all chunks by offset Chunk::CHUNK_SIZE.
	for (float z = -World::RENDER_DISTANCE; z <= World::RENDER_DISTANCE; z++) {
		for (float y = -World::RENDER_DISTANCE; y <= World::RENDER_DISTANCE; y++) {
			for (float x = -World::RENDER_DISTANCE; x <= World::RENDER_DISTANCE; x++) {
				glm::vec3 chunkPos = playerChunkPos + glm::vec3(x * Chunk::CHUNK_SIZE, y * Chunk::CHUNK_SIZE, z * Chunk::CHUNK_SIZE);
				if (abs(x) == World::RENDER_DISTANCE || abs(y) == World::RENDER_DISTANCE || abs(z) == World::RENDER_DISTANCE) {
					m_ChunkManager->LoadChunk(chunkPos);
				}
			}
		}
	}
	for (float z = -World::RENDER_DISTANCE; z <= World::RENDER_DISTANCE; z++) {
		for (float y = -World::RENDER_DISTANCE; y <= World::RENDER_DISTANCE; y++) {
			for (float x = -World::RENDER_DISTANCE; x <= World::RENDER_DISTANCE; x++) {
				glm::vec3 chunkPos = playerChunkPos + glm::vec3(x * Chunk::CHUNK_SIZE, y * Chunk::CHUNK_SIZE, z * Chunk::CHUNK_SIZE);
				if (abs(x) == World::RENDER_DISTANCE || abs(y) == World::RENDER_DISTANCE || abs(z) == World::RENDER_DISTANCE) {
					m_ChunkManager->UpdateChunkMesh(m_ChunkManager->GetChunkFromPosition(chunkPos));
				}
			}
		}
	}
}
