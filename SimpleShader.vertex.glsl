#version 330

layout(location=0) in vec3 in_position;

out vec4 ex_color;

uniform mat4 mvp;
uniform vec3 color;

void main(void) {
    ex_color = vec4(color.xyz, 1);
    gl_Position = mvp * vec4(in_position.xyz, 1);
}
