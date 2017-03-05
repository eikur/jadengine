#include "TransformComponent.h"

TransformComponent::TransformComponent(GameObject* parent, Component::componentType type, bool active) : Component(parent, TRANSFORM, active)
{

}

TransformComponent::~TransformComponent()
{}

void TransformComponent::OnEditor()
{
	// show in editor window
}
