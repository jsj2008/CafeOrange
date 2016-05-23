#version 410 compatibility


uniform mat4 world_matrix;
uniform mat4 model_matrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 vs_fs_color;

void main(void)
{
    vs_fs_color = vec4(color, 1);
	gl_Position = world_matrix* model_matrix* vec4(position,1);
}
