#version 410 compatibility

uniform int texsize;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

uniform int tex1blend;
uniform int tex2blend;
uniform int tex3blend;

in vec2 vs_fs_texcel;
in vec4 vs_fs_color;
layout (location = 0) out vec4 OutColor;

void Blend(in vec4 BaseColor, in vec4 BlendColor,in int BlendWay, out vec4 BlendedColor)
{
	if(BlendWay == 0)
		BlendedColor = BaseColor*BlendColor;
	else if(BlendWay == 1)
		BlendedColor = BaseColor + BlendColor;
	else if(BlendWay == 2)
		BlendedColor = min(BaseColor ,BlendColor);
	else if(BlendWay == 3)
		BlendedColor = max(BaseColor ,BlendColor);
	else if(BlendWay == 4)
		BlendedColor = BaseColor - BlendColor;
	else if(BlendWay == 5)
		BlendedColor = abs(BaseColor - BlendColor);
}


void main(void)
{
	vec4 BlendColor;
	vec4 BlendedColor;
	vec4 FragColor = texture(tex0, vs_fs_texcel) * vs_fs_color;

	if(texsize >=2)
	{	
		BlendColor = texture(tex1, vs_fs_texcel);
		Blend(FragColor, BlendColor, tex1blend, BlendedColor );
		FragColor = BlendedColor;
	}

	if(texsize >= 3)
	{
		BlendColor = texture(tex2, vs_fs_texcel);
		Blend(FragColor, BlendColor, tex2blend, BlendedColor );
		FragColor = BlendedColor;
	}

	if(texsize >= 4)
	{
		BlendColor = texture(tex3, vs_fs_texcel);
		Blend(FragColor, BlendColor, tex3blend, BlendedColor );
		FragColor = BlendedColor;
	}

	OutColor = FragColor;
}
