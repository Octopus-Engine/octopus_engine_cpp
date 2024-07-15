#include "PeriodicAreaOfDamage.hh"

#include "library/Library.hh"
#include "state/State.hh"
#include "step/Step.hh"

#include "step/entity/EntityHitPointChangeStep.hh"
#include "step/entity/EntityMoveStep.hh"
#include "step/custom/implem/ImpactStep.hh"
#include "step/unit/UnitDataStep.hh"

#include "logger/Logger.hh"

namespace octopus
{

void periodicAreaOfDamageRoutine(Entity const &ent_p, Step & step_p, State const &state_p)
{
	Unit const &unit_l = static_cast<Unit const &>(ent_p);
	PeriodicAreaOfDamageData const * data_l = static_cast<PeriodicAreaOfDamageData const *>(unit_l._unitData.get());
	PeriodicAreaOfDamageStaticData const * sData_l = static_cast<PeriodicAreaOfDamageStaticData const *>(unit_l._staticUnitData);
	PeriodicAreaOfDamageData * new_data_l = static_cast<PeriodicAreaOfDamageData *>(data_l->clone());
	bool change_done_l = false;
	unsigned long long step_l = step_p.getId();

	// If tracking entity we update position and check liveliness
	if(sData_l->_track)
	{
		if(!state_p.isEntityAlive(new_data_l->_followed_entity))
		{
			// stop effect by killing it
			if(sData_l->_stop_on_death)
			{
				Fixed maxHp_l = unit_l.getHpMax();
				Fixed curHp_l = unit_l._hp + step_p.getHpChange(unit_l._handle);
				step_p.addSteppable(new EntityHitPointChangeStep(unit_l._handle, -10. * maxHp_l, curHp_l, maxHp_l));
			}
		}
		// if entity alive we update position
		else
		{
			Entity const * ent_l = state_p.getEntity(new_data_l->_followed_entity);
			Vector move_l = ent_l->_pos - unit_l._pos;
			step_p.addSteppable(new EntityMoveStep(unit_l._handle, move_l));
		}
	}

	// If tick we do damage
	if(step_l >= new_data_l->_last_tick + sData_l->_damageTick
	|| new_data_l->_first_tick == 0)
	{
		new_data_l->_last_tick = step_l;
		if(new_data_l->_first_tick == 0)
		{
			new_data_l->_first_tick = step_l;
		}
		change_done_l = true;

		// get damaged units
		TargetPanel panel_l = lookUpNewTargets(state_p, unit_l._handle, sData_l->_range, !sData_l->_friendly_fire);
		for(Entity const *ent_l : panel_l.units)
		{
			// Skip the followed entity if necessary
			if(sData_l->_track
			&& !sData_l->_self_damage
			&& ent_l->_handle == new_data_l->_followed_entity)
			{
				continue;
			}

			// skip invulnerable entities
			if(ent_l->_model._invulnerable)
			{
				continue;
			}

			// damage
			Fixed curHp_l = ent_l->_hp + step_p.getHpChange(ent_l->_handle);
			Fixed maxHp_l = ent_l->getHpMax();
			step_p.addSteppable(new EntityHitPointChangeStep(ent_l->_handle, -sData_l->_damage, curHp_l, maxHp_l));
			// impact
			step_p.addSteppable(new ImpactStep(unit_l._model._id, ent_l->_pos));
		}
	}

	// If done we end
	if(step_l >= new_data_l->_first_tick + sData_l->_duration)
	{
		Fixed maxHp_l = unit_l.getHpMax();
		Fixed curHp_l = unit_l._hp + step_p.getHpChange(unit_l._handle);
		step_p.addSteppable(new EntityHitPointChangeStep(unit_l._handle, -10. * maxHp_l, curHp_l, maxHp_l));
	}

	if(change_done_l)
	{
		Logger::getDebug()<<"PeriodicAreaOfDamageRoutine :: adding change"<<std::endl;
		step_p.addSteppable(new UnitDataStep(unit_l._handle, new_data_l));
	}
	else
	{
		delete new_data_l;
	}
}

} // octopus