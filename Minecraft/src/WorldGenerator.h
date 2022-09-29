#pragma once
#include "Chunk.h"

namespace WorldGenerator {
	void FillChunkList(Blocks::Block* blocks, glm::vec3 pos);
	void ConfigureNoise();
}