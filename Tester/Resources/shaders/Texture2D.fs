#version 450 core
			
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

uniform float u_TilingFactor;

void main()
{
    // color = vec4(v_TexCoord, 0.0, 1.0); -- UV Viz
    color = texture(u_Texture, v_TexCoord) * u_Color;
}
