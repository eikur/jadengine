#include "Component.h"

Component::Component(GameObject* parent, componentType type, bool active) : parent(parent), type(type), active(active)
{

}

Component::~Component() {}

bool Component::Update(float dt) { return true; }

bool Component::CleanUp() { return true; }

bool Component::Init() { return true; }

bool Component::DebugDraw() { return true; }