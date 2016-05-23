#version 410 compatibility

uniform mat4 world_matrix;
uniform mat4 model_matrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

out vec4 vs_fs_color;
out vec3 vs_fs_normal;
out vec4 vs_fs_vertex;

void main(void)
{
    vs_fs_color = vec4(0.8,0,0, 1);

	vs_fs_normal = normal;
	vs_fs_vertex = world_matrix* model_matrix* vec4(position,1);	
	gl_Position =vs_fs_vertex;	
}
