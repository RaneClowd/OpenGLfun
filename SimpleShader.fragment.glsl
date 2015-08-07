#version 330

uniform mat4 modelMatrix;

uniform struct Light {
    vec3 position;
    vec3 intensities;
} light;

in vec4 fragColor;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 finalColor;

void main(void) {
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vec3 normal = normalize(normalMatrix * fragNormal);

    vec3 fragPosition = vec3(modelMatrix * vec4(fragVert, 1));

    vec3 surfaceToLight = light.position - fragPosition;

    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);

    finalColor = vec4(brightness * light.intensities * fragColor.rgb, fragColor.a);
}
