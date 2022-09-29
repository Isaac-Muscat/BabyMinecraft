#include "WorldGenerator.h"
#include "FastNoiseLite.h"


namespace WorldGenerator {

	FastNoiseLite noise;

	void ConfigureNoise() {
		noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	}

	void FillChunkList(Blocks::Block* blocks, glm::vec3 pos) {
		// Sky
		if (pos.y >= 0) {
			for (int i = 0; i < Chunk::CHUNK_SIZE_3; i++) {
				blocks[i] = Blocks::Block::Air;
			}
		}

		// Above Ground
		else if (pos.y >= -Chunk::CHUNK_SIZE) {
			for (float z = 0; z < Chunk::CHUNK_SIZE; z++) {
				for (float x = 0; x < Chunk::CHUNK_SIZE; x++) {
					double noiseHeight = noise.GetNoise((float)x + pos.x, (float)z + pos.z);
					int height = static_cast<int>((1 + noiseHeight) * 0.5 * Chunk::CHUNK_SIZE);

					for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
						int index = static_cast<int>(x + y * Chunk::CHUNK_SIZE + z * Chunk::CHUNK_SIZE_2);
						if (y > height) {
							blocks[index] = Blocks::Block::Air;
						}
						else if (y == height) {
							blocks[index] = Blocks::Block::Grass;
						}
						else {
							blocks[index] = Blocks::Block::Dirt;
						}
					}
				}
			}
		}

		// Underground
		else {
			for (int i = 0; i < Chunk::CHUNK_SIZE_3; i++) {
				blocks[i] = Blocks::Block::Stone;
			}
		}
	}
}
