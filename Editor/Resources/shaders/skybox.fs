#version 330 core

out vec4 color;

in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

void main(){
    color = texture(u_Skybox, v_TexCoords);
    // color = vec4(1.0, 0.0, 1.zz)
}