#ifndef __MODULE_SHADERS_H__
#define __MODULE_SHADERS_H__

#include <stdlib.h>
#include <vector>
#include <map>
#include <fstream>

class ProgramManager
{
	const char* output_file = "shader_log.txt";
	
	struct LessString
	{
		bool operator()(const std::string left, const std::string right) const
		{
			return ::strcmp(left.c_str(), right.c_str()) < 0;
		}
	};

	typedef std::map<std::string, unsigned, LessString> ProgramList;

	ProgramList programs;
	static std::auto_ptr<ProgramManager> instance;

public: 
	ProgramManager();
	~ProgramManager();

	GLuint Load(const char* name, const char* vertex_shader, const char* fragment_shader);
	void Clear(); 

	int	GetUniformLocation(const char* name, const char* uniform);
	void UseProgram(const char* name); 
	void UnuseProgram();

	static ProgramManager* GetInstance(); 

	void PrintErrorLog(const char* header, const std::vector<GLchar> &log_to_print) const;
};

#endif

