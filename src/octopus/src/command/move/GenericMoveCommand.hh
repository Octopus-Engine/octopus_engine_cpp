#ifndef __GenericMoveCommand__
#define __GenericMoveCommand__

#include "logger/Logger.hh"
#include "command/Command.hh"

#include "command/CommandHelpers.hh"
#include "command/CommandQueue.hh"
#include "command/data/ProductionData.hh"
#include "state/Handle.hh"
#include "state/entity/Building.hh"
#include "step/command/CommandQueueStep.hh"
#include "step/Step.hh"

namespace octopus
{

/// @brief This command will select the correct command
/// to be executed based on the target location
class GenericMoveCommand : public Command
{
public:
	GenericMoveCommand() {}

	GenericMoveCommand(std::vector<Handle> const &handles_p, Vector const &target_p)
		: GenericMoveCommand(handles_p, target_p, Handle(), false) {}

	GenericMoveCommand(std::vector<Handle> const &handles_p, Vector const &target_p, Handle const &target_handle)
		: GenericMoveCommand(handles_p, target_p, target_handle, true) {}

	GenericMoveCommand(std::vector<Handle> const &handles_p, Vector const &target_p, Handle const &target_handle, bool target_set) :
		_handles(handles_p), _target(target_p), _target_handle(target_handle), _target_set(target_set)
	{
		if(_handles.size() > 0)
		{
			_handleCommand = _handles[0];
		}
	}

	virtual void registerCommand(Step & step_p, State const &state_p) override
	{
		std::stringstream ss_l;
		ss_l << "GenericMoveCommand:: register Command "<<_target<<" ";
		for(Handle handle_l : _handles)
		{
			ss_l<<handle_l<<" ";
		}
		Logger::getNormal() << ss_l.str() <<std::endl;
		step_p.addSteppable(new CommandStorageStep(this));

		std::list<Handle> flocking_handles_l;
		for(Handle handle_l : _handles)
		{
			Command *cmd_l = newTargetCommand(state_p, handle_l, _target_handle, _target, !_target_set);
			if(dynamic_cast<EntityMoveCommand*>(cmd_l))
			{
				flocking_handles_l.push_back(handle_l);
				delete cmd_l;
			}
			else if(cmd_l)
			{
				cmd_l->setQueued(_queued);
				cmd_l->registerCommand(step_p, state_p);
			}
		}
		if(!flocking_handles_l.empty())
		{
			Command * cmd_l = new EntityFlockMoveCommand(flocking_handles_l, _target, false);

			cmd_l->setQueued(_queued);
			cmd_l->registerCommand(step_p, state_p);
		}
	}

	/// @brief no op
	virtual bool applyCommand(Step & step_p, State const &state_p, CommandData const *data_p, PathManager &pathManager_p) const override { return true; }

	std::vector<Handle> const &getHandles() const { return _handles; }
	Vector const &getTarget() const { return _target; }
	Handle const &getTargetHandle() const { return _target_handle; }
	bool isTargetSet() const { return _target_set; }
private:
	std::vector<Handle> _handles;

	Vector _target;
	Handle _target_handle;
	bool _target_set = false;
};

} // namespace octopus


#endif
