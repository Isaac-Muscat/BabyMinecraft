#version 460 core

out vec4 a_Color;

in vec2 TexCoord;

uniform sampler2D textureAtlas;

void main() {
	a_Color = texture(textureAtlas, TexCoord);
}