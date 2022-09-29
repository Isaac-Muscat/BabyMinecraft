#version 460 core

out vec4 a_Color;

in vec2 TexCoord;

uniform sampler2D m_Texture;

void main() {
	a_Color = texture(m_Texture, TexCoord);
}