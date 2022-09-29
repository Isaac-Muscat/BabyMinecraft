#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;

out vec2 TexCoord;

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main() {
	gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0f, 1.0f);
	TexCoord = a_TexCoord;
}