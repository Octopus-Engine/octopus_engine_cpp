#ifndef __BuildingGroupProductionCommand__
#define __BuildingGroupProductionCommand__

#include "command/Command.hh"

#include "command/CommandHelpers.hh"
#include "command/CommandQueue.hh"
#include "command/data/ProductionData.hh"
#include "state/Handle.hh"
#include "state/entity/Building.hh"
#include "step/command/CommandQueueStep.hh"

namespace octopus
{

inline long long remainingQueueTime(Building const &building_p, State const &state_p)
{
	// remaining queue time
	long long time_l = 0;

	if(building_p.getQueue().hasCommand())
	{
	    for(CommandBundle const &bundle_l : building_p.getQueue().getList())
		{
			ProductionData const *data_l = dynamic_cast<ProductionData const *>(getData(bundle_l._var));
            if(!data_l)
            {
                continue;
            }

            Player const &player_l = *state_p.getPlayer(building_p._player);
            Fixed completeTime_l = data_l->getCompleteTime(player_l);

			time_l += completeTime_l.to_int() - data_l->_progression.to_int();
		}
	}

	return time_l;
}

template<typename production_t>
Handle getBestProductionBuilding(std::vector<Handle> const &handles_p, State const &state_p, production_t const &model_p)
{
    bool found_l = false;
    Handle best_l {std::numeric_limits<unsigned long>::max(), 0};
    long long lowestQueue_l = 0;
    for(Handle const &idx_l : handles_p)
    {
        // check ent
        Entity const *ent_l = state_p.getEntity(idx_l);
        // skip non building
        if(!ent_l->_model._isBuilding)
        {
            continue;
        }
        Building const *building_l = dynamic_cast<Building const *>(ent_l);

        // skip if cannot produce model
        if(!building_l
		|| !building_l->_buildingModel.canProduce(&model_p)
        || !building_l->isBuilt())
        {
            continue;
        }

        // get production time queued up
        long long queueTime_l = remainingQueueTime(*building_l, state_p);

        if(!found_l || queueTime_l < lowestQueue_l)
        {
            found_l = true;
            best_l = idx_l;
            lowestQueue_l = queueTime_l;
        }
    }

    return best_l;
}

/// @brief This command will select the best production
/// for the production entity
template<typename ProductionCommand_t, typename Production_t>
class BuildingGroupProductionCommand : public Command
{
public:
	BuildingGroupProductionCommand() {}
	BuildingGroupProductionCommand(std::vector<Handle> const &handles_p, Production_t const &model_p) :
		_handles(handles_p), _model(&model_p)
	{}

	/// @brief register the command into the step
	/// This method is responsible to
	/// handle cost of command and spawning command in step
	virtual void registerCommand(Step & step_p, State const &state_p) override
	{
		step_p.addSteppable(new CommandStorageStep(this));
		if(!_model) return;

        Handle best_l = getBestProductionBuilding(_handles, state_p, *_model);

        if(best_l.index < state_p.getEntities().size())
        {
            ProductionCommand_t *cmd_l = new ProductionCommand_t(best_l, best_l, *_model);
            cmd_l->setQueued(_queued);
			cmd_l->registerCommand(step_p, state_p);
        }

	}

	/// @brief no op
	virtual bool applyCommand(Step & step_p, State const &state_p, CommandData const *data_p, PathManager &pathManager_p) const override { return true; }
private:
	std::vector<Handle> _handles;

	Production_t const *_model {nullptr};
};

} // namespace octopus


#endif
