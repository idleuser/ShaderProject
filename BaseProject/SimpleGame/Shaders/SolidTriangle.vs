#version 330

uniform float u_Time;
in vec3 a_Local;
in vec3 a_Position;
in float a_Size;
in float a_Mass;
in vec2 a_Vel;

const float C_PI = 3.141592;
const float C_Gravity = -9.81;

void sin1()
{
	vec4 newPosition;
	float time = u_Time * 0.2;

    newPosition.x = a_Position.x + time;
	newPosition.y = a_Position.y + 0.5 * sin(time * 2 * C_PI);
	newPosition.z = 0;
	newPosition.w = 1;

	gl_Position = newPosition;
}

void Falling()
{
	float t = mod(u_Time * 0.2, 1.0);
	float tt = t * t;
	float vx, vy;	// 초기 속도

	vx = a_Vel.x;
	vy = a_Vel.y;

	vec4 newPos;
	newPos.x = a_Position.x + vx * t;
	newPos.y = a_Position.y + vy * t + 0.5 * C_Gravity * tt;
	newPos.z = 0;
	newPos.w = 1;

	gl_Position = newPos;
}

void ParticlesFalling()
{
	float t = mod(u_Time * 0.2, 1.0);
	float tt = t * t;
	float vx, vy;	// 초기 속도

	vx = a_Vel.x;
	vy = a_Vel.y;
	vec3 scaledPos = a_Local * a_Size;

	vec4 newPos;
	newPos.x = (scaledPos.x + a_Position.x) + (vx * t);
	newPos.y = (scaledPos.y + a_Position.y) + (vy * t + 0.5 * C_Gravity * tt);
	newPos.z = 0;
	newPos.w = 1;

	gl_Position = newPos;
}

void main()
{
	ParticlesFalling();
}