#version 330

in vec4 texcoord;
uniform sampler2D texture;

out vec4 out_Color;

void main(void) {
    vec2 coord2d;
    float intensity;

    if (texcoord.w < 0.0) {
        coord2d = vec2((fract(texcoord.x) + texcoord.w) / 16.0, texcoord.z);
        intensity = 1.0;
    } else {
        coord2d = vec2((fract(texcoord.x + texcoord.z) + texcoord.w) / 16.0, -texcoord.y);
        intensity = 0.85;
    }

    out_Color = texture2D(texture, coord2d);

    if (out_Color.a < 0.4) discard;

    out_Color.xyz *= intensity;
}
