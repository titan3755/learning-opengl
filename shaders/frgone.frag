#version 330 core
in vec3 vertexColor;
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D textureOne;
void main()
{
	FragColor = texture(textureOne, TexCoord) * vec4(vertexColor, 1.0) * vec4(0.9, 0.9, 0.4, 1.0);
}