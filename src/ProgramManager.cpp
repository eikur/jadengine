#include "Globals.h"
#include "glew-2.0.0\include\GL\glew.h"
#include "ProgramManager.h"

ProgramManager::ProgramManager(){}
ProgramManager::~ProgramManager() {}

GLuint ProgramManager::Load(const char* name, const char* vertex_shader_name, const char* fragment_shader_name)
{
	// GLSL shader examples in www.khronos.org/opengl/wiki/Example_Code
	std::map<std::string, unsigned, LessString>::iterator it;

	it = programs.find(name);
	if (it != programs.end())
		return (*it).second;

	GLuint vertex_shader;
	GLuint fragment_shader;
	FILE* fp;
	long size;
	char *buffer;

	// vertex shader load
	fp = fopen(vertex_shader_name, "r");
	if (fp == nullptr){
		MYLOG("Error opening file: %s\n", strerror(errno));
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	buffer = (char*) malloc(size + 1);

	rewind(fp);
	fread(buffer, size, 1, fp);
	buffer[size] = '\0';
	
	// compile vertex shader
	fclose(fp);

	// fragment shader load
	fp = fopen(fragment_shader_name, "r");
	if (fp == nullptr) {
		MYLOG("Error opening file: %s\n", strerror(errno));
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	buffer = (char*)malloc(size + 1);

	rewind(fp);
	fread(buffer, size, 1, fp);
	buffer[size] = '\0';

	// compile fragment shader
	fclose(fp);



	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram( program );
	

	free(buffer);

}
void ProgramManager::Clear() {}

int	ProgramManager::GetUniformLocation(const char* name, const char* uniform) {
	return 0;
}
void ProgramManager::UseProgram(const char* name) {}
void ProgramManager::UnuseProgram() {}

ProgramManager* ProgramManager::GetInstance()
{
	return nullptr;
}