#version 330

layout(location=0) out vec4 FragColor;
in vec2 v_TPos;
uniform float u_Time;

const float C_PI = 3.141592;

void LinePattern()
{
	//FragColor = vec4(v_TPos, 0, 1);
	float lineCount = 10;
	float lineWidth = 1;
	float per = 0.5 * C_PI;

	lineCount = lineCount/2;
	lineWidth = 50 / lineWidth;

	float gray1 = pow(abs(sin((v_TPos.x * 2 * C_PI - per) * lineCount)), lineWidth);
	float gray2 = pow(abs(sin((v_TPos.y * 2 * C_PI - per) * lineCount)), lineWidth);
	FragColor = vec4(gray1+ gray2);
}

void CirclePattern()
{
	float radius = 0.5;
	vec2 center = vec2(0.5, 0.5);
	vec2 curPos = v_TPos.xy;
	float lineWidth = 0.005;
	float dist = distance(center, curPos);

	if(dist > radius - lineWidth && dist < radius + lineWidth){
		FragColor = vec4(v_TPos, 0, 1);
	}
	else{
		FragColor = vec4(0);
	}
}

void CircleSin()
{
	float radius = 0.5;
	float lineWidth = 1;
	lineWidth = 64 / lineWidth;
	vec2 center = vec2(0.5, 0.5);
	vec2 curPos = v_TPos.xy;
	float dist = distance(center, curPos);
	float value = abs(sin(dist * 8 * C_PI - u_Time*3));

	FragColor = vec4(pow(value, lineWidth));
}

void Frac()
{
    // 1. 좌표계를 -1.0 ~ 1.0 범위로 이동 (중앙이 0,0이 되도록)
    vec2 uv = (v_TPos - 0.5) * 2.0;
    vec2 uv0 = uv; // 초기 좌표 저장 (기하학적 구조 유지용)
    vec3 finalColor = vec3(0.0);

    // 2. 반복문을 통해 프랙탈 층(Layer)을 쌓음
    for (float i = 0.0; i < 4.0; i++) {
        // 공간을 소수점 단위로 반복 (fract) 시키고 다시 중앙 정렬
        uv = fract(uv * 1.5) - 0.5;

        // 원형 거리 계산
        float d = length(uv) * exp(-length(uv0));

        // CircleSin의 핵심 로직: sin을 이용한 링 패턴 생성
        // i를 더해 각 층마다 다른 색상과 타이밍을 가짐
        vec3 col = vec3(0.5 + 0.5 * cos(u_Time + uv0.xyx * 2.0 + i));
        
        d = sin(d * 8.0 + u_Time) / 8.0;
        d = abs(d);

        // 빛나는 선 효과 (CircleSin의 lineWidth와 유사한 원리)
        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }

    FragColor = vec4(finalColor, 1.0);
}

void main()
{
	Frac();
}
