#include "Globals.h"
#include "ModuleShaders.h"


ModuleShaders::ModuleShaders( bool active ) : Module( active ){}
ModuleShaders::~ModuleShaders() {}

void ModuleShaders::Load(const char* name, const char* vertex_shader, const char* fragment_shader){}
void ModuleShaders::Clear() {}

int	ModuleShaders::GetUniformLocation(const char* name, const char* uniform) {
	return 0;
}
void ModuleShaders::UseProgram(const char* name) {}
void ModuleShaders::UnuseProgram() {}