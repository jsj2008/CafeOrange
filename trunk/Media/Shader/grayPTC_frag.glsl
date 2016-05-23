#version 410 compatibility

uniform int texsize;
uniform sampler2D tex0;
uniform int blendway;

in vec2 vs_fs_texcel;
in vec4 vs_fs_color;
layout (location = 0) out vec4 OutColor;


void Blend(in vec3 BaseColor, in int BlendWay, out float GrayColor)
{
   if(blendway == 0)
   {//average
      GrayColor = (BaseColor.r + BaseColor.g + BaseColor.b)/3.0f;
   }
   else if(blendway == 1)
   {//NTSC
      GrayColor =dot(BaseColor.rgb,vec3( 0.299, 0.587, 0.114) );
   }
   else if(blendway == 2)
   {//HSV
      GrayColor = dot(BaseColor.rgb,vec3( 0.2125, 0.7154, 0.0721) );
   }
   else if(blendway == 3)
   {//Desaturation
      GrayColor = (max(max(BaseColor.r,BaseColor.g),BaseColor.b) + min( min(BaseColor.r,BaseColor.g),BaseColor.b) ) /2.0f;
   }
   else if(blendway == 4)
   {//Decomposition-max
      GrayColor = max(max(BaseColor.r,BaseColor.g),BaseColor.b) ;
   }
   else if(blendway ==5)
   {//Decompositin-min
       GrayColor = min(min(BaseColor.r,BaseColor.g),BaseColor.b) ;
    }

}


void main(void)
{
   vec4 FragColor  = texture(tex0, vs_fs_texcel) * vs_fs_color;

   float GrayColor = 0.f;
   Blend(FragColor.rgb, blendway, GrayColor);
   OutColor = vec4(GrayColor,GrayColor,GrayColor,1.0f);
}
