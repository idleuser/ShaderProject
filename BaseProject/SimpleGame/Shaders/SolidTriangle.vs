#version 330

uniform float u_Time;
in vec3 a_Position;

void sin1()
{
	vec4 newPosition;
	float time = u_Time * 0.2;

    newPosition.x = a_Position.x + time;
	newPosition.y = a_Position.y + 0.5 * sin(time * 2 * 3.141592);
	newPosition.z = 0;
	newPosition.w = 1;

	gl_Position = newPosition;
}


void main()
{
	sin1();
}