#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "EntityManager.h"

#include "Boss3.h"

Boss3::Boss3(Entity* target) : Entity(Entity::Types::npc_garcia), target(target)
{ }

Boss3::~Boss3() {}

bool Boss3::Init()
{
	if (LoadFromConfigFile(CONFIG_FILE) == false)
	{
		MYLOG("Error loading player config from file");
		return false;
	}
	UpdateCurrentAnimation(&idle);
	facing_right = false;
	current_combo_hits = 3;	// knocking attack 
	return true;
}

bool Boss3::Update(unsigned int msec_elapsed, const bool upd_logic)
{
	if (state == none && !position.IsZero())
		UpdateAIState(approach);

	if (blocking_animation_remaining_msec > 0)
		blocking_animation_remaining_msec = MAX(blocking_animation_remaining_msec - msec_elapsed, 0);
	if (air_remaining_msec > 0)
		air_remaining_msec = MAX(air_remaining_msec - msec_elapsed, 0);
	if (unhittable_remaining_msec > 0)
		unhittable_remaining_msec = MAX(unhittable_remaining_msec - msec_elapsed, 0);

	if (IsAlive() == false)
	{
		RemoveColliders();
		if (upd_logic && current_animation == &being_knocked)
			UpdatePosition(UpdateKnockedMotion());
		if (blocking_animation_remaining_msec <= 0 && current_animation != &dying)
			UpdateCurrentAnimation(&dying, dying_duration);
		if (blocking_animation_remaining_msec <= 0 && current_animation == &dying)
		{
			CleanUp();
			return false;
		}
		return true;
	}

	move_speed = { 0,0 };
	if (grounded == false)
	{
		if (air_remaining_msec > 0)
		{
			if (is_being_thrown_front)
				move_speed = UpdateThrownFrontMotion();
			else
				move_speed = UpdateKnockedMotion();
		}
		else
		{
			position.y = ground_y;
		}
	}

	if (is_being_thrown_back)
		UpdateThrownBackMotion();

	if (blocking_animation_remaining_msec <= 0)
	{
		if (current_animation == &being_hold_front_hit)
			UpdateCurrentAnimation(&being_hold_front);
		else if (current_animation == &being_thrown_front || current_animation == &being_thrown_back)
		{
			DecreaseHealth(throw_dmg);
			if (IsAlive())
				UpdateCurrentAnimation(&standing_up, standing_up_duration);
		}
		else if (current_animation == &being_knocked)
		{
			UpdateCurrentAnimation(&standing_up, standing_up_duration, fx_ground_hit);
			UpdateAIState(retreat);
		}
		else if (current_animation == &standing_up)
		{
			UpdateCurrentAnimation(&idle);
			if (state == switching_sides)
			{
				facing_right = target->position.x > position.x;
				UpdateAIDestinationPoint(switching_sides);
			}
		}
		else if (current_animation == &being_hit)
			UpdateCurrentAnimation(&idle);
		else if (current_animation == &attack1)
		{
			UpdateCurrentAnimation(&idle, attack_pause);
			UpdateAIState(retreat);
		}
	}

	if (unhittable_remaining_msec <= 0 && (current_animation == &being_hit || current_animation == &being_hold_front_hit))
		is_hittable = true;

	// IA start
	if (AllowAnimationInterruption() && is_being_hold_front == false && is_being_hold_back == false)
	{
		switch (state) {

		case approach:
			facing_right = target->position.x > position.x;
			UpdateCurrentAnimation(&walk);
			UpdateAIDestinationPoint(approach);
			move_speed = SpeedTowardsPoint(AI_move_destination);
			if (move_speed.IsZero())
					UpdateAIState(frontal_attack);
			break;

		case frontal_attack:
			if (position.DistanceTo(target->position) < 50 )
				UpdateCurrentAnimation(&attack1, attacks_duration);
			else
			{
				facing_right = target->position.x > position.x;
				UpdateAIDestinationPoint(frontal_attack);
				move_speed = SpeedTowardsPoint(AI_move_destination);
				UpdateCurrentAnimation(&running);
			}
			break;

		case retreat:
			facing_right = target->position.x > position.x;;
			UpdateCurrentAnimation(&walk);
			move_speed = SpeedTowardsPoint(AI_move_destination);
			if (move_speed.IsZero())
				UpdateAIState(approach);
			break;
		}
		
	}
	
	if (upd_logic)
		UpdatePosition(move_speed);

	return true;
}

void Boss3::CleanUp()
{
	App->manager->enemy_queue.remove(this);
}

bool Boss3::InEnemyActionQueue() const
{
	unsigned int max_enemies_queue = 3;
	unsigned int enemies_in_queue = App->manager->enemy_queue.size();

	if (enemies_in_queue == 0)
	{
		App->manager->enemy_queue.push_back((Entity*) this);
		return true;
	}

	for (std::list<Entity*>::const_iterator it = App->manager->enemy_queue.cbegin(); it != App->manager->enemy_queue.cend(); ++it)
		if (*it == this)
			return true;

	if (enemies_in_queue < max_enemies_queue) {
		App->manager->enemy_queue.push_back((Entity*) this);
		return true;
	}
	else
		return false;
}

