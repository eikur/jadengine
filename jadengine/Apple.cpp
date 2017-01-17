#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include "Apple.h"


Apple::Apple() : Entity(Entity::Types::item_apple)
{

}

Apple::~Apple() {}

bool Apple::Init()
{
	if (LoadFromConfigFile(CONFIG_FILE) == false)
	{
		LOG("Error loading apple config from file");
		return false;
	}
	UpdateCurrentAnimation(&idle);
	return true;
}

bool Apple::Update(unsigned int, const bool)
{
	if (IsAlive() == false)
	{
		RemoveColliders();
		CleanUp();
		return 0;
	}
	return true;
}

void Apple::DecreaseHealth(int amount)
{
	health = MAX(health - amount, 0);
}
// ------------ Load config from file ---------
bool Apple::LoadFromConfigFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);
	
	if (json_object_dothas_value_of_type(root_object, "item_apple.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(root_object, "item_apple.graphics_file"));
	if (graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

	max_health = (int)json_object_dotget_number(root_object, "item_apple.max_health");
	health = max_health;
	LoadiPointFromJSONObject(root_object, "item_apple.sprite_offset", &sprite_offset);
	hit_collider = LoadColliderFromJSONObject(root_object, "item_apple.colliders.hit", colliderType::ITEMS, &hit_collider_offset);
	LoadAnimationFromJSONObject(root_object, "item_apple.idle", &idle);

	json_value_free(root_value);

	return true;
}