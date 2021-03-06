#include "Globals.h"

#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentAnimation.h"
#include "ComponentCamera.h"
#include "ComponentBillboard.h"
#include "ParticleSystem.h"
#include "ComponentPhysics.h"


GameObject::GameObject(const char* name, GameObject* parent, bool active) : name(name), parent(parent), active(active)
{

}
GameObject::~GameObject()
{

}

bool GameObject::Init()
{
	bool ret = true;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->active == true)
			ret = (*it)->Init();
	}

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if ((*it)->active == true)
			ret = (*it)->Init();
	}
	return ret;
}

bool GameObject::Update( float dt )
{
	bool ret = true; 

	glPushMatrix(); 
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->active == true)
			ret = (*it)->Update(dt);
	}
	
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if ((*it)->active == true)
			ret = (*it)->Update(dt);
	}
	glPopMatrix();
	return ret; 
}

bool GameObject::CleanUp()
{
	MYLOG("GameObject %s: Removing children GameObjects", name.c_str());
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}
	children.clear(); 

	MYLOG("GameObject %s: Removing components", name.c_str());
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->CleanUp(); 
		RELEASE(*it);
	}
	components.clear();

	return true; 
}

bool GameObject::DebugDraw()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->active == true)
			(*it)->DebugDraw();
	}

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if ((*it)->active == true)
			(*it)->DebugDraw();
	}

	return true;
}
void GameObject::DrawSkeleton(float3 color) const
{

	glColor3f(color.x, color.y, color.z);
	glPushMatrix();
	FindComponentByType(Component::componentType::TRANSFORM)->Update();

	for (std::vector<GameObject*>::const_iterator it = children.cbegin(); it != children.cend(); ++it)
	{
		glDisable(GL_LIGHTING);
		float3 line_end = (*it)->GetLocalPosition();
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(line_end.x, line_end.y, line_end.z);
		glEnd();
		glEnable(GL_LIGHTING);
		(*it)->DrawSkeleton();
	}

	glPopMatrix();

}

//-- GUI interface

void GameObject::OnEditor()
{
	ImGui::Checkbox(name.c_str(), &active );

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->OnEditor();
	}
}

int GameObject::OnHierarchy( int id, ImGuiTreeNodeFlags node_flags, GameObject *& selected_gameobject)
{
	ImGuiTreeNodeFlags flags = node_flags | (selected_gameobject == this ? ImGuiTreeNodeFlags_Selected : 0);

	if (children.empty() == true)	// leaf node
	{
		ImGui::TreeNodeEx((void*)(intptr_t)id, flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", name.c_str());
		if (ImGui::IsItemClicked())
			selected_gameobject = this; 
	}
	else    //parent node
	{
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, flags , "%s", name.c_str(), id);
		if (ImGui::IsItemClicked())
			selected_gameobject = this; 

		if (node_open)
		{
			for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
				id = (*it)->OnHierarchy(id+1, flags, selected_gameobject);
			ImGui::TreePop();
		}
	}
	return id;
}

void GameObject::AddGameObjectToChildren(GameObject* game_object)
{
	children.push_back(game_object); 
}

Component* GameObject::CreateComponent(Component::componentType type)
{
	static_assert(Component::componentType::UNKNOWN == 8, "Component class code needs update");
	Component* ret = nullptr;
	
	switch (type)
	{
	case Component::componentType::TRANSFORM: ret = new ComponentTransform(this, true); 
		transform = (ComponentTransform*) ret; 
		break;
	case Component::componentType::MESH: ret = new ComponentMesh(this, true); 
		mesh_component = (ComponentMesh*)ret;
		break;
	case Component::componentType::MATERIAL: ret = new ComponentMaterial(this, true);  break;
	case Component::componentType::ANIMATION: ret = new ComponentAnimation(this, true); break;
	case Component::componentType::CAMERA: ret = new ComponentCamera(this, true); 
		camera_component = (ComponentCamera*) ret;
		break;
	case Component::componentType::BILLBOARD: ret = new ComponentBillboard(this, true); break;
	case Component::componentType::PARTICLES: ret = new ParticleSystem(this, true); break;
		particle_system = (ParticleSystem*)ret;
	case Component::componentType::PHYSICS: ret = new ComponentPhysics(this, true); 
		physics_component = (ComponentPhysics*) ret;
		break; 
	case Component::componentType::UNKNOWN: 
	default:ret = nullptr;  break;
	}

	if (ret != nullptr)
	{
		components.push_back(ret);
	}
	return ret;
}