iPoint Boss3::SpeedTowardsPoint(iPoint to_point) const
{
	iPoint ret = { 0,0 };
	int horizontal_diff = to_point.x - position.x;
	int vertical_diff = to_point.y - position.y;
	int hmod = horizontal_diff < 0 ? -1 : (horizontal_diff > 0 ? +1 : 0);
	int vmod = vertical_diff < 0 ? -1 : (vertical_diff > 0 ? +1 : 0);

	float speed_slope = (float)speed.y / (float)speed.x;
	float straight_line_slope;

	if (hmod == 0)
		straight_line_slope = INFINITY;
	else if (vmod == 0)
		straight_line_slope = 0;
	else
		straight_line_slope = (float)vertical_diff / (float)horizontal_diff;

	if (abs(straight_line_slope) > speed_slope)
		ret = { 0, vmod * speed.y };
	else
		ret = { hmod * speed.x, vmod * speed.y };

	if (state == retreat)
		ret.x -= hmod * 1;

	if (abs(ret.x) > abs(horizontal_diff))
		ret.x = horizontal_diff;
	if (abs(ret.y) > abs(vertical_diff))
		ret.y = vertical_diff;

	return ret;

}

void Boss3::UpdateAIDestinationPoint(AIState curr_state)
{
	App->renderer->GetPlayerPositionLimits(position_limits);
	int left = position_limits.x;
	int right = position_limits.x + position_limits.w;
	int left_of_target_mod = position.x < target->position.x ? -1 : 1;

	switch (curr_state)
	{
	case approach:
		AI_move_destination = { position.x, target->GetDepth() };
		break;
	case frontal_attack:
		AI_move_destination = { target->position.x + left_of_target_mod * 30,position.y };
		break;
	case retreat:
		if (target->facing_right)
			AI_move_destination = { right + 10, position.y };
		else
			AI_move_destination = { left - 10, position.y };

		break;
	default: break;
	}
}

bool Boss3::LoadFromConfigFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	if (json_object_dothas_value_of_type(root_object, "boss.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(root_object, "boss.graphics_file"));
	if (graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

	// ------------------------ health ----------------------------
	max_health = (int)json_object_dotget_number(root_object, "boss.max_health");
	health = max_health;
	lives = (int)json_object_dotget_number(root_object, "boss.lives");
	score = (int)json_object_dotget_number(root_object, "boss.score");
	LoadiPointFromJSONObject(root_object, "boss.speed", &speed);

	// -------------------- damages -------------------------
	attack2_dmg = (int)json_object_dotget_number(root_object, "damages.attack_boss");
	throw_dmg = (int)json_object_dotget_number(root_object, "damages.throw");

	//----------------------- colliders ---------------------------
	while (hit_collider == nullptr)
		hit_collider = LoadColliderFromJSONObject(root_object, "boss.colliders.hit", colliderType::ENEMY, &hit_collider_offset);
	while (attack_collider == nullptr)
		attack_collider = LoadColliderFromJSONObject(root_object, "boss.colliders.attack", colliderType::ENEMY_ATTACK, &attack_collider_offset);

	layer_depth = (int)json_object_dotget_number(root_object, "collision.layer_depth");

	//----------------------- duration ---------------------------
	attacks_duration = (int)json_object_dotget_number(root_object, "durations.attacks");
	being_hit_duration = (int)json_object_dotget_number(root_object, "durations.being_hit_enemy");
	being_knocked_duration = (int)json_object_dotget_number(root_object, "durations.being_knocked");
	being_thrown_duration = (int)json_object_dotget_number(root_object, "durations.being_thrown");
	standing_up_duration = (int)json_object_dotget_number(root_object, "durations.standing_up_enemy");
	unhittable_max_msec = (int)json_object_dotget_number(root_object, "durations.unhittable");
	dying_duration = (int)json_object_dotget_number(root_object, "durations.dying");
	attack_pause = (int)json_object_dotget_number(root_object, "durations.attack_pause");
	combo_window_msec = (int)json_object_dotget_number(root_object, "durations.combo_window_enemy");

	//----------------------- sprites ---------------------------
	LoadiPointFromJSONObject(root_object, "boss.sprite_offset", &sprite_offset);
	LoadiPointFromJSONObject(root_object, "boss.sprite_offset_flip", &sprite_offset_flip);

	//----------------------- animations ---------------------------
	LoadAnimationFromJSONObject(root_object, "boss.idle", &idle);
	LoadAnimationFromJSONObject(root_object, "boss.walk", &walk);
	LoadAnimationFromJSONObject(root_object, "boss.run", &running);
	LoadAnimationFromJSONObject(root_object, "boss.attack1", &attack1);
	LoadAnimationFromJSONObject(root_object, "boss.being_hit", &being_hit);
	LoadAnimationFromJSONObject(root_object, "boss.being_hold_front", &being_hold_front);
	LoadAnimationFromJSONObject(root_object, "boss.being_hold_front_hit", &being_hold_front_hit);
	LoadAnimationFromJSONObject(root_object, "boss.being_hold_back", &being_hold_back);
	LoadAnimationFromJSONObject(root_object, "boss.being_knocked", &being_knocked);
	LoadAnimationFromJSONObject(root_object, "boss.being_thrown_front", &being_thrown_front);
	LoadAnimationFromJSONObject(root_object, "boss.being_thrown_back", &being_thrown_back);
	LoadAnimationFromJSONObject(root_object, "boss.standing_up", &standing_up);
	LoadAnimationFromJSONObject(root_object, "boss.dying", &dying);
	LoadSDLRectFromJSONObject(root_object, "boss.shadow", &shadow);

	//----------------------- sounds ---------------------------
	LoadSoundFXFromJSONObject(root_object, "fx.death_boss", &fx_death);
	LoadSoundFXFromJSONObject(root_object, "fx.attack_hit_enemy", &fx_attack_hit);
	LoadSoundFXFromJSONObject(root_object, "fx.ground_hit", &fx_ground_hit);

	//--- free json 
	json_value_free(root_value);

	return true;
}
