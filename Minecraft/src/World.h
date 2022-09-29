#pragma once
#pragma once

#include <memory>
#include <vector>

#include "JoyGL/Core/Shader.h"

#include "ChunkManager.h"
#include "Chunk.h"

class World {
public:
	static const int RENDER_DISTANCE = 4;

	World(const glm::vec3& spawnPos);
	~World();

	void Draw(const glm::mat4& projection, const glm::mat4& view) const;
	void Update(const glm::vec3 playerPos);

	void UpdateChunksFromPlayerPos(const glm::vec3& playerPos, const glm::vec3& m_PrevPlayerPos);

private:
	std::shared_ptr<JoyGL::Shader> m_BlockShader;
	unsigned int m_GLTextureAtlasID;
	std::unique_ptr<ChunkManager> m_ChunkManager;
};