#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normals;
out vec3 v_FragPos;

void main()
{
    v_Normals = mat3(transpose(inverse(u_Transform))) * a_Normals;
    v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    gl_Position = u_ViewProjection * (u_Transform * vec4(a_Position, 1.0));	
}
