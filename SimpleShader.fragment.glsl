#version 330

uniform mat4 modelMatrix;
uniform vec3 cameraPosition;

uniform float shininess;
uniform vec3 specularColor;

uniform struct Light {
    vec3 position;
    vec3 intensities;
    float attenuation;
    float ambientCoefficient;
} light;

in vec4 fragColor;
in vec3 fragNormal;
in vec3 fragVert;


in vec2 fragTexCoords;

uniform bool useTexture;
uniform sampler2D textureSampler;


layout(location=0) out vec4 finalColor;

void main(void) {
    if (useTexture) {
        finalColor = texture(textureSampler, fragTexCoords);
    } else {
        vec3 normal = normalize(transpose(inverse(mat3(modelMatrix))) * fragNormal);
        vec3 fragPosition = vec3(modelMatrix * vec4(fragVert, 1));
        vec3 surfaceToLight = normalize(light.position - fragPosition);
        vec3 surfaceToCamera = normalize(cameraPosition - fragPosition);

        vec3 ambient = light.ambientCoefficient * fragColor.rgb * light.intensities;

        float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
        vec3 diffuse = diffuseCoefficient * fragColor.rgb * light.intensities;

        float specularCoefficient = 0.0;
        if (diffuseCoefficient > 0.0)
            specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), shininess);
        vec3 specular = specularCoefficient * specularColor * light.intensities;

        float distanceToLight = length(light.position - fragPosition);
        float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

        vec3 linearColor = ambient + attenuation*(diffuse+specular);

        vec3 gamma = vec3(1.0/2.2);
        finalColor = vec4(pow(linearColor, gamma), fragColor.a);
    }
}
