#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 FragColor;
uniform sampler2D ourTexture; //the texture reference

void main()
{
    FragColor = texture(ourTexture, texCoord) * vec4(color, 1.0);
}