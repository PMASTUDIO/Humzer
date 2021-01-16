#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out int o_IDBuffer;

in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

void main(){
    color = texture(u_Skybox, v_TexCoords);
    o_IDBuffer = -1; // Temporary
    // color = vec4(1.0, 0.0, 1.zz)
}