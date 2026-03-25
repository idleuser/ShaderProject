#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	//m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_TriangleShader = CompileShaders("./Shaders/SolidTriangle.vs", "./Shaders/SolidTriangle.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	m_Initialized = true;
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	//float centerX = 0, centerY = 0;
	//float size = 0.1;
	//float mass = 1;
	//float vx = 1, vy = 3;
	//float triangle[]
	//	=
	//{	// triangle1
	//	centerX + size / 2, centerY - size / 2, 0, 
	//	mass, vx, vy,
	//	centerX - size / 2, centerY - size / 2, 0, 
	//	mass, vx, vy,
	//	centerX + size / 2, centerY + size / 2, 0, 
	//	mass, vx, vy,
	//	// triangle2
	//	centerX - size / 2, centerY - size / 2, 0, 
	//	mass, vx, vy,
	//	centerX + size / 2, centerY + size / 2, 0, 
	//	mass, vx, vy,
	//	centerX - size / 2, centerY + size / 2, 0, 
	//	mass, vx, vy
	//};

	float halfSize = 0.1;
	float triangle[]
		=
	{	// triangle1
		halfSize, -halfSize, 0,
		-halfSize, -halfSize, 0,
		halfSize, halfSize, 0,

		// triangle2
		-halfSize, -halfSize, 0,
		halfSize, halfSize, 0,
		-halfSize, halfSize, 0,
	};

	glGenBuffers(1, &m_VBOTriangle);											// 버퍼 생성
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);								// 데이터 바인딩
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);	// gpu메모리에 데이터를 할당


	glGenVertexArrays(1, &m_VAOParticles);
	glBindVertexArray(m_VAOParticles);		// 배열에 바인딩

	// 1. 파티클 틀 설정 - m_VBOTriangle 연결
	// 파티클 1개 = 정점 6개
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);
	int attribLocal = glGetAttribLocation(m_TriangleShader, "a_Local");
	glEnableVertexAttribArray(attribLocal);
	glVertexAttribPointer(attribLocal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(attribLocal, 0);

	// 2. 인스턴스 데이터 설정 - m_VBOParticle 연결
	// 데이터 - 위치, 크기, 질량, 속도, RV, RV2, RV3
	glGenBuffers(1, &m_VBOParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	int attribSize = glGetAttribLocation(m_TriangleShader, "a_Size");
	glEnableVertexAttribArray(attribSize);
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	glEnableVertexAttribArray(attribMass);
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");
	glEnableVertexAttribArray(attribVel);
	int attribRV = glGetAttribLocation(m_TriangleShader, "a_RV");
	glEnableVertexAttribArray(attribRV);
	int attribRV2 = glGetAttribLocation(m_TriangleShader, "a_RV2");
	glEnableVertexAttribArray(attribRV2);
	int attribRV3 = glGetAttribLocation(m_TriangleShader, "a_RV3");
	glEnableVertexAttribArray(attribRV3);

	int stride = sizeof(float) * 10;

	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glVertexAttribDivisor(attribPosition, 1);
	glVertexAttribPointer(attribSize, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribDivisor(attribSize, 1);
	glVertexAttribPointer(attribMass, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 4));
	glVertexAttribDivisor(attribMass, 1);
	glVertexAttribPointer(attribVel, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 5));
	glVertexAttribDivisor(attribVel, 1);
	glVertexAttribPointer(attribRV, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 7));
	glVertexAttribDivisor(attribRV, 1);
	glVertexAttribPointer(attribRV2, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 8));
	glVertexAttribDivisor(attribRV2, 1);
	glVertexAttribPointer(attribRV3, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 9));
	glVertexAttribDivisor(attribRV3, 1);

	glBindVertexArray(0);				// VAO 해제
	glBindBuffer(GL_ARRAY_BUFFER, 0);	// VBO 해제
}

void Renderer::GenParticles(int num)
{
	std::default_random_engine dre;
	std::uniform_real_distribution<float> urdPos(-0.3f, 0.3f);
	std::uniform_real_distribution<float> urdSize{ 0.01 , 0.1 };
	std::uniform_real_distribution<float> urdVx{ -3.0 , 3.0 };
	std::uniform_real_distribution<float> urdVy{ -2.0 , 6.0 };
	std::uniform_real_distribution<float> urdRV{ 0.0 , 1.0 };

	m_ParticleNum = num;
	std::vector<Particle> particles(m_ParticleNum);

	for (int i = 0; i < m_ParticleNum; ++i) {
		/*particles[i].pos[0] = urdPos(dre);
		particles[i].pos[1] = urdPos(dre);*/
		particles[i].pos[0] = 0.0f;
		particles[i].pos[1] = 0.0f;
		particles[i].pos[2] = 0.0f;

		particles[i].size = urdSize(dre);
		particles[i].mass = 1.0f;

		particles[i].vel[0] = urdVx(dre);
		particles[i].vel[1] = urdVy(dre);

		particles[i].rv = urdRV(dre);
		particles[i].rv2 = urdRV(dre);
		particles[i].rv3 = urdRV(dre);
	}

	// 버퍼에 데이터 업로드
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_STATIC_DRAW);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawSolidTriangle(float x, float y, float z, float size, float r, float g, float b, float a)
{
	static float g_time = 0;
	g_time += 0.001;

	//Program select
	glUseProgram(m_TriangleShader);

	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(uTime, g_time);

	glUniform4f(glGetUniformLocation(m_TriangleShader, "u_Trans"), 0, 0, 0, 1);
	glUniform4f(glGetUniformLocation(m_TriangleShader, "u_Color"), 1, 1, 1, 1);

	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	glEnableVertexAttribArray(attribMass);
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");
	glEnableVertexAttribArray(attribVel);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTriangle);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(attribMass, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(attribVel, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawParticles()
{
	static float g_time = 0;
	g_time += 0.001;

	glUseProgram(m_TriangleShader);

	glUniform1f(glGetUniformLocation(m_TriangleShader, "u_Time"), g_time);
	glUniform4f(glGetUniformLocation(m_TriangleShader, "u_Trans"), 0, 0, 0, 1);
	glUniform4f(glGetUniformLocation(m_TriangleShader, "u_Color"), 1, 1, 1, 1);

	glBindVertexArray(m_VAOParticles);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_ParticleNum);

	glBindVertexArray(0);
}

void Renderer::GetGLPosition(float x, float y, float* newX, float* newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}