#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleTimer.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"

#include "Player.h"
#include "EnemyGarcia.h"
#include "Apple.h"
#include "Chicken.h"
#include "Boss3.h"

#include "EntityManager.h"

EntityManager::EntityManager(bool active): Module(active){

}

EntityManager::~EntityManager() {
}

// ----- Module basic functions --------
bool EntityManager::Init()
{
	if (LoadConfigFromFile(CONFIG_FILE) == false)
	{
		MYLOG("Entity Manager: failed to initialise\n");
		return false;
	}
	else 
		return true;
}

bool EntityManager::Start()
{
	return true;
}

update_status EntityManager::Update() 
{
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		pause = !pause;
		if (pause)
			App->timer->TimerPause();
		else
			App->timer->TimerResume();
		if (player != nullptr)
			App->audio->PlayFx(fx_pause);
	}

	if (pause == false)
	{
		elapsed_msec += App->timer->DeltaTime();
		time_left_msec -= App->timer->DeltaTime();
		if (remaining_msec_go_arrow > 0)
			remaining_msec_go_arrow -= App->timer->DeltaTime();

		if (time_left_msec <= 0 && player != nullptr) 
		{
			if (player->IsAlive())
				player->TimeOver();
			time_left_msec = 0;
		}

		if (elapsed_msec >= upd_logic_msec)
			upd_logic = true;

	
		entities.sort(Entity::ptrEntityDepthComparison());
		for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end();)
		{
			if ((*it)->Update(elapsed_msec, upd_logic) == false)
			{

				if (*it == player)
					player = nullptr;
				else
					player->AddScore((*it)->score);
				if (*it == boss)
					boss = nullptr;
				RELEASE(*it);
				it = entities.erase(it);
			}
			else
			{
				(*it)->Draw();
				++it;
			}
		}
		
		if (upd_logic == true)
		{
			elapsed_msec = 0;
			upd_logic = false;
		}
	}

	PrintStatus();

	if (player == nullptr)
	{
		if (App->fade->isFading() == false)
			App->fade->FadeToBlack((Module*)App->intro, (Module*)App->scene3, 2.0f);
	}
	
	CheatCodes();

	return UPDATE_CONTINUE;
}

bool EntityManager::CleanUp()
{
	MYLOG("EntityManager: Removing entities from application\n");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		(*it)->CleanUp(); (*it)->RemoveColliders();	delete *it;
	}
	entities.clear();
	enemy_queue.clear();

	return true;
}

//---------------------- Entity Management ------------------
Entity* EntityManager::CreateEntity(Entity::Types type)
{
	static_assert(Entity::Types::unknown == 5, "code needs update");
	Entity* ret = nullptr;

	switch (type)
	{
	case Entity::Types::player: ret = new Player(); break;
	case Entity::Types::npc_garcia: ret = new EnemyGarcia(player); break;
	case Entity::Types::item_apple: ret = new Apple(); break;
	case Entity::Types::item_chicken: ret = new Chicken(); break;
	case Entity::Types::npc_boss: ret = new Boss3(player); break;
	}

	if (ret != nullptr)
	{
		if (ret->Init() == false)
		{
			delete ret;
			ret = nullptr;
		}
		else
		{
			entities.push_back(ret);
		}
	}
	return ret;
}

void EntityManager::KnockDownAllEnemies(bool wipeout)  {
	for (std::list<Entity*>::const_iterator it = entities.cbegin(); it != entities.cend();++it)
	{
		if ((*it)->m_type == Entity::Types::npc_garcia || (*it)->m_type == Entity::Types::npc_boss)
		{
			if ((*it)->IsAlive())
			{
				if (wipeout)
					(*it)->SetBeingKnocked((*it)->max_health);
				else
					(*it)->SetBeingKnocked();
			}
		}
	}
}

int EntityManager::GetEnemyCount() {
	int ret = 0;
	for (std::list<Entity*>::const_iterator it = entities.cbegin(); it != entities.cend(); ++it)
		if ((*it)->m_type == Entity::Types::npc_garcia)
			ret += 1;
	return ret;
}

void EntityManager::RestoreTimeLeft()
{
	time_left_msec = 81999;
}

void EntityManager::ShowGoArrow()
{
	remaining_msec_go_arrow = 7 * blink_msec_go_arrow;
}

void EntityManager::HandleCollision(Collider* a, Collider* b)
{
	Entity* first = nullptr;
	Entity* second = nullptr;
	Collider* first_col = nullptr;
	Collider* second_col = nullptr;
		
	// order by type to ease the logic
	if (a->type <= b->type)
	{
		first = a->parent; first_col = a;
		second = b->parent; second_col = b;
	}
	else
	{
		first = b->parent; first_col = b;
		second = a->parent; second_col = a;
	}

	int depth_difference = first->GetDepth() - second->GetDepth();
	if (depth_difference < -layer_depth || depth_difference > layer_depth)
		return;


	switch (first_col->type)
	{
		case colliderType::PLAYER:
			if (second_col->type == colliderType::ITEMS)	
			{
				first->SetReachableItem(second);
			}
			else if (second_col->type == colliderType::ENEMY)
			{
				if (first->IsGrounded() && !first->IsHoldingSomeone() && first->AllowAnimationInterruption() &&
					second->IsGrounded() && !second->is_being_thrown_back && second->IsAlive() &&
					((first->facing_right == true && first_col->rect.x <= second_col->rect.x) ||
					(first->facing_right == false && second_col->rect.x <= first_col->rect.x)))
				{
					if (first->facing_right == second->facing_right)
					{
						first->SetHoldingBack(second); 
						second->SetBeingHoldBack();
					}
					else
					{
						first->SetHoldingFront(second);
						second->SetBeingHoldFront();
					}
					
					if (depth_difference != -1 || abs(first->position.x - second->position.x) < first_col->rect.w)
					{
						if (first->facing_right)
							second->UpdatePosition({ first->position.x - second->position.x + first_col->rect.w, first->position.y - second->position.y - 1 });
						else
							second->UpdatePosition({ first->position.x - second->position.x - first_col->rect.w, first->position.y - second->position.y - 1 });
					}
				}
			}
			else if (second_col->type == colliderType::ENEMY_ATTACK)
			{
				if (second->is_attacking && first->is_hittable && first->IsGrounded() && first->IsAlive())
				{
					if (second->position.x <= first->position.x)
						first->facing_right = false;
					else
						first->facing_right = true;

					App->audio->PlayFx(second->fx_attack_hit);
					if (second->current_combo_hits <= 2)
						first->SetBeingHit(second->attack1_dmg);
					else if (second->current_combo_hits == 3)
						first->SetBeingKnocked(second->attack2_dmg);
					second->combo_remaining_msec = second->combo_window_msec;

				}
			}
			else
			{
				MYLOG("Bad defined collision, check collision matrix");
			}
		break;

		case colliderType::PLAYER_ATTACK:
			if (second_col->type == colliderType::ENEMY)
			{
				if (first->is_attacking  && second->is_hittable && second->IsAlive() )	
				{
					second->is_hittable = false;
					App->audio->PlayFx(first->fx_attack_hit);
					if (first->position.x <= second->position.x)
						second->facing_right = false;
					else
						second->facing_right = true;

					if (first->IsGrounded())
					{
						if (first->current_combo_hits <= 2)
							second->SetBeingHit(first->attack1_dmg);
						else if (first->current_combo_hits <= 3)
							second->SetBeingHit(first->attack2_dmg);
						else if( first->current_combo_hits == 4)
						{					
							second->SetBeingKnocked(first->attack3_dmg);
							first->combo_remaining_msec = 0;
						}

						first->combo_remaining_msec = first->combo_window_msec;
					}
					else {
						second->SetBeingKnocked(first->attack3_dmg);
					}
				}
			}
			else
			{
				MYLOG("Bad defined collision, check collision matrix");
			}
			break;

		case colliderType::ENEMY:
			if (second_col->type == colliderType::ENEMY) 
			{
				if (first->is_being_thrown_front && first->IsGrounded() == false && second->is_hittable && second->IsAlive() )
					second->SetBeingKnocked(first->throw_dmg);
				else if (second->is_being_thrown_front && second->IsGrounded() == false && first->is_hittable && first->IsAlive())
					first->SetBeingKnocked(second->throw_dmg);
			}
			else
			{
				MYLOG("Bad defined collisions, check collision matrix");
			}
			break;
	}
}

// -------------------------- Miscellaneous -------------------------------
void EntityManager::PrintStatus()
{
	App->renderer->Blit(hud_graphics, 0, 0, &hud_section, 0.0F);
	if (pause)
	{
		App->fonts->Print(134, 96, ModuleFonts::Fonts::scene_overlap, "PAUSE");
	}
	if (player != nullptr){
		App->fonts->Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(time_left_msec / 2000, 2));
		App->fonts->Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, App->fonts->GetPrintableValue(player->score, 6));
		App->fonts->Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(player->help, 1));
		App->fonts->Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, App->fonts->GetPrintableValue(player->lives, 1));
		PrintPlayerHealth();
		PrintGoArrow();
	}
	else {
		App->fonts->Print(hud_time_pos.x, hud_time_pos.y, ModuleFonts::Fonts::hud_big, "00");
		App->fonts->Print(hud_score_pos.x, hud_score_pos.y, ModuleFonts::Fonts::hud_small, "000000");
		App->fonts->Print(hud_help_pos.x, hud_help_pos.y, ModuleFonts::Fonts::hud_big, "0" );
		App->fonts->Print(hud_lives_pos.x, hud_lives_pos.y, ModuleFonts::Fonts::hud_big, "0");
		App->fonts->Print(146, 110, ModuleFonts::Fonts::scene_overlap, "END");
	}
	if (boss != nullptr)
	{
		App->renderer->Blit(hud_graphics, hud_boss_pos.x, hud_boss_pos.y, &hud_boss_section, 0.0F);
		App->fonts->Print(hud_boss_msg_pos.x, hud_boss_msg_pos.y, ModuleFonts::Fonts::hud_small, "-BOSS-");
		PrintBossHealth();
	}

}

void EntityManager::PrintPlayerHealth()
{
	int min_pixels = 2;
	int sections_to_draw = player->health / min_pixels;
	for (int i = 0; i < sections_to_draw; ++i)
	{
		App->renderer->Blit(hud_graphics, hud_health_pos.x + i*hud_health_section.w, hud_health_pos.y, &hud_health_section, 0.0F);
	}
}

void EntityManager::PrintBossHealth()
{
	int min_pixels = 2;
	int sections_to_draw;
	SDL_Rect *h_section;

	if (boss->health > 110)
	{
		sections_to_draw = 40;
		h_section = &hud_high_health_section;
	}
	else if (boss->health > 80)
	{
		sections_to_draw = 40;
		h_section = &hud_medium_health_section;
	}
	else
	{
		sections_to_draw = boss->health / min_pixels;
		h_section = &hud_health_section;
	}
	for (int i = 0; i < sections_to_draw; ++i)
		App->renderer->Blit(hud_graphics, hud_health_boss_pos.x + i*hud_health_section.w, hud_health_boss_pos.y, h_section, 0.0F);
}

void EntityManager::PrintGoArrow()
{
	static int audio_in_turn;
	if (remaining_msec_go_arrow <= 0)
		return;

	int current_turn = remaining_msec_go_arrow / blink_msec_go_arrow;
	bool show = current_turn % 2 == 0 ? true : false;
	
	if (show)
	{
		App->renderer->Blit(hud_graphics, hud_go_arrow_pos.x, hud_go_arrow_pos.y, &hud_go_arrow_section, 0.0F);
		if (audio_in_turn != current_turn)
		{
			App->audio->PlayFx(fx_go_arrow);
			audio_in_turn = current_turn;
		}
	}
}

bool EntityManager::LoadConfigFromFile(const char* file_path)
{
	JSON_Value *root_value;
	JSON_Object *root_object;
	JSON_Array *j_array;

	root_value = json_parse_file(file_path);
	if (root_value == nullptr)
		return false;
	else
		root_object = json_object(root_value);

	// depth for collisions
	layer_depth = (int)json_object_dotget_number(root_object, "collision.layer_depth");


	// hud load
	if (json_object_dothas_value_of_type(root_object, "hud.graphics_file", JSONString))
		hud_graphics = App->textures->Load(json_object_dotget_string(root_object, "hud.graphics_file"));
	if (hud_graphics == nullptr)
	{
		json_value_free(root_value);
		return false;
	}

	j_array = json_object_dotget_array(root_object, "hud.section");
	hud_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.time_pos");
	hud_time_pos.x = (int)json_array_get_number(j_array, 0);
	hud_time_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.score_pos");
	hud_score_pos.x = (int)json_array_get_number(j_array, 0);
	hud_score_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.lives_pos");
	hud_lives_pos.x = (int)json_array_get_number(j_array, 0);
	hud_lives_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.help_pos");
	hud_help_pos.x = (int)json_array_get_number(j_array, 0);
	hud_help_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.health_pos");
	hud_health_pos.x = (int)json_array_get_number(j_array, 0);
	hud_health_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.health_section");
	hud_health_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.boss_section");
	hud_boss_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.high_health_section");
	hud_high_health_section= { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.medium_health_section");
	hud_medium_health_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.boss_pos");
	hud_boss_pos.x = (int)json_array_get_number(j_array, 0);
	hud_boss_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.boss_health_pos");
	hud_health_boss_pos.x = (int)json_array_get_number(j_array, 0);
	hud_health_boss_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.boss_msg_pos");
	hud_boss_msg_pos.x = (int)json_array_get_number(j_array, 0);
	hud_boss_msg_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	if (json_object_dothas_value_of_type(root_object, "fx.pause", JSONString))
		fx_pause = App->audio->LoadFx(json_object_dotget_string(root_object, "fx.pause"));

	j_array = json_object_dotget_array(root_object, "hud.go_arrow_pos");
	hud_go_arrow_pos.x = (int)json_array_get_number(j_array, 0);
	hud_go_arrow_pos.y = (int)json_array_get_number(j_array, 1);
	json_array_clear(j_array);

	j_array = json_object_dotget_array(root_object, "hud.go_arrow_section");
	hud_go_arrow_section = { (int)json_array_get_number(j_array,0),(int)json_array_get_number(j_array,1),(int)json_array_get_number(j_array,2),(int)json_array_get_number(j_array,3) };
	json_array_clear(j_array);

	blink_msec_go_arrow = (int)json_object_dotget_number(root_object, "hud.go_arrow_blink_msec");

	if (json_object_dothas_value_of_type(root_object, "hud.fx_go_arrow", JSONString))
		fx_go_arrow = App->audio->LoadFx(json_object_dotget_string(root_object, "hud.fx_go_arrow"));

	json_value_free(root_value);

	return true;
}

void EntityManager::CheatCodes()
{
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && player != nullptr)
	{
		Entity *a = (Entity*) CreateEntity(Entity::Types::npc_garcia);
		if (a != nullptr){
			a->SetPosition({ player->position.x + 147, player->GetDepth() });

		}
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && player != nullptr)
	{
		Entity *a = (Entity*)CreateEntity(Entity::Types::npc_boss);
		if (a != nullptr) {
			a->SetPosition({ player->position.x + 147, player->GetDepth() });
			boss = a;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && player != nullptr)
	{
		Entity *a = (Entity*)CreateEntity(Entity::Types::item_apple);
		if (a != nullptr) {
			a->SetPosition({ player->position.x + 48, player->GetDepth() });

		}
		return;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && player != nullptr)
	{
		Entity *a = (Entity*)CreateEntity(Entity::Types::item_chicken);
		if (a != nullptr) {
			a->SetPosition({ player->position.x + 48, player->GetDepth() });

		}
		return;
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT && player != nullptr)
	{
		time_left_msec -= 2000;
		return;
	}
}



