#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int o_IDBuffer;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;
in flat int v_ObjectID;

uniform sampler2D u_Textures[32];

void main()
{
    // color = vec4(v_TexCoord, 0.0, 1.0); -- UV Viz
    color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
    o_IDBuffer = v_ObjectID;
    // color = v_Color;
}
