#version 330

uniform float u_Time;
in vec3 a_Local;		// 삼각형 지역좌표
in vec3 a_Position;
in float a_Size;
in float a_Mass;
in vec2 a_Vel;
in float a_RV;
in float a_RV2;

const float C_PI = 3.141592;
const float C_Gravity = -9.81;

// 0.0 ~ 1.0 사이의 랜덤값을 반환하는 함수
float hash(float n) {
    return fract(sin(n) * 43758.5453123);
}

void ParticlesFalling()
{
	vec4 newPos;
	float startTime = hash(a_RV);
	float newTime = u_Time*0.5 - startTime;

	if(newTime > 0) {
		float t = mod(newTime, 1.0);
		float tt = t * t;
		float vx, vy;	// 초기 속도
		float angle;

		vx = a_Vel.x / 3;
		vy = a_Vel.y / 3;
		angle = a_RV * 2 * C_PI;
		vec3 scaledPos = a_Local * a_Size;

		newPos.x = (scaledPos.x + a_Position.x) + (vx * t) + sin(angle);
		newPos.y = (scaledPos.y + a_Position.y) + (vy * t + 0.5 * C_Gravity * tt) + cos(angle);
		newPos.z = 0;
		newPos.w = 1;
	}
	else {
		newPos = vec4(0.0, 0.0, 0.0, 0.0);
	}

	gl_Position = newPos;
}

void main()
{
	ParticlesFalling();
}