#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include "Chicken.h"


Chicken::Chicken() : Entity(Entity::Types::item_chicken)
{

}

Chicken::~Chicken() {}

bool Chicken::Init()
{
	if (LoadFromConfigFile(CONFIG_FILE) == false)
	{
		MYLOG("Error loading apple config from file");
		return false;
	}
	UpdateCurrentAnimation(&idle);
	return true;
}

bool Chicken::Update(unsigned int, const bool)
{
	if (IsAlive() == false)
	{
		RemoveColliders();
		CleanUp();
		return 0;
	}
	return true;
}

void Chicken::DecreaseHealth(int amount)
{
	health = MAX(health - amount, 0);
}
// ------------ Load config from file ---------
bool Chicken::LoadFromConfigFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	if (json_object_dothas_value_of_type(root_object, "item_chicken.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(root_object, "item_chicken.graphics_file"));
	if (graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

	max_health = (int)json_object_dotget_number(root_object, "item_chicken.max_health");
	health = max_health;
	LoadiPointFromJSONObject(root_object, "item_chicken.sprite_offset", &sprite_offset);
	hit_collider = LoadColliderFromJSONObject(root_object, "item_chicken.colliders.hit", colliderType::ITEMS, &hit_collider_offset);
	LoadAnimationFromJSONObject(root_object, "item_chicken.idle", &idle);

	json_value_free(root_value);

	return true;
}
