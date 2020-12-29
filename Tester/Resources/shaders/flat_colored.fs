#version 450 core
			
out vec4 color;

in vec3 v_Normals;
in vec3 v_FragPos;

uniform vec4 u_Color;

// PHONG LIGHTING UNIFORMS
uniform vec3 u_ViewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
}; 
uniform Material u_Material;

void main()
{
 
    // ambient
    vec3 ambient = vec3(1.0, 1.0, 1.0) * u_Material.ambient; // light_color * ...

    // diffuse
    vec3 norm = normalize(v_Normals);
    vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - v_FragPos);  // lightPos - v_FragPos

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(1.0, 1.0, 1.0) * (diff * u_Material.diffuse); // lightColor * ...

    // specular
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = vec3(1.0, 1.0, 1.0) * (spec * u_Material.specular);    // lightColor * ...

    vec3 result = (ambient + diffuse + specular) * vec3(u_Color.xyz);

    color = vec4(result, 1.0);
}
