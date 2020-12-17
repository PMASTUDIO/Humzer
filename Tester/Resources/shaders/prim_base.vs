#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;

out vec3 v_Position;
out vec4 v_Color;

void main()
{
    v_Position = a_Position;
    v_Color = a_Color;
    gl_Position = vec4(a_Position, 1.0);	
}
