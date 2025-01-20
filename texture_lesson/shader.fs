#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 FragColor;
uniform sampler2D ourTexture; //the texture reference
uniform sampler2D otherTexture;

void main()
{
    // mix -> linearly interpolate between two values. 0.2 returns 80% of first value and 20% of the sescond value.
    FragColor = mix(texture(ourTexture, texCoord), texture(otherTexture, texCoord), 0.2);
}