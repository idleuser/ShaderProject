#version 330

uniform float u_Time;
in vec3 a_Local;		// 삼각형 지역좌표
in vec3 a_Position;
in float a_Size;
in float a_Mass;
in vec2 a_Vel;
in float a_RV;
in float a_RV2;
in float a_RV3;

out float v_Gray;

const float C_PI = 3.141592;
const float C_Gravity = -9.81;

// 0.0 ~ 1.0 사이의 랜덤값을 반환하는 함수
float hash(float n) {
    return fract(sin(n) * 43758.5453123);
}

void Sin()
{
	vec4 newPos = vec4(0.0, 0.0, 0.0, 0.0);
	float startTime = a_RV * 2;
	float newTime = u_Time * 0.5 - startTime;

	if(mod(newTime, 1.0) > 0.5){
		float t = mod(newTime * 2, 1.0);
		float amp = (1 - t) * 0.5 * (a_RV2 - 0.5) * 2;
		float period = a_RV3;

		vec3 scaledPos = a_Local * a_Size * 0.2;

		newPos.x = (scaledPos.x + a_Position.x) + t;
		newPos.y = (scaledPos.y + a_Position.y) + amp * sin(t * 2 * C_PI * period);
		newPos.z = 0;
		newPos.w = 1;
		v_Gray = 1-t;
	}
	else{
		float lifeScale = 2.0;
		float lifeTime = 0.5 + a_RV2 * lifeScale;
		float t = lifeTime * fract(newTime/lifeTime);		// 0~lifeTime 구간 반복
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
		v_Gray = 0;
	}

	gl_Position = newPos;
}

void ParticlesFalling()
{
	vec4 newPos = vec4(0.0, 0.0, 0.0, 0.0);
	float startTime = hash(a_RV) * 3;
	float newTime = u_Time * 0.5 - startTime;

	if(newTime > 0) {
		float lifeScale = 2.0;
		float lifeTime = 0.5 + a_RV2 * lifeScale;
		float t = lifeTime * fract(newTime/lifeTime);		// 0~lifeTime 구간 반복
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
	Sin();
}