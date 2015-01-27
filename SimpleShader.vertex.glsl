#version 330

layout(location=0) in vec4 coord;
out vec4 texcoord;

uniform mat4 mvp;

void main(void) {
    texcoord = coord;
    gl_Position = mvp * vec4(coord.xyz, 1);
}
