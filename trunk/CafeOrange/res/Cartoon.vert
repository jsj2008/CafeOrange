attribute vec4  EdgeColorVec; 
attribute float ThreshHold;
attribute float TextureSize;
//attribute float LightSize;

varying vec3  Normal;
varying vec3  Position;

varying vec4  EdgeColor;
varying float threshhold;
varying float textureSize;
//varying float Lights;
                    
void main()
{
   gl_Position = ftransform();
   gl_TexCoord[0]    = gl_TextureMatrix[0] * gl_MultiTexCoord0; 
   
   EdgeColor  = EdgeColorVec;
   threshhold = ThreshHold;
   textureSize= TextureSize;   
   Position   = vec3(gl_ModelViewMatrix * gl_Vertex);
   Normal     = normalize(gl_NormalMatrix * gl_Normal);//normalize(gl_Normal);
}
      