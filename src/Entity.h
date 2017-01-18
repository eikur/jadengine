#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <math.h>
#include "parson/parson.h"
#include "Point.h"
#include "Globals.h"
#include "Animation.h"

struct Collider;
enum colliderType;
struct SDL_Texture;
struct SDL_Rect;

class Entity 
{
public:
	enum Types
	{
		player,
		npc_garcia,
		item_apple,
		item_chicken,
		npc_boss,
		unknown
	};

	enum AIState {
		none,
		approach,
		frontal_attack,
		retreat,
		switching_sides
	};

	struct ptrEntityDepthComparison {
		bool operator() (const Entity* left, const Entity* right) const {
			return left->GetDepth() < right->GetDepth();
		}
	};

public:
	Entity(Types type);
	virtual ~Entity();

	virtual bool Init();
	virtual bool Update(unsigned int msec_elapsed, const bool upd_logic = false);

	virtual void UpdatePosition(const iPoint new_speed);
	virtual void SetPosition(const iPoint new_position);
	void UpdateCurrentAnimation(Animation *new_anim, int block_anim_duration = 0, int fx_new_anim = -1, bool override_current = false);
	bool Draw() const;

	iPoint UpdateKnockedMotion();
	iPoint UpdateThrownFrontMotion();
	void UpdateThrownBackMotion();

	int  GetDepth() const;
	void SetDepth( int new_depth);
	
	bool IsGrounded() const;
	bool IsAlive() const;
	bool IsHoldingSomeone();

	void IncreaseHealth(int amount);
	virtual void DecreaseHealth(int amount);
	virtual void AddScore(int amount);
	virtual void IncreaseHelp(int amount);
	void TimeOver();

	bool AllowAnimationInterruption() const;
	void SetIdle();
	void SetReachableItem( Entity* reachable = nullptr);
	void SetHoldingFront(Entity* held = nullptr);
	void SetHoldingBack(Entity* held = nullptr);
	void SetBeingHit(int damage = 0);
	void SetBeingHoldFront();
	void SetBeingHoldFrontHit(int damage = 0);
	void SetBeingHoldBack();
	void SetBeingThrownFront( iPoint pvt);
	void SetBeingThrownBack( iPoint pvt);
	void SetBeingKnocked(int damage = 0);
		
	virtual void UpdateAIState(AIState new_state);
	void RemoveColliders();
	virtual void CleanUp();

protected:
	virtual void UpdateAIDestinationPoint(AIState new_state);
	
	virtual bool LoadFromConfigFile(const char* file_path);
	void LoadAnimationFromJSONObject(JSON_Object *j_object, const char *dotget_path, Animation* animation);
	void LoadSDLRectFromJSONObject(JSON_Object* j_object, const char *dotget_path, SDL_Rect *rect);
	void LoadSoundFXFromJSONObject(JSON_Object* j_object, const char *dotget_path, unsigned int *fx );
	void LoadiPointFromJSONObject(JSON_Object* j_object, const char *dotget_path, iPoint *point);
	Collider* LoadColliderFromJSONObject(JSON_Object* j_object, const char *dotget_path, colliderType type, iPoint *offset);

public: 
	Types m_type = Types::unknown;
	iPoint position = {0, 0};

	int health = 0;
	int max_health = 0;
	int lives = 1;
	int score = 0;
	int help = 0;

	int attack1_dmg = 0;
	int attack2_dmg = 0;
	int attack3_dmg = 0;
	int attack_back_dmg = 0;
	int throw_dmg = 0;

	bool facing_right = true;
	bool is_hittable = true;
	bool is_attacking = false;
	bool is_holding_front = false;
	bool is_holding_back = false;
	bool is_being_hold_front = false;
	bool is_being_hold_back = false;
	bool is_being_thrown_front = false;
	bool is_being_thrown_back = false;

	unsigned int fx_attack_hit = 0;

	int current_combo_hits = 0;
	int combo_remaining_msec = 0;
	int combo_window_msec = 0;
	int current_combo_hold_hits = 0;
	int combo_hold_remaining_msec = 0;
	int combo_hold_window_msec = 0;


protected:
	int	ground_y = 0;
	iPoint speed = {0, 0};
	iPoint move_speed = {0, 0};
	SDL_Rect position_limits = { 28, 162, 264, 55 };	// initial margins
	
	bool grounded = true;
	bool jumping = false;
	int air_remaining_msec = 0;
	int jump_prep_duration = 0;
	int jump_duration = 0;

// hit control
	int unhittable_max_msec = 0;
	int unhittable_remaining_msec = 0;

// throw control 
	iPoint pivot = { 0,0 };

// animation durations
	int blocking_animation_remaining_msec = 0;
	int attacks_duration = 0;
	int attack_pause = 0;
	int hold_attacks_duration = 0;
	int take_item_duration = 0;
	int throwing_duration = 0;
	int holding_swap_duration = 0;
	int being_hold_attack_duration = 0;
	int being_thrown_duration = 0;
	int being_hit_duration = 0; 
	int being_knocked_duration = 0;
	int standing_up_duration = 0;
	int dying_duration = 0;

// Colliders
	Collider *attack_collider = nullptr;
	Collider *hit_collider = nullptr;
	iPoint attack_collider_offset = {0, 0};
	iPoint hit_collider_offset = {0, 0};


// Graphics and animations
	SDL_Texture *graphics = nullptr;
	Animation * current_animation = nullptr;
	iPoint sprite_offset = {0, 0};
	iPoint sprite_offset_flip = {0, 0};

	Animation idle;
	Animation walk;
	Animation take_item;

	Animation attack1;
	Animation attack2;
	Animation attack3;
	Animation attack_back;

	Animation jump_prep;
	Animation jump;
	Animation jump_attack;
	Animation jump_land;

	Animation holding_front;
	Animation holding_front_attack;
	Animation holding_front_attack2;
	Animation holding_back; 
	Animation throwing_front;
	Animation throwing_back;
	Animation holding_swap;
	
	Animation being_hold_front;
	Animation being_hold_front_hit;
	Animation being_hold_back;
	Animation being_hold_attack;
	Animation being_thrown_front;
	Animation being_thrown_back;
	Animation being_hit;
	Animation being_knocked;
	Animation standing_up;

	Animation dying;
	Animation running;
	SDL_Rect shadow = { 0,0 };

// sounds
	unsigned int fx_voice = 0;
	unsigned int fx_jump = 0;
	unsigned int fx_landing_jump = 0;
	unsigned int fx_landing_knocked = 0;
	unsigned int fx_attack_miss = 0;
	unsigned int fx_attack_hit_hard = 0;
	unsigned int fx_ground_hit = 0;
	unsigned int fx_death = 0;
	unsigned int fx_health_restore = 0;
	unsigned int fx_extra_life = 0;
	unsigned int fx_extra_help = 0;
	unsigned int fx_use_help = 0;

// Interactions and collisions
	int layer_depth = 0;
	Entity* held_entity = nullptr;
	Entity* reachable_item = nullptr;

	AIState state = none;
	iPoint AI_move_destination = { 0,0 };

};

#endif // __ENTITY_H__