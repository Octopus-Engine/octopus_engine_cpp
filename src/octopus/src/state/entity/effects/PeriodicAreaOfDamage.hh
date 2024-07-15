#pragma once

#include "state/entity/Unit.hh"

namespace octopus
{

class State;
class Step;

struct PeriodicAreaOfDamageStaticData : public StaticUnitData
{
	/// @brief Create a new UnitData that is a copy of this one
	/// @note a new allocation must be performed
	virtual StaticUnitData* clone() const
	{
		PeriodicAreaOfDamageStaticData * new_l = new PeriodicAreaOfDamageStaticData();
		*new_l = *this;
		return new_l;
	}

	unsigned long long _duration = 250;
	unsigned long long _damageTick = 50;
	Fixed _damage = 10;
	Fixed _range = 3;
	/// @brief Tell if the damage should be applied to allied entities
	bool _friendly_fire = true;

	/// @brief Tell if the area of damage should follow an entity position
	bool _track = false;
	/// @brief Tell if the area of damage should damage the followed entity
	bool _self_damage = false;
	/// @brief Tell if the are of damage should stop when the followed entity dies
	bool _stop_on_death = true;
};

struct PeriodicAreaOfDamageData : public UnitData
{
	/// @brief Create a new UnitData that is a copy of this one
	/// @note a new allocation must be performed
	virtual UnitData* clone() const
	{
		PeriodicAreaOfDamageData * new_l = new PeriodicAreaOfDamageData();
		new_l->copyFrom(this);
		return new_l;
	}
	/// @brief copy everything from another UnitData
	virtual void copyFrom(UnitData const *other_p)
	{
		PeriodicAreaOfDamageData const *data_l = dynamic_cast<PeriodicAreaOfDamageData const *>(other_p);
		*this = *data_l;
	}

	unsigned long _last_tick = 0;
	unsigned long _first_tick = 0;

	Handle _followed_entity;
};

void periodicAreaOfDamageRoutine(Entity const &ent_p, Step & step_p, State const &state_p);

}