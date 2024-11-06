#version 330 core
in vec3 vertexColor;
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D textureOne;
uniform sampler2D textureTwo;
uniform float mixValue;
vec2 invtTexCoord = vec2(TexCoord.x, TexCoord.y);
void main()
{
	FragColor = mix(texture(textureOne, TexCoord), texture(textureTwo, invtTexCoord), mixValue);
}