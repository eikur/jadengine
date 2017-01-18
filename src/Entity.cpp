#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleTextures.h"

#include "Entity.h"

Entity::Entity(Types type) : m_type(type)
{}

Entity::~Entity()
{}

bool Entity::Init()
{
	return true;
}

// ----------------- Updates ------------------------

bool Entity::Update(unsigned int, const bool)
{
	return true;
}

void Entity::UpdatePosition(const iPoint new_speed)
{
	if (grounded)
		ground_y = position.y;

	position += new_speed;

	if (attack_collider != nullptr)
	{
		if (facing_right)
			attack_collider->rect.x = position.x + attack_collider_offset.x;
		else
			attack_collider->rect.x = position.x + -(attack_collider_offset.x + attack_collider->rect.w);
		attack_collider->rect.y = position.y + attack_collider_offset.y;
	}
	if (hit_collider != nullptr)
	{
		if (facing_right)
			hit_collider->rect.x = position.x + hit_collider_offset.x;
		else
			hit_collider->rect.x = position.x - (hit_collider_offset.x + hit_collider->rect.w);
		hit_collider->rect.y = position.y + hit_collider_offset.y;
	}

	if (grounded)
		ground_y = position.y;
	
}

void Entity::SetPosition(const iPoint new_position)
{
	position = new_position;

	if (grounded)
		ground_y = position.y;


	if (facing_right)
	{
		if (attack_collider != nullptr)
			attack_collider->rect.x = position.x + attack_collider_offset.x;
		hit_collider->rect.x = position.x + hit_collider_offset.x;
	}
	else
	{
		if (attack_collider != nullptr)
			attack_collider->rect.x = position.x + -(attack_collider_offset.x + attack_collider->rect.w);
		hit_collider->rect.x = position.x - (hit_collider_offset.x + hit_collider->rect.w);
	}
	if (attack_collider != nullptr)
		attack_collider->rect.y = position.y + attack_collider_offset.y;
	hit_collider->rect.y = position.y + hit_collider_offset.y;
}

bool Entity::Draw() const
{
	if (facing_right)
	{
		App->renderer->Blit(graphics, position.x + sprite_offset.x, position.y + sprite_offset.y, &(current_animation->GetCurrentFrame()), 1.0F, false);
		if (grounded == false)
			App->renderer->Blit(graphics, position.x + sprite_offset.x, ground_y + sprite_offset.y, &shadow, 1.0f, false);
	}
	else
	{
		App->renderer->Blit(graphics, position.x + sprite_offset_flip.x, position.y + sprite_offset_flip.y, &(current_animation->GetCurrentFrame()), 1.0F, true);
		if (grounded == false)
			App->renderer->Blit(graphics, position.x + sprite_offset_flip.x, ground_y + sprite_offset_flip.y, &shadow, 1.0f, true);
	}
	return true;
}

bool Entity::AllowAnimationInterruption() const
{
	return blocking_animation_remaining_msec <= 0;
}

void Entity::UpdateCurrentAnimation(Animation *new_anim, int block_anim_duration, int fx_new_anim, bool override_current)
{
	if (current_animation != new_anim || override_current)
	{
		current_animation = new_anim;
		current_animation->Reset();
		if (fx_new_anim != -1)
			App->audio->PlayFx(fx_new_anim);
		blocking_animation_remaining_msec = block_anim_duration;

		// grounded update
		if (current_animation == &jump ||
			current_animation == &jump_attack ||
			current_animation == &being_thrown_front ||
			current_animation == &being_knocked ||
			current_animation == &holding_swap)
			grounded = false;
		else
			grounded = true;

		//update jumping status
		if (current_animation == &jump || current_animation == &jump_attack)
			jumping = true;
		else
			jumping = false;

		// update the hittable status!
		if (current_animation == &being_hit ||
			current_animation == &being_thrown_front ||
			current_animation == &being_thrown_back ||
			current_animation == &being_knocked ||
			current_animation == &throwing_front ||
			current_animation == &throwing_back ||
			current_animation == &standing_up ||
		//	current_animation == &jump ||
			current_animation == &jump_attack ||
			current_animation == &holding_swap
			)
			is_hittable = false;
		else
			is_hittable = true;

		// update the attacking status
		if (current_animation == &attack1 ||
			current_animation == &attack2 ||
			current_animation == &attack3 ||
			//current_animation == &attack_back ||
			current_animation == &jump_attack )
			is_attacking = true;
		else
			is_attacking = false;

		// holding status
		if (current_animation == &holding_front || current_animation == &holding_front_attack || current_animation == &holding_front_attack2 ||
			(current_animation == &holding_swap && is_holding_front))
			is_holding_front = true;
		else
			is_holding_front = false;

		if (current_animation == &holding_back || (current_animation == &holding_swap && is_holding_back))
			is_holding_back = true;
		else
			is_holding_back = false;

		if (current_animation == &being_hold_back)
			is_being_hold_back = true;
		else
			is_being_hold_back = false;

		if (current_animation == &being_hold_front || current_animation == &being_hold_front_hit)
			is_being_hold_front = true;
		else
			is_being_hold_front = false;

		// being thrown status 
		is_being_thrown_back = current_animation == &being_thrown_back;
		is_being_thrown_front = current_animation == &being_thrown_front;
	}
}

iPoint Entity::UpdateKnockedMotion()
{
	int divisor = being_knocked_duration / 7;
	int frames_left = air_remaining_msec / divisor;
	iPoint ret = { 0,0 };

	int mod = facing_right ? 1 : -1;
	switch (frames_left)
	{
	case 6: ret.x -= mod * 5; ret.y -= 7; being_knocked.SetCurrentFrame(0); break;
	case 5: ret.x -= mod * 5; ret.y -= 4; being_knocked.SetCurrentFrame(0); break;
	case 4: ret.x -= mod * 4; ret.y -= 3; being_knocked.SetCurrentFrame(0); break;
	case 3: ret.x -= mod * 4; ret.y += 4; being_knocked.SetCurrentFrame(0); break;
	case 2: ret.x -= mod * 4; ret.y += 3; being_knocked.SetCurrentFrame(1); break;
	case 1: ret.x -= mod * 3; ret.y += 3; being_knocked.SetCurrentFrame(1); break;
	case 0: ret.x -= mod * 2; ret.y += 2; being_knocked.SetCurrentFrame(1); break;
	}
	return ret;
}

iPoint Entity::UpdateThrownFrontMotion()
{
	iPoint ret = { 0,0 };
	int divisor = being_thrown_duration / 8;
	int frames_left = air_remaining_msec / divisor;

	int mod = facing_right ? 1 : -1;

	being_thrown_front.SetCurrentFrame(7 - frames_left);
	
	switch (frames_left)
	{
	case 7: SetPosition({ pivot.x - mod * 14, pivot.y });   break;
	case 6: SetPosition({ pivot.x + mod * 34, pivot.y - 30 });  break;
	case 5: ret.x += mod * 5; ret.y = -1;     break;
	case 4:  ret.x += mod * 4; ret.y += 2;   break;
	case 3:  ret.x += mod * 4; ret.y += 3; break;
	case 2:  ret.x += mod * 3; ret.y += 3;  break;
	case 1:  ret.x += mod * 3; ret.y -= 3;  break;
	case 0:  ret.x += mod * 2; ret.y += 3;  break;
	}

	return ret;
}

void Entity::UpdateThrownBackMotion() {
	
	int divisor = being_thrown_duration / 8;
	int frames_left = blocking_animation_remaining_msec / divisor;

	int mod = facing_right ? 1 : -1;
	
	being_thrown_back.SetCurrentFrame(7 - frames_left);
	switch (frames_left)
	{
	case 7: SetPosition({ pivot.x + mod * 16, pivot.y }); break;
	case 6: SetPosition({ pivot.x + mod * 8, pivot.y - 30 });   break;
	case 5: SetPosition({ pivot.x - mod * 0, pivot.y - 22 });  break;	
	case 4: SetPosition({ pivot.x - mod * 30, pivot.y });   break;
	case 3: grounded = false; SetPosition({ pivot.x - mod * 40, pivot.y - 3 });  break;
	case 2: SetPosition({ pivot.x - mod * 50, pivot.y -5});   break;
	case 1: SetPosition({ pivot.x - mod * 55, pivot.y - 3 }); break;
	case 0: grounded = true; SetPosition({ pivot.x - mod * 60, pivot.y }); break;
	}
}

// ----------------- Depth related ------------------------
int Entity::GetDepth() const {
	return ground_y;
}

void Entity::SetDepth(int new_depth) {
	int diff = new_depth - ground_y;
	UpdatePosition({ 0,diff });
}

bool Entity::IsGrounded() const 
{
	return grounded;
}

//---------------------- Health related------------------------------

bool Entity::IsAlive() const
{
	return health > 0;
}

void Entity::IncreaseHealth(int amount) {
	health = MIN(health + amount, max_health);
}

void Entity::DecreaseHealth(int amount) {
	health = MAX(health - amount, 0);
	if (IsAlive() == false)
	{
		if (current_animation == &being_thrown_back || current_animation == &being_thrown_front)
			UpdateCurrentAnimation(&dying, dying_duration, fx_death, true);
		else
		{
			air_remaining_msec = being_knocked_duration;
			UpdateCurrentAnimation(&being_knocked, being_knocked_duration, fx_death, true);
		}
	}
}
void Entity::AddScore(int)
{}

void Entity::TimeOver()
{
	if (held_entity != nullptr)
	{
		held_entity->SetIdle();
		held_entity = nullptr;
	}
	SetBeingKnocked(max_health);
}

void Entity::IncreaseHelp(int)
{}

void Entity::RemoveColliders()
{
	if (hit_collider != nullptr)
	{
		hit_collider->to_delete = true;
		hit_collider = nullptr;
	}
	if (attack_collider != nullptr)
	{
		attack_collider->to_delete = true;
		attack_collider = nullptr;
	}
}
void Entity::CleanUp()
{
	if (graphics != nullptr)
		App->textures->Unload(graphics);
}



void Entity::UpdateAIState(AIState new_state)
{
	UpdateAIDestinationPoint( new_state );
	state = new_state;
}

void Entity::UpdateAIDestinationPoint(AIState )
{

}

//-------------------    Interaction between entities ---------------------------------

void Entity::SetIdle()
{
	UpdateCurrentAnimation(&idle);
}

void Entity::SetReachableItem(Entity* reachable)
{
	if (reachable != nullptr)
		reachable_item = reachable;
}

void Entity::SetBeingHit(int damage){
	is_hittable = false;
	unhittable_remaining_msec = unhittable_max_msec;
	UpdateCurrentAnimation(&being_hit, being_hit_duration, -1, true);
	if (held_entity != nullptr)
	{
		held_entity->SetIdle();
		held_entity = nullptr;
	}
	DecreaseHealth(damage);
}

void Entity::SetBeingHoldFrontHit(int damage) {
	is_hittable = false;   
	unhittable_remaining_msec = unhittable_max_msec;
	UpdateCurrentAnimation(&being_hold_front_hit, being_hit_duration, -1, true);
	DecreaseHealth(damage);
}

void Entity::SetHoldingFront(Entity* held)
{
	held_entity = held;
	UpdateCurrentAnimation(&holding_front);
}

void Entity::SetHoldingBack(Entity* held)
{
	held_entity = held;
	UpdateCurrentAnimation(&holding_back);
}

void Entity::SetBeingHoldFront()
{
	UpdateCurrentAnimation(&being_hold_front);
}

void Entity::SetBeingHoldBack()
{
	UpdateCurrentAnimation(&being_hold_back);
}

void Entity::SetBeingThrownFront( iPoint pvt) {
	is_hittable = false;
	air_remaining_msec = being_thrown_duration;
	pivot = pvt;
	UpdateCurrentAnimation(&being_thrown_front, being_thrown_duration);
}
void Entity::SetBeingThrownBack( iPoint pvt) {
	is_hittable = false;
	pivot = pvt;
	UpdateCurrentAnimation(&being_thrown_back, being_thrown_duration);
}

void Entity::SetBeingKnocked(int damage)
{
	is_hittable = false;
	if (held_entity != nullptr)
	{
		held_entity->SetIdle();
		held_entity = nullptr;
	}
	air_remaining_msec = being_knocked_duration;
	UpdateCurrentAnimation(&being_knocked, being_knocked_duration, -1, true);
	DecreaseHealth(damage);
}

bool Entity::IsHoldingSomeone()
{
	return held_entity != nullptr;
}

//------------------------- JSON LOAD ---------------------------
bool Entity::LoadFromConfigFile(const char* ) { 
	return true; 
}

void Entity::LoadAnimationFromJSONObject(JSON_Object* j_object, const char *dotget_path, Animation* animation)
{
	JSON_Array *j_array, *j_array_inner;
	std::string tmp = dotget_path;

	tmp.append(".speed");
	animation->speed = (float)json_object_dotget_number(j_object, tmp.c_str());
	
	tmp = dotget_path;
	tmp.append(".frames");
	j_array = json_object_dotget_array(j_object, tmp.c_str());
	for (int i = 0; i < (int)json_array_get_count(j_array); ++i)
	{
		j_array_inner = json_array_get_array(j_array, i);
		animation->frames.push_back({ (int)json_array_get_number(j_array_inner, 0), (int)json_array_get_number(j_array_inner, 1), (int)json_array_get_number(j_array_inner, 2), (int)json_array_get_number(j_array_inner, 3) });
		json_array_clear(j_array_inner);
	}
}

void Entity::LoadSDLRectFromJSONObject(JSON_Object* j_object, const char *dotget_path, SDL_Rect *rect) 
{
	JSON_Array *j_array;
	j_array = json_object_dotget_array(j_object, dotget_path);
	*rect = { (int)json_array_get_number(j_array,0), (int)json_array_get_number(j_array,1), (int)json_array_get_number(j_array,2), (int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);
}

void Entity::LoadSoundFXFromJSONObject(JSON_Object* j_object, const char *dotget_path, unsigned int *fx)
{
	if (json_object_dothas_value_of_type(j_object, dotget_path, JSONString))
		*fx = App->audio->LoadFx(json_object_dotget_string(j_object, dotget_path));
}

void Entity::LoadiPointFromJSONObject(JSON_Object* j_object, const char *dotget_path, iPoint *point)
{
	JSON_Array *j_array;
	j_array = json_object_dotget_array(j_object, dotget_path);
	point->x = (int)json_array_get_number(j_array, 0);
	point->y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);
}

Collider* Entity::LoadColliderFromJSONObject(JSON_Object* j_object, const char *dotget_path, colliderType type, iPoint *offset)
{
	JSON_Array *j_array;
	Collider* ret;
	j_array = json_object_dotget_array(j_object, dotget_path);
	*offset = { (int)json_array_get_number(j_array, 0), (int)json_array_get_number(j_array, 1) };
	ret = App->collision->AddCollider(
	{ offset->x + position.x, offset->y + position.y, (int)json_array_get_number(j_array, 2) , (int)json_array_get_number(j_array, 3) },
		type, (Entity*) this);
	json_array_clear(j_array);
	return ret;
}
