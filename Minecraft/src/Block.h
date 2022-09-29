#pragma once

namespace Blocks {

	extern constexpr float SCALE = 16.0f / 256.0f;

	enum class Block {
		Air, Grass, Dirt, Stone, Log
	};

	// Bottom left of texture atlas is (0, 0).
	// Top right of texture atlas is (16, 16).
	struct BlockFaceCoords {
		int TopX, TopY;
		int SideX, SideY;
		int BotX, BotY;
	};

	// Texture coordinates are returned from the bottom left of 
	// the target texture from the texture atlas.
	inline BlockFaceCoords GetBlockFaceCoords(Block block) {
		switch (block) {

		case Block::Grass:
			return { 2, 15, 1, 15, 0, 15 };
			break;

		case Block::Dirt:
			return { 0, 15, 0, 15, 0, 15 };
			break;

		case Block::Stone:
			return { 3, 15, 3, 15, 3, 15 };
			break;

		case Block::Log:
			return { 5, 15, 4, 15, 4, 15 };
			break;
		
		// Just return a grass block if not found
		default:
			return { 2, 15, 1, 15, 0, 15 };
		}
	}

}