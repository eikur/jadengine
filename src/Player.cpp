#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "EntityManager.h"

#include "Player.h"

Player::Player() : Entity(Types::player) {}
Player::~Player(){}

bool Player::Init()
{
	if (LoadFromConfigFile(CONFIG_FILE) == false)
	{
		MYLOG("Error loading player config from file");
		return false;
	}
	ReRaise();
	return true;
}

bool Player::Update(unsigned int msec_elapsed, const bool upd_logic)
{
	if (blocking_animation_remaining_msec > 0)
		blocking_animation_remaining_msec = MAX(blocking_animation_remaining_msec - msec_elapsed, 0);
	if (air_remaining_msec > 0)
		air_remaining_msec = MAX(air_remaining_msec - msec_elapsed, 0);
	if (combo_remaining_msec > 0)
		combo_remaining_msec = MAX(combo_remaining_msec - msec_elapsed, 0);
	if (combo_hold_remaining_msec > 0)
		combo_hold_remaining_msec = MAX(combo_hold_remaining_msec - msec_elapsed, 0);

	current_combo_hits = combo_remaining_msec <= 0 ? 0 : (current_combo_hits >= 4 ? 0 : current_combo_hits);
	current_combo_hold_hits = combo_hold_remaining_msec <= 0 ? 0 : current_combo_hold_hits;

	if (IsAlive() == false)
	{
		if (upd_logic && current_animation == &being_knocked)
			UpdatePosition(UpdateKnockedMotion());

		if (blocking_animation_remaining_msec <= 0 && current_animation != &dying)
			UpdateCurrentAnimation(&dying, dying_duration);

		if (blocking_animation_remaining_msec <= 0 && current_animation == &dying)
		{
			ModifyLives(-1);
			if (lives > 0)
				ReRaise();
			else
			{
				RemoveColliders();
				CleanUp();
				return false;
			}
		}
		return true;
	}
	GetInput();
	move_speed = { 0,0 };

	// animation update
	if (grounded == false) 
	{
		if (air_remaining_msec > 0)
		{
			if (respawn_fall == true)
			{
				if (upd_logic)
				move_speed.y += 9; 
			}
			else if (jumping)
				move_speed.y = UpdateJumpVerticalSpeed();
			else if (is_holding_back || is_holding_front)
					UpdateHoldingSwapMotion();
			else
				move_speed = UpdateKnockedMotion();
		}
		else
		{
			respawn_fall = false;
			move_speed.y = 0;
			position.y = ground_y;
			if (jumping)
				UpdateCurrentAnimation(&jump_land, jump_prep_duration, fx_landing_jump);
		}
	}
	if (current_animation == &throwing_back)
		UpdateThrowingBackMotion();
	if (current_animation == &throwing_front)
		UpdateThrowingFrontMotion();

	// animation transition
	if (blocking_animation_remaining_msec <= 0)
	{
		if (current_animation == &jump_prep){
			UpdateCurrentAnimation(&jump);
			air_remaining_msec = jump_duration;
		}
		else if (current_animation == &jump_land || current_animation == &being_hit || current_animation == &take_item)
			UpdateCurrentAnimation(&idle);
		else if (current_animation == &being_knocked)
			UpdateCurrentAnimation(&standing_up, standing_up_duration);
		else if (current_animation == &holding_front_attack)
			UpdateCurrentAnimation(&holding_front);
		else if (current_animation == &holding_front_attack2)
		{
			held_entity->SetBeingKnocked(attack2_dmg);
			held_entity = nullptr;
			UpdateCurrentAnimation(&idle, hold_attacks_duration, fx_attack_hit_hard);
			current_combo_hold_hits = 0;
		}
		else if (current_animation == &throwing_back || current_animation == &throwing_front)
		{
			held_entity = nullptr;
			UpdateCurrentAnimation(&idle, hold_attacks_duration, fx_ground_hit);
		}

		else if (current_animation == &holding_swap)
		{
			is_holding_back = !is_holding_back;	is_holding_front = !is_holding_front; 	facing_right = !facing_right;
			position.y = ground_y;
			current_hold_swaps += 1;
			if (current_hold_swaps < max_hold_swaps)
			{
				if (is_holding_front)
				{
					held_entity->SetBeingHoldFront();	
					UpdateCurrentAnimation(&holding_front, 0, fx_landing_jump);
				}
				else if (is_holding_back)
				{
					held_entity->SetBeingHoldBack(); 
					UpdateCurrentAnimation(&holding_back, 0, fx_landing_jump);
				}
			}
			else
			{
				current_hold_swaps = 0;
				held_entity->SetIdle();
				held_entity = nullptr;
				UpdateCurrentAnimation(&idle, 0, fx_landing_jump);
			}
		}
	}

	if (AllowAnimationInterruption())
	{
		if (is_holding_front)
		{
			if (held_entity->health <= 0)
			{
				held_entity = nullptr;
				UpdateCurrentAnimation(&idle);
				current_hold_swaps = 0;
			}
			else if (input_hold_front_throw)
			{
				facing_right = !facing_right;
				UpdateCurrentAnimation(&throwing_front, throwing_duration);
				held_entity->SetBeingThrownFront( position);
				held_entity = nullptr;
				current_hold_swaps = 0;
			}
			else if (input_attack )
			{
				if (current_combo_hold_hits <= 1)
				{
					held_entity->SetBeingHoldFrontHit(attack1_dmg);
					UpdateCurrentAnimation(&holding_front_attack, hold_attacks_duration, fx_attack_hit);
					current_combo_hold_hits += 1;
					combo_hold_remaining_msec = combo_hold_window_msec;
				}
				else
				{
					UpdateCurrentAnimation(&holding_front_attack2, hold_attacks_duration);
					current_combo_hold_hits = 0;
				}
		
			}
			else if (input_jump)
			{
				UpdateCurrentAnimation(&holding_swap, holding_swap_duration, fx_jump);
				air_remaining_msec = holding_swap_duration;
			}
			else if ((facing_right == true && input_horizontal < 0 )|| (facing_right == false && input_horizontal > 0))
			{
				held_entity->SetIdle();
				held_entity = nullptr;
				UpdateCurrentAnimation(&idle);
				facing_right = !facing_right;
				current_hold_swaps = 0;
			}
		}
		else if (is_holding_back)
		{
			if (input_jump)
			{
				UpdateCurrentAnimation(&holding_swap, holding_swap_duration, fx_jump);
				air_remaining_msec = holding_swap_duration;
			}
			else if (input_attack)
			{
				held_entity->SetBeingThrownBack(position);
				held_entity = nullptr;
				UpdateCurrentAnimation(&throwing_back, throwing_duration);
				current_hold_swaps = 0;
			}
			else if ((facing_right == true && input_horizontal < 0) || (facing_right == false && input_horizontal > 0))
			{
				held_entity->SetIdle();
				held_entity = nullptr;
				UpdateCurrentAnimation(&idle);
				facing_right = !facing_right;
				current_hold_swaps = 0;
			}
		}
		else
		{
			move_speed.y += grounded? speed.y*input_vertical : 0;
			move_speed.x += speed.x*input_horizontal;
			facing_right = input_horizontal == 0 ? facing_right : (input_horizontal > 0 ? true : false);

			if (grounded == false)	
			{
				if (input_attack)
					UpdateCurrentAnimation(&jump_attack, 0, fx_voice);
			}
			else 		
			{
				if (input_help)
					UseHelp();
				else if (input_attack_back)
					UpdateCurrentAnimation(&attack_back, attacks_duration, fx_attack_miss);
				else if (input_jump)
					UpdateCurrentAnimation(&jump_prep, jump_prep_duration, fx_jump);
				else if (input_attack)
				{
					if (reachable_item != nullptr)
					{
						UpdateCurrentAnimation(&take_item, take_item_duration, fx_health_restore);
						IncreaseHealth(reachable_item->max_health);
						reachable_item->DecreaseHealth(reachable_item->max_health);	
						reachable_item = nullptr;
					}
					else
					{
						if (current_combo_hits <= 1)
							UpdateCurrentAnimation(&attack1, attacks_duration, fx_attack_miss);
						else  if (current_combo_hits == 2)
							UpdateCurrentAnimation(&attack2, attacks_duration, fx_attack_miss);
						else if (current_combo_hits == 3)
							UpdateCurrentAnimation(&attack3, attacks_duration, fx_attack_miss);
						current_combo_hits += 1;
					}
				}
				else if (move_speed.IsZero())
					UpdateCurrentAnimation(&idle);
				else
					UpdateCurrentAnimation(&walk);
			}
		}
	}

	reachable_item = nullptr;

	if (upd_logic)
	{
		UpdatePosition(move_speed);
		App->renderer->MoveCamera(position.x, speed.x);
	}

	// miscelaneous
	CheatCodes();

	return true;
}

void Player::AddScore(int addition)
{
	if ((score + addition) >= 999999) {
		score = 999999;
		return;
	}
	if (score < 50000 && (score+addition)>=50000)
	{ 
		ModifyLives(+1);
	}
	score += addition;
}

void Player::ModifyLives(int mod_to_add)
{
	lives += mod_to_add;
	if (lives> 9)
	{
		lives = 9;
		return;
	}
	if (lives<= 0)
	{
		lives = 0;
		return;		//Game over
	}
	if (mod_to_add > 0)
		App->audio->PlayFx(fx_extra_life);
}

void Player::IncreaseHelp(int amount)
{
	help += amount;
	if (help > 9)
	{
		help = 9;
		return;
	}
	App->audio->PlayFx(fx_extra_help);
}

void Player::ReRaise()
{
	position = { 20, 32};
	position.x += position_limits.x;
	ground_y = 174;
	UpdateCurrentAnimation(&jump, jump_duration);
	facing_right = true;
	respawn_fall = true;
	air_remaining_msec = jump_duration;
	health = max_health;
	help = 1;
	App->manager->KnockDownAllEnemies();
	App->manager->RestoreTimeLeft();
}

//-------------- Specific updates
void Player::UpdatePosition(const iPoint new_speed) {

	if (grounded)
		ground_y = position.y;

	position += new_speed;

	App->renderer->GetPlayerPositionLimits(position_limits);

	int up = position_limits.y;
	int down = up + position_limits.h;
	int left = position_limits.x;
	int right = left + position_limits.w;

	if (held_entity == nullptr && current_animation != &throwing_back)
	{
		if (position.x > right)
			position.x = right;
		else
			if (position.x < left)
				position.x = left;
		if (grounded)
		{
			if (position.y < up)
				position.y = up;
			else
				if (position.y > down)
					position.y = down;
			ground_y = position.y;
		}
	}

	//apply offset to colliders
	if (facing_right)
	{
		attack_collider->rect.x = position.x + attack_collider_offset.x;
		hit_collider->rect.x = position.x + hit_collider_offset.x;
	}
	else
	{
		attack_collider->rect.x = position.x + -(attack_collider_offset.x + attack_collider->rect.w);
		hit_collider->rect.x = position.x - (hit_collider_offset.x + hit_collider->rect.w);
	}
	attack_collider->rect.y = position.y + attack_collider_offset.y;	
	hit_collider->rect.y = position.y + hit_collider_offset.y;
}

int Player::UpdateJumpVerticalSpeed()
{
	int divisor = jump_duration / 16;
	int frames_left = air_remaining_msec / divisor;
	int ret = 0;

	switch (frames_left)
	{
	case 0:	 ret += 7; break;
	case 1:	 ret += 6; break;
	case 2:	 ret+= 5; break;
	case 3:	 ret+= 5; break;
	case 4:	 ret+= 4; break;
	case 5:	 ret+= 3; break;
	case 6:	 ret+= 2; break;
	case 7:  ret+= 1; break;
	case 8:  ret-= 1; break;
	case 9:  ret-= 2; break;
	case 10: ret-= 3; break;
	case 11: ret-= 4; break;
	case 12: ret-= 5; break;
	case 13: ret-= 5; break;
	case 14: ret-= 6; break;
	case 15: ret-= 7; break;
	}
	return ret;
} 

void Player::UpdateHoldingSwapMotion()
{
	int divisor = holding_swap_duration / 5;
	int frames_left = air_remaining_msec / divisor;

	int mod = facing_right ? 1 : -1;

	holding_swap.SetCurrentFrame(4 - frames_left);
	switch (frames_left)
	{
	case 4: SetPosition({ held_entity->position.x - mod * 24, held_entity->position.y - 6 }); break;
	case 3: SetPosition({ held_entity->position.x, held_entity->position.y - 50 }); break;
	case 2: SetPosition({ held_entity->position.x, held_entity->position.y - 50 }); break;
	case 1: SetPosition({ held_entity->position.x, held_entity->position.y - 50 }); break;
	case 0: SetPosition({ held_entity->position.x + mod * 24 , held_entity->position.y - 6 }); break;
	}
}

void Player::UpdateThrowingFrontMotion()
{
	int divisor = throwing_duration / 4;
	int frames_left = blocking_animation_remaining_msec / divisor;

	throwing_front.SetCurrentFrame(3 - frames_left);
}

void Player::UpdateThrowingBackMotion()
{
	int divisor = throwing_duration / 4;
	int frames_left = blocking_animation_remaining_msec / divisor;

	throwing_back.SetCurrentFrame(3 - frames_left);
}

//--- Input related -------

void Player::GetInput()
{
	ResetInput();

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		input_vertical = -1;
	else
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			input_vertical = 1;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		input_horizontal = -1;
	else
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			input_horizontal = 1;
	
	input_help = App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN;
	input_jump = App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN;
	

	input_hold_front_throw = 
		(App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) && 
		((facing_right == true && input_horizontal < 0) || (facing_right == false && input_horizontal > 0));

	input_attack_back = 
		(App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT) || 
		(App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN);

	input_attack = App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && input_hold_front_throw == false && input_attack_back == false;
	
}

void Player::ResetInput()
{
	input_horizontal = input_vertical = 0;
	input_help = input_attack = input_jump = input_attack_back = input_hold_front_throw = false;
}

void Player::UseHelp()
{
	if (help <= 0)
		return;
	help -= 1;
	App->audio->PlayFx(fx_use_help);
	App->manager->KnockDownAllEnemies(true);
}
//--------------------------- Miscelaneous ---------------------------------------------

bool Player::LoadFromConfigFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	if (json_object_dothas_value_of_type(root_object, "player.graphics_file", JSONString))
		graphics = App->textures->Load(json_object_dotget_string(root_object, "player.graphics_file"));
	if (graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

//----------------------- game variables ---------------------------
	LoadiPointFromJSONObject(root_object, "player.speed", &speed);
	max_health = (int)json_object_dotget_number(root_object, "player.max_health");
	health = max_health;
	lives = (int)json_object_dotget_number(root_object, "player.lives");
	score = (int)json_object_dotget_number(root_object, "player.score");
	help = (int)json_object_dotget_number(root_object, "player.help");

	//----------------------- damages ---------------------------

	attack1_dmg = (int)json_object_dotget_number(root_object, "damages.attack1");
	attack2_dmg = (int)json_object_dotget_number(root_object, "damages.attack2");
	attack3_dmg = (int)json_object_dotget_number(root_object, "damages.attack3");
	attack_back_dmg = (int)json_object_dotget_number(root_object, "damages.attack_back");
	throw_dmg = (int)json_object_dotget_number(root_object, "damages.throw");


//----------------------- logic durations---------------------------
	attacks_duration = (int)json_object_dotget_number(root_object, "durations.attacks");
	hold_attacks_duration = (int)json_object_dotget_number(root_object, "durations.hold_attacks");
	jump_prep_duration = (int)json_object_dotget_number(root_object, "durations.jump_prep");
	jump_duration = (int)json_object_dotget_number(root_object, "durations.jump");
	throwing_duration = (int)json_object_dotget_number(root_object, "durations.throwing");
	being_hit_duration = (int)json_object_dotget_number(root_object, "durations.being_hit_player");
	being_thrown_duration = (int)json_object_dotget_number(root_object, "durations.being_thrown");
	being_knocked_duration= (int)json_object_dotget_number(root_object, "durations.being_knocked");
	standing_up_duration = (int)json_object_dotget_number(root_object, "durations.standing_up_player");
	holding_swap_duration = (int)json_object_dotget_number(root_object, "durations.holding_swap");
	take_item_duration = (int)json_object_dotget_number(root_object, "durations.take_item");
	combo_window_msec = (int)json_object_dotget_number(root_object, "durations.combo_window");
	combo_hold_window_msec = (int)json_object_dotget_number(root_object, "durations.hold_combo_window");
	dying_duration = (int)json_object_dotget_number(root_object, "durations.dying");

//----------------------- colliders ---------------------------
	hit_collider = LoadColliderFromJSONObject(root_object, "player.colliders.hit", colliderType::PLAYER, &hit_collider_offset);
	attack_collider = LoadColliderFromJSONObject(root_object, "player.colliders.attack", colliderType::PLAYER_ATTACK, &attack_collider_offset);

//----------------------- sprite offsets ---------------------------
	LoadiPointFromJSONObject(root_object, "player.sprite_offset", &sprite_offset);
	LoadiPointFromJSONObject(root_object, "player.sprite_offset_flip", &sprite_offset_flip);

// ------------------ animations -------------------------------------
	LoadAnimationFromJSONObject(root_object, "player.idle", &idle);
	LoadAnimationFromJSONObject(root_object, "player.walk", &walk);
	LoadAnimationFromJSONObject(root_object, "player.jump_prep", &jump_prep);
	LoadAnimationFromJSONObject(root_object, "player.jump", &jump);
	LoadAnimationFromJSONObject(root_object, "player.jump_attack", &jump_attack);
	jump_land = jump_prep;
	LoadAnimationFromJSONObject(root_object, "player.attack1", &attack1);
	LoadAnimationFromJSONObject(root_object, "player.attack2", &attack2);
	LoadAnimationFromJSONObject(root_object, "player.attack3", &attack3);
	LoadAnimationFromJSONObject(root_object, "player.attack_back", &attack_back);
	LoadAnimationFromJSONObject(root_object, "player.holding_front", &holding_front);
	LoadAnimationFromJSONObject(root_object, "player.holding_front_attack", &holding_front_attack);
	LoadAnimationFromJSONObject(root_object, "player.holding_front_attack2", &holding_front_attack2);
	LoadAnimationFromJSONObject(root_object, "player.holding_back", &holding_back);
	LoadAnimationFromJSONObject(root_object, "player.holding_swap", &holding_swap);
	LoadAnimationFromJSONObject(root_object, "player.throwing_front", &throwing_front);
	LoadAnimationFromJSONObject(root_object, "player.throwing_back", &throwing_back);
	LoadAnimationFromJSONObject(root_object, "player.take_item", &take_item);
	LoadAnimationFromJSONObject(root_object, "player.being_hit", &being_hit);
	LoadAnimationFromJSONObject(root_object, "player.being_knocked", &being_knocked);
	LoadAnimationFromJSONObject(root_object, "player.being_thrown_front", &being_thrown_front);
	LoadAnimationFromJSONObject(root_object, "player.standing_up", &standing_up);
	LoadAnimationFromJSONObject(root_object, "player.dying", &dying);

	LoadSDLRectFromJSONObject(root_object, "player.shadow", &shadow);

// ---------------------- sound effects ----------------------------
	LoadSoundFXFromJSONObject(root_object, "fx.voice_player", &fx_voice);
	LoadSoundFXFromJSONObject(root_object, "fx.life_up", &fx_extra_life);
	LoadSoundFXFromJSONObject(root_object, "fx.health_item", &fx_health_restore);
	LoadSoundFXFromJSONObject(root_object, "fx.attack_miss", &fx_attack_miss);
	LoadSoundFXFromJSONObject(root_object, "fx.attack_hit", &fx_attack_hit);
	LoadSoundFXFromJSONObject(root_object, "fx.attack_hit_hard", &fx_attack_hit_hard);
	LoadSoundFXFromJSONObject(root_object, "fx.ground_hit", &fx_ground_hit);
	LoadSoundFXFromJSONObject(root_object, "fx.jump", &fx_jump);
	LoadSoundFXFromJSONObject(root_object, "fx.jump_land", &fx_landing_jump);
	LoadSoundFXFromJSONObject(root_object, "fx.death_player", &fx_death);
	LoadSoundFXFromJSONObject(root_object, "fx.help_up", &fx_extra_help);
	LoadSoundFXFromJSONObject(root_object, "fx.help_use", &fx_use_help);
	
	

	json_value_free(root_value);

	return true;

}

void Player::CheatCodes() {
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		ModifyLives(+1);
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
		AddScore(1000);
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		IncreaseHelp(1);
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		SetBeingHit(attack1_dmg);
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		SetBeingKnocked(attack3_dmg);
}