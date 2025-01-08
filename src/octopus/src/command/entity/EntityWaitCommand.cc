
#include "EntityWaitCommand.hh"

#include "logger/Logger.hh"
#include "state/State.hh"
#include "step/Step.hh"
#include "step/entity/EntityMoveStep.hh"

namespace octopus
{

EntityWaitCommand::EntityWaitCommand(Handle const &commandHandle_p, Handle const& source_p)
	: Command(commandHandle_p)
	, _source(source_p)
{}

bool EntityWaitCommand::applyCommand(Step & step_p, State const &state_p, CommandData const *, PathManager &) const
{
	Logger::getNormal() << "EntityWaitCommand:: entry source = " << _source<< std::endl;
	// Use
	step_p.addEntityMoveStep(new EntityMoveStep(createEntityMoveStep(*state_p.getEntity(_source))));

	return true;
}

} // namespace octopus

