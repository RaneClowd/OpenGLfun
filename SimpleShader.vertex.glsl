#version 330

uniform mat4 mvp;
uniform vec3 color;

layout(location=0) in vec3 vert;
layout(location=1) in vec3 vertNormal;

out vec3 fragVert;
out vec4 fragColor;
out vec3 fragNormal;

void main(void) {
    fragColor = vec4(color.xyz, 1);
    fragNormal = vertNormal;
    fragVert = vert;

    gl_Position = mvp * vec4(vert.xyz, 1);
}
