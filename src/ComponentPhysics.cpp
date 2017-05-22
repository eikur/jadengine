#include "ImGui/imgui.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "GameObject.h"

#include "ComponentPhysics.h"

ComponentPhysics::ComponentPhysics(GameObject* parent, bool active) : Component(parent, PHYSICS, active), btMotionState()
{
	collider = App->physics->AddBox(1, this);
}

ComponentPhysics::~ComponentPhysics() {}

void ComponentPhysics::OnEditor()
{
	if (ImGui::CollapsingHeader("Physics Component"))
	{
		ImGui::Text("Here will be shown the shape, and size / radius of the primitive");
		
	}

}

void ComponentPhysics::getWorldTransform(btTransform &worldTrans) const
{
	worldTrans.setFromOpenGLMatrix(parent->GetWorldTransformMatrix().Transposed().ptr());
}



void ComponentPhysics::setWorldTransform(const btTransform &worldTrans) 
{
	btVector3 in_pos = worldTrans.getOrigin();
	btQuaternion in_rot = worldTrans.getRotation();

	float3 pos(in_pos.getX(), in_pos.getY(), in_pos.getZ());
	Quat rot(in_rot.getX(), in_rot.getY(), in_rot.getZ(), in_rot.getW());
	parent->SetLocalTransform(pos, rot);

}

void ComponentPhysics::UpdateRigidbodyTransform() {
	collider->setMotionState(this);
}

