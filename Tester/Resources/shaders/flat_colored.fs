#version 450 core
			
out vec4 color;

in vec3 v_Normals;
in vec3 v_FragPos;

uniform vec4 u_Color;

// PHONG LIGHTING UNIFORMS
uniform vec3 u_ViewPos;

void main()
{
    float ambientStrength = 0.1;    
    float specularStrength = 0.5;
 
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

    vec3 norm = normalize(v_Normals);
    vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - v_FragPos);  // lightPos - v_FragPos

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0); // diff * lightColor ...

    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);  // ... * lightColor

    vec3 result = (ambient + diffuse + specular) * vec3(u_Color.xyz);

    color = vec4(result, 1.0);
}
