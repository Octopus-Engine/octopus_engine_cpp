
#include "EntityRallyPointCommand.hh"

#include "logger/Logger.hh"
#include "step/Step.hh"
#include "command/CommandHelpers.hh"
#include "step/command/CommandQueueStep.hh"

namespace octopus
{

EntityRallyPointCommand::EntityRallyPointCommand(Handle const &source_p, Handle const &target_p, Vector const &pos_p, bool targetNotSet_p)
	: Command(source_p)
	, _target(target_p)
	, _pos(pos_p)
	, _targetNotSet(targetNotSet_p)
{}

bool EntityRallyPointCommand::applyCommand(Step & step_p, State const &state_p, CommandData const *, PathManager &) const
{
	Logger::getNormal() << "EntityRallyPointCommand:: entry handle = " << _handleCommand <<", pos = "<< _pos << ", target = "<< _target << ", targetNotSet = "<< _targetNotSet << std::endl;
	Command *cmd_l = newTargetCommand(state_p, _handleCommand, _target, _pos, _targetNotSet);
	if(cmd_l)
	{
		Logger::getNormal() << "EntityRallyPointCommand:: set rally point " << std::endl;
		step_p.addSteppable(new CommandSpawnStep(cmd_l));
	}

	// false here is required because if a next step is issued on the same step as a new command is spawn
	// the newly command is discarded
	return false;
}

} // namespace octopus

