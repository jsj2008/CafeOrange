#version 410 compatibility

uniform int texsize;
uniform sampler2D tex0;

uniform float brightness;
uniform float contrast;

in vec2 vs_fs_texcel;
in vec4 vs_fs_color;
layout (location = 0) out vec4 OutColor;

void main(void)
{
	vec4 FragColor = texture(tex0, vs_fs_texcel) * vs_fs_color;

	FragColor *= contrast;
	FragColor = FragColor + vec4(brightness,brightness,brightness,0);

	OutColor = FragColor;
}
