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
	GLint size;
	char *buffer;
	GLint result;

	GLuint program = glCreateProgram();

	if (vertex_shader_name != nullptr)
	{
		fp = fopen(vertex_shader_name, "r");
		if (fp == nullptr){
			MYLOG("Error opening file: %s\n", strerror(errno));
		}
		else
		{
			fseek(fp, 0, SEEK_END);
			size = ftell(fp) + 1; 
			buffer = (char*)malloc(size);
			memset(buffer, 0, size);
			rewind(fp);
			fread(buffer, size, 1, fp);
			buffer[size - 1] = '\0';
			
			fclose(fp);

			vertex_shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex_shader, 1, &buffer, &size);
			glCompileShader(vertex_shader);
			free(buffer);


			result = 0;
			glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				// error display management
				GLint maxLength = 0;
				glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(vertex_shader, maxLength, &maxLength, &errorLog[0]);
				PrintErrorLog("Vertex Shader Compilation Failed", errorLog);
			}
			else
			{
				glAttachShader(program, vertex_shader);
			}
			glDeleteShader(vertex_shader);
		}
	}

	if (fragment_shader_name != nullptr)
	{
		fp = fopen(fragment_shader_name, "r");
		if (fp == nullptr) {
			MYLOG("Error opening file: %s\n", strerror(errno));
		}
		else
		{
			fseek(fp, 0, SEEK_END);
			size = ftell(fp) + 1;
			buffer = (char*)malloc(size);
			memset(buffer, 0, size);

			rewind(fp);
			fread(buffer, size-1, 1, fp);
			buffer[size-1] = '\0';
			fclose(fp);

			fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment_shader, 1, &buffer, &size);
			glCompileShader(fragment_shader);
			free(buffer);

			result = 0;
			glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				// error display management
				GLint maxLength = 0;
				glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, &errorLog[0]);
				PrintErrorLog("Fragment Shader Compilation Failed", errorLog);
			}
			else
			{
				glAttachShader(program, fragment_shader);
			}
			glDeleteShader(fragment_shader);
		}
	}

	glLinkProgram( program );

	result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		//The program is useless now. So delete it.
		glDeleteProgram(program);

		//Provide the infolog in whatever manner you deem best.
		PrintErrorLog("Program link failed", infoLog);
		//Exit with failure.
		return -1;
	}
	else
	{
		programs[name] = program;
		return program;
	}
}


void ProgramManager::Clear() {}

int	ProgramManager::GetUniformLocation(const char* name, const char* uniform) {
	return 0;
}
void ProgramManager::UseProgram(const char* name) {
	std::map<std::string, unsigned, LessString>::iterator it;

	it = programs.find(name);
	if (it == programs.end())
		return;
	else
		glUseProgram((*it).second);
}


void ProgramManager::UnuseProgram() {
	glUseProgram(0);
}

ProgramManager* ProgramManager::GetInstance()
{
	return nullptr;
}

void ProgramManager::PrintErrorLog(const char* header, const std::vector<GLchar> &log_to_print ) const
{
	std::ofstream output; 
	//output.open(output_file, std::ios::out | std::ios::app);
	output.open(output_file, std::ios::out | std::ios::trunc);
	output << "------------------------------------" << std::endl;
	output << header << std::endl;
	output << "------------------------------------" << std::endl;
	for (std::vector<GLchar>::const_iterator it = log_to_print.cbegin(); it != log_to_print.cend(); ++it)
		output << (*it); 
	output << std::endl << std::endl;
	output.close(); 
}