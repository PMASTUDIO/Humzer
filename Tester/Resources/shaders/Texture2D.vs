#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_ObjectID;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;
out flat int v_ObjectID;

void main()
{
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
    v_TilingFactor = a_TilingFactor;
    v_ObjectID = a_ObjectID;

    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
}
