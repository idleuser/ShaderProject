#version 330

in vec3 a_Pos;
in vec2 a_TPos;
uniform float u_Time;

out vec2 v_TPos;

void main()
{
	vec4 newPos = vec4(a_Pos, 1.0);

	v_TPos = a_TPos;

	gl_Position = newPos;
}