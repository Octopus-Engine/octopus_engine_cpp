#include "ProjectileEffect.hh"

#include "library/Library.hh"
#include "state/State.hh"
#include "step/Step.hh"

#include "step/entity/EntityHitPointChangeStep.hh"
#include "step/entity/EntityMoveStep.hh"

#include "logger/Logger.hh"

namespace octopus
{

void projectileEffectRoutine(Entity const &ent_p, Step & step_p, State const &state_p)
{
	Unit const &unit_l = static_cast<Unit const &>(ent_p);
	ProjectileEffectData const * data_l = static_cast<ProjectileEffectData const *>(unit_l._unitData.get());
	ProjectileEffectStaticData const * sData_l = static_cast<ProjectileEffectStaticData const *>(unit_l._staticUnitData);

	static Fixed range_check(Fixed::OneAsLong()/10, true);

	// If we reached the target
	if(square_length(ent_p._pos-data_l->_target) < range_check*range_check)
	{
		// stop effect by killing it
		Fixed maxHp_l = unit_l.getHpMax();
		Fixed curHp_l = unit_l._hp + step_p.getHpChange(unit_l._handle);
		step_p.addSteppable(new EntityHitPointChangeStep(unit_l._handle, -10. * maxHp_l, curHp_l, maxHp_l));

		// apply effect
		sData_l->_runnable(step_p, state_p, data_l->_source, Handle(), data_l->_target);
	}
	// Update position to target
	else
	{
		step_p.addSteppable(new EntityMoveStep(unit_l._handle, get_capped_direction(ent_p._pos, data_l->_target, sData_l->_speed)));
	}
}

} // octopus