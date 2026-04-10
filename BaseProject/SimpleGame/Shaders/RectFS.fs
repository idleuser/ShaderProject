#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

uniform float u_Time;
uniform vec4 u_DropInfo[1000]; // vec4(x, y, startTime, lifeTime)

const float C_PI = 3.141592;

void LinePattern()
{
	//FragColor = vec4(v_Tex, 0, 1);
	float lineCount = 10;
	float lineWidth = 1;
	float per = 0.5 * C_PI;

	lineCount = lineCount/2;
	lineWidth = 50 / lineWidth;

	float gray1 = pow(abs(sin((v_Tex.x * 2 * C_PI - per) * lineCount)), lineWidth);
	float gray2 = pow(abs(sin((v_Tex.y * 2 * C_PI - per) * lineCount)), lineWidth);
	FragColor = vec4(gray1+ gray2);
}

void CirclePattern()
{
	float radius = 0.5;
	vec2 center = vec2(0.5, 0.5);
	vec2 curPos = v_Tex.xy;
	float lineWidth = 0.005;
	float dist = distance(center, curPos);

	if(dist > radius - lineWidth && dist < radius + lineWidth){
		FragColor = vec4(v_Tex, 0, 1);
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
	vec2 curPos = v_Tex.xy;
	float dist = distance(center, curPos);
	float value = abs(sin(dist * 8 * C_PI - u_Time*3));

	FragColor = vec4(pow(value, lineWidth));
}

// 멀어질 수록 진폭이 줄어든다.
void RainDrop()
{
	float accum = 0;

	for(int i = 0;i < 100;i++) {
		float lifeTime = u_DropInfo[i].w * 2;
		float startTime = u_DropInfo[i].z;
		float newTime = u_Time - startTime;

		if(newTime > 0){
			newTime = fract(newTime/lifeTime);

			float oneMinus = 1 - newTime;
			float t = newTime * lifeTime;

			float lineWidth = 4;
			lineWidth = 64 / lineWidth;

			vec2 center = u_DropInfo[i].xy;;
			vec2 curPos = v_Tex.xy;

			float range = t / 8;
			float dist = distance(center, curPos);
			float fade = 30 * clamp(range - dist, 0, 1);

			float value = pow(abs(sin(dist * 4 * C_PI * 10 - t*10)), lineWidth);	// (선두께(파장sin()))

			accum += value * fade * oneMinus;
		}
	}
	FragColor = vec4(accum * 0.4);
}

void Frac()
{
    // 1. 좌표계를 -1.0 ~ 1.0 범위로 이동 (중앙이 0,0이 되도록)
    vec2 uv = (v_Tex - 0.5) * 2.0;
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

// 1. 개별 파동 에너지를 반환하는 함수
float CalculateRipple(int i)
{
    float startTime = u_DropInfo[i].z;
    float lifeTime = u_DropInfo[i].w;
    float t = u_Time - startTime;

    if(t <= 0.0) return 0.0;

    // fract를 써서 반복시키되, 시간에 따른 감쇄(fade)를 조절
    float progress = fract(t / (lifeTime * 500));
    float dist = distance(u_DropInfo[i].xy, v_Tex.xy);
    
    // 파동의 확산 범위
    float waveEdge = progress * 0.2;
    float ring = abs(dist - waveEdge);
    
    // 파동의 날카로운 형태 (pow를 이용해 강조)
    float wave = pow(max(0.0, 1.0 - ring * 15.0), 5.0);
    
    // 사라지지 않게 하려면 1.0 - progress(fade)를 제거하거나 최소값을 부여
    float fade = (t/10 - progress) + 0.2; 

    return wave * fade;
}

// 2. 통합 프랙탈 함수
void FractalRain()
{
    float totalEnergy = 0.0;
    // 성능을 위해 상위 일부만 루프
    for(int i = 0; i < 50; i++) {
        totalEnergy += CalculateRipple(i);
    }

    vec2 uv = (v_Tex - 0.5) * 2.0;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

  // 파동이 칠 때만 1.5 + @가 되어 더 격렬하게 진동
        uv = fract(uv * (1.5 + totalEnergy * 0.3)) - 0.5;

        float d = exp(-length(uv0));
        //float d = length(uv) * exp(-length(uv0));


        // 색상: 시간에 따라 계속 변하게 설정 (u_Time)
        vec3 col = vec3(0.1, 0.4, 0.8) * (0.5 + 0.5 * cos(u_Time + uv0.xyx));
        
        // 파동 에너지를 sin 내부의 위상 변화로 사용
        d = sin(d * 8.0 + u_Time + totalEnergy * 5.0) / 8.0;
        d = abs(d);

        // 기본 밝기 0.01을 유지하여 파동이 없어도 형태가 보이게 함
        float brightness = 0.01 + (totalEnergy * 0.03);
        d = pow(brightness / d, 1.2);

        finalColor += col * d;

    FragColor = vec4(finalColor, 1.0);
}

void Flag()
{
    float amp = 0.2;
    float speed = 2 ;
    float freqBoost = 1 / (v_Tex.x + 0.3);
    float sinInput = v_Tex.x * 2 * C_PI - u_Time * speed;
	float sinVal = v_Tex.x * amp * freqBoost * sin(sinInput) + 0.5;
    float fWith = 0.5;
    float width = 0.5 * mix(1, fWith, v_Tex.x);
    float color = 0;


    if(v_Tex.y < sinVal + width/2 && v_Tex.y > sinVal - width/2)
    {
        color = 1;
    }
    else
    {
        color = 0;
    }

    FragColor = vec4(color);
}

void Smoke()
{
    float amp = 0.2;
    float speed = 2 ;
    float newY = 1 - v_Tex.y;
    float freqBoost = 1 / (newY + 0.3);
    float sinInput = newY * 2 * C_PI - u_Time * speed;
	float sinVal = newY * amp * freqBoost * sin(sinInput) + 0.5;
    float fWith = 0.1;
    float width = 0.5 * mix(fWith, 1, newY);
    float color = 0;


    if(v_Tex.x < sinVal + width/2 && v_Tex.x > sinVal - width/2)
    {
        color = 1;
    }
    else
    {
        color = 0;
    }

    FragColor = vec4(color);
}

void main()
{
    Smoke();
}
