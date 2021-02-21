#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int o_IDBuffer;

in vec2 v_TexCoord;
in vec3 v_Normals;
in vec3 v_FragPos;

uniform int u_ObjectID;

// PHONG STUFF
struct Material {
    sampler2D diffuse;
    vec3 specular;
    int shininess;
}; 
uniform Material u_Material;

// uniform vec3 lightPos;
uniform vec3 u_ViewPos;

void main()
{
    float ambientStrength = 0.1;

    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0) * vec3(texture(u_Material.diffuse, v_TexCoord)); // ambientStrength * lightColor ...

    vec3 norm = normalize(v_Normals);
    vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - v_FragPos);  // lightPos - v_FragPos

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0) * vec3(texture(u_Material.diffuse, v_TexCoord)); // diff * lightColor ...

    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = vec3(1.0, 1.0, 1.0) * (spec * u_Material.specular);  // lightColor * ...

    vec3 result = (ambient + diffuse + specular);

    // color = vec4(v_TexCoord, 0.0, 1.0); // -- UV Viz
    // color = texture(u_Texture, v_TexCoord);
    color = vec4(result, 1.0);

    o_IDBuffer = u_ObjectID;
}
