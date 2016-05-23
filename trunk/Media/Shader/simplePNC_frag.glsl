#version 410 compatibility

in vec4 vs_fs_color;
in vec3 vs_fs_normal;
in vec4 vs_fs_vertex;


void main(void)
{
   vec3 L =normalize(gl_LightSource[0].position.xyz - vs_fs_vertex.xyz);   
   vec4 Idiff = vs_fs_color * max(dot(vs_fs_normal,L), 0.1); 
   Idiff = clamp(Idiff, 0.0, 1.0); 

   gl_FragColor = Idiff;  
}
