#version 410 compatibility

uniform float alpha;

uniform int texsize;
uniform sampler2D tex0;

in vec2 vs_fs_texcel;
in vec4 vs_fs_color;
layout (location = 0) out vec4 OutColor;

void main(void)
{
/*
	vec4 FragColor; 
	if(texsize == 0)
		FragColor = vs_fs_color;
	else if(texsize == 1)
		FragColor = texture(tex0, vs_fs_texcel) * vs_fs_color;
*/

	vec4 FragColor = texture(tex0, vs_fs_texcel) * vs_fs_color;
	OutColor = FragColor * alpha;
}
