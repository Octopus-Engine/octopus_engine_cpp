#pragma once

#include "state/entity/Unit.hh"

namespace octopus
{

class State;
class Step;

struct ProjectileEffectStaticData : public StaticUnitData
{
	/// @brief Create a new UnitData that is a copy of this one
	/// @note a new allocation must be performed
	virtual StaticUnitData* clone() const
	{
		ProjectileEffectStaticData * new_l = new ProjectileEffectStaticData();
		*new_l = *this;
		return new_l;
	}
	/// @brief projectile step speed
	Fixed _speed;
	/// @brief The runnable effect
	std::function<void(Step &, State const &, Handle const &, Handle const &, Vector const &)> _runnable;
};

struct ProjectileEffectData : public UnitData
{
	/// @brief Create a new UnitData that is a copy of this one
	/// @note a new allocation must be performed
	virtual UnitData* clone() const
	{
		ProjectileEffectData * new_l = new ProjectileEffectData();
		new_l->copyFrom(this);
		return new_l;
	}
	/// @brief copy everything from another UnitData
	virtual void copyFrom(UnitData const *other_p)
	{
		ProjectileEffectData const *data_l = dynamic_cast<ProjectileEffectData const *>(other_p);
		*this = *data_l;
	}

	/// @brief position of the target of the projectile
	Vector _target;
	/// @brief source of the projectile
	Handle _source;
};

void projectileEffectRoutine(Entity const &ent_p, Step & step_p, State const &state_p);

}