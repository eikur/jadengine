#include "ProgramManager.h"

ProgramManager::ProgramManager(){}
ProgramManager::~ProgramManager() {}

void ProgramManager::Load(const char* name, const char* vertex_shader, const char* fragment_shader){}

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