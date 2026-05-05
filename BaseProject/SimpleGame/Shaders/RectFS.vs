#version 330

in vec3 a_Pos;
in vec2 a_Tex;
uniform float u_Time;

out vec2 v_Tex;

void main()
{
	v_Tex = a_Tex;

	vec4 newPos = vec4(a_Pos, 1.0);
	gl_Position = newPos;
}