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
		float mass = (collider->getInvMass() == 0) ? 0.0f : collider->getInvMass();
		btVector3 inertia = collider->getLocalInertia();
		float inertia_f[3] = { inertia.getX(), inertia.getY(), inertia.getZ() };

		ImGui::InputFloat("Mass", &mass);
		ImGui::DragFloat3("Inertia", inertia_f);

		inertia.setX(inertia_f[0]); inertia.setY(inertia_f[1]); inertia.setZ(inertia_f[2]);
		collider->setMassProps(mass == 0.0? mass : 1/mass, inertia);
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

