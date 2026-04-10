#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <random>

#include "Dependencies\glew.h"

struct Particle
{
	float pos[3];
	float size;
	float mass;
	float vel[2];
	float rv, rv2, rv3;
};

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void DrawSolidTriangle();
	void GenParticles(int particleNum);
	void DrawParticles();
	void DrawFS();
private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	GLuint m_VBOTriangle = 0;
	GLuint m_TriangleShader = 0;

	GLuint m_ParticleNum = 1;
	GLuint m_VBOParticle = 0;
	GLuint m_VAOParticles = 0;

	// fragment shader
	GLuint m_VBOFS = 0;
	GLuint m_FSShader = 0;

	float m_DropPoints[1000 * 4];
};