Component* GameObject::FindComponentByType(Component::componentType type) const
{
	for (std::vector<Component*>::const_iterator it = components.cbegin(); it != components.cend(); ++it)
	{
		if ((*it)->type == type)
			return *it;
	}
	return nullptr; 
}

void GameObject::SetLocalTransform( float3 new_pos, Quat new_rot, float3 new_scale)
{
	if (transform == nullptr)
	{
		transform = (ComponentTransform*) CreateComponent(Component::componentType::TRANSFORM);
	}

	transform->SetTransform(new_pos, new_rot, new_scale);
}

float3 GameObject::GetLocalPosition() const
{
	if (transform == nullptr)
	{
		return float3::zero;
	}
	return transform->position;
}

float3 GameObject::GetWorldPosition() const
{
	if (parent == nullptr)
		return GetLocalPosition(); 
	else
		return (GetWorldTransformMatrix().Mul(GetLocalPosition().ToPos4())).Float3Part();
}


const std::string& GameObject::GetName() const
{
	return name;
}

void GameObject::SetNextAnimationID( int next_id) 
{
	ComponentAnimation *anim = (ComponentAnimation*) FindComponentByType(Component::componentType::ANIMATION); 
	if (anim != nullptr)
		anim->SetNextAnimationInstanceID(next_id); 

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
		(*it)->SetNextAnimationID(next_id);

}

void GameObject::UpdateBoundingBoxesRecursively()
{
	if (mesh_component != nullptr)
		mesh_component->UpdateBoundingBox(GetWorldTransformMatrix()); 

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if ((*it)->active == true)
			(*it)->UpdateBoundingBoxesRecursively(); 
	}
}

AABB GameObject::GetBoundingBox()
{

	if (mesh_component != nullptr)
		return mesh_component->GetBoundingBox();
	AABB ret;
	ret.SetNegativeInfinity();
	return ret;
}

void GameObject::GetAllMeshComponents(std::vector<ComponentMesh*>& meshes) const
{
	if (mesh_component != nullptr)
		meshes.push_back(mesh_component);
	
	for (std::vector<GameObject*>::const_iterator it = children.cbegin(); it != children.cend(); ++it)
	{
		if ((*it)->active == true)
			(*it)->GetAllMeshComponents(meshes);
	}

}

float4x4 GameObject::GetModelSpaceTransformMatrix() const
{
	if (parent == nullptr)
		return float4x4::identity;
	else
		return parent->GetModelSpaceTransformMatrix() * GetLocalTransformMatrix(); 
}

float4x4 GameObject::GetWorldTransformMatrix() const
{
	if (parent == nullptr)
		return GetLocalTransformMatrix();
	else
		return parent->GetWorldTransformMatrix()*GetLocalTransformMatrix();
}

float4x4 GameObject::GetLocalTransformMatrix() const
{
	if (transform == nullptr)
		return float4x4::zero;
	return transform->GetTransform(); 
}

void GameObject::UpdateCameraWorldTransform() 
{
	if (camera_component != nullptr)
		camera_component->UpdateFrustumTransform(GetWorldTransformMatrix());

	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if ((*it)->active == true)
			(*it)->UpdateCameraWorldTransform();
	}
}

const std::vector<GameObject*>& GameObject::GetChildren() const {
	return children;
}

void GameObject::UpdatePhysicsBody() const
{
	if (physics_component == nullptr)
		return;
	physics_component->UpdateRigidbodyTransform(); 
}