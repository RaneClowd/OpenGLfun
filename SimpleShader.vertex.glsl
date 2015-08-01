#version 330

layout(location=0) in vec3 in_position;

out vec4 ex_color;

uniform mat4 mvp;
uniform vec4 color;

void main(void) {
    ex_color = color;
    gl_Position = mvp * vec4(in_position.xyz, 1);
}
