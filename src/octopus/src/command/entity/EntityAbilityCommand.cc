
#include "EntityAbilityCommand.hh"

#include "logger/Logger.hh"
#include "state/State.hh"
#include "state/entity/Entity.hh"
#include "step/Step.hh"
#include "step/command/CommandQueueStep.hh"
#include "step/entity/EntityUpdateReloadAbilityStep.hh"
#include "step/command/CommandDataWaypointStep.hh"

namespace octopus
{

EntityAbilityCommand::EntityAbilityCommand(Handle const &commandHandle_p, Handle const &target_p, Vector const &pointTarget_p, std::string const &id_p)
	: Command(commandHandle_p)
    , _target(target_p)
    , _pointTarget(pointTarget_p)
    , _id(id_p)
	, _moveCommand(commandHandle_p, commandHandle_p, {0,0}, 0, {}, false, false, true)
{}

void EntityAbilityCommand::registerCommand(Step & step_p, State const &state_p)
{
	Entity const * ent_l = state_p.getEntity(_handleCommand);
	Player const * player_l = state_p.getPlayer(ent_l->_player);

    Ability const & ability_l = getAbility(ent_l->_model, _id);
    // check reload time
    unsigned long reload_l = getReloadAbilityTime(*ent_l, ability_l._reloadKey, getMinReloadTime(ent_l->_model, ability_l._reloadKey));
    if(reload_l >= getMinReloadTime(ent_l->_model, ability_l._reloadKey)
	&& meetRequirements(ability_l._requirements, *player_l)
    && ability_l._checker(step_p, state_p, _handleCommand, _target, _pointTarget)
    && step_p.checkAbilityNotAlreadyRegistered(_handleCommand, ability_l._reloadKey))
    {
        // spaw command
	    step_p.addSteppable(new CommandSpawnStep(this));
    }
}

bool inRange(State const &state_p, Handle const & source_p, Vector const & target_p, Fixed const &range_p)
{
	Entity const * entSource_l = state_p.getEntity(source_p);

	// direction (source -> target)
	Vector dir_l = target_p - entSource_l->_pos;
	// square distances
	Fixed sqDistance_l = square_length(dir_l);
	// range squared (+ rays)
	Fixed rangeAndRays_l = range_p + entSource_l->_model._ray;
	Fixed sqRange_l = rangeAndRays_l * rangeAndRays_l;

	return sqDistance_l < sqRange_l;
}

bool EntityAbilityCommand::applyCommand(Step & step_p, State const &state_p, CommandData const *data_p, PathManager &pathManager_p) const
{
	MoveData const &data_l = *static_cast<MoveData const *>(data_p);
	Logger::getDebug() << "EntityAbilityCommand:: apply Command "<<_handleCommand <<std::endl;

	Entity const * ent_l = state_p.getEntity(_handleCommand);

    Ability const & ability_l = getAbility(ent_l->_model, _id);

    // check reload time
    unsigned long reload_l = getReloadAbilityTime(*ent_l, ability_l._reloadKey, getMinReloadTime(ent_l->_model, ability_l._reloadKey));

	Vector targetPoint_l = _pointTarget;
	bool requireTarget_l = ability_l._requireTargetHandle || ability_l._requireTargetPoint;
	if(ability_l._requireTargetHandle && state_p.isEntityAlive(_target))
	{
		targetPoint_l = state_p.getLoseEntity(_target.index)->_pos;
	}

	// If target died
	if(ability_l._requireTargetHandle && !state_p.isEntityAlive(_target))
	{
		return true;
	}

	if(requireTarget_l)
	{
		// we need to move
		if(!inRange(state_p, _handleCommand, targetPoint_l, ability_l._range))
		{
			// update the target
			Vector diff_l = targetPoint_l - data_l._finalPoint;
			if(square_length(diff_l) > 1.)
			{
				Logger::getDebug() << "\t\tEntityAbilityCommand:: changed target "<< _handleCommand
								   << " to " << targetPoint_l.x.to_double()<<";"<<targetPoint_l.y.to_double() <<std::endl;
				step_p.addSteppable(new CommandDataWaypointSetStep(_handleCommand, data_l._waypoints, {targetPoint_l}));
			}
			// just move
			Logger::getDebug() << "\t\tEntityAbilityCommand:: adding move step "
								<< _handleCommand << " target " << targetPoint_l.x.to_double()<<";"<<targetPoint_l.y.to_double()
								<< " speed " <<ent_l->getStepSpeed().to_double()<<std::endl;
			// add move command
			_moveCommand.applyCommand(step_p, state_p, data_p, pathManager_p);
			return false;
		}
		// can run
		else
		{
			// reset reload time
			step_p.addSteppable(new EntityUpdateReloadAbilityStep(_handleCommand, ability_l._reloadKey, reload_l, 1));

			ability_l._runnable(step_p, state_p, _handleCommand, _target, _pointTarget);
			return true;
		}
	}
	else
	{
        // reset reload time
        step_p.addSteppable(new EntityUpdateReloadAbilityStep(_handleCommand, ability_l._reloadKey, reload_l, 1));

		ability_l._runnable(step_p, state_p, _handleCommand, _target, _pointTarget);
		return true;
	}

	return false;
}

void EntityAbilityCommand::cleanUp(Step & step_p, State const &state_p, CommandData const *data_p) const
{
	_moveCommand.cleanUp(step_p, state_p, data_p);
}

} // namespace octopus

