# version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform float offset;
out vec4 position;
void main()
{
    vec3 offset_vector = vec3(offset, 0.0, 0.0);
    position = vec4(aPos + offset_vector, 1.0);
    gl_Position = position;

}