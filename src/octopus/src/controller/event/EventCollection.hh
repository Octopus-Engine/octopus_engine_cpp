#ifndef __EventCollection__
#define __EventCollection__

#include <list>
#include <unordered_set>
#include <unordered_map>

#include "controller/score/ScoreTracker.hh"
#include "state/Handle.hh"
#include "step/Steppable.hh"
#include "utils/Fixed.hh"

namespace octopus
{
class EventEntityModelDied;
class EventEntityModelFinished;
class State;

/// @brief store event visiting a step
/// @warning visitor is not stateless therefore create one
/// visitor per step visited
/// @warning calling procedure is to visit the step that has just been applied to the state
class EventCollection : public SteppableVisitor
{
public:
	EventCollection(State const &state_p, ScoreTracker &scoreTracker_p);
	~EventCollection();

	std::list<EventEntityModelDied *> _listEventEntityModelDied;
	std::list<EventEntityModelFinished *> _listEventEntityModelFinished;

	// may create event
	virtual void visit(BuildingStep const *);
	virtual void visit(EntityHitPointChangeStep const *);
	virtual void visit(BuildingSpawnStep const *);
	virtual void visit(EntitySpawnStep const *);
	virtual void visit(UnitSpawnStep const *);
	virtual void visit(UnitHarvestDropStep const *);

	/// NA
	virtual void visit(AttackModifierStep const *) {}
	virtual void visit(BuildingAutoBuildStep const *) override {}
	virtual void visit(BuildingCancelStep const *) {}
	virtual void visit(BuildingRemoveRallyPointStep const *) {}
	virtual void visit(BuildingSetRallyPointStep const *) {}
    virtual void visit(CancelUnitProductionStep const *) {}
	virtual void visit(CommandAddSubCommandStep const *) {}
	virtual void visit(CommandDataWaypointAddStep const *) {}
	virtual void visit(CommandDataWaypointRemoveStep const *) {}
	virtual void visit(CommandDataWaypointSetStep const *) {}
	virtual void visit(CommandDelSubCommandStep const *) {}
	virtual void visit(CommandDepositChangeStep const *) {}
	virtual void visit(CommandHarvestingChangeStep const *) {}
	virtual void visit(CommandHarvestTimeSinceHarvestStep const *) {}
	virtual void visit(CommandHarvestPointChangeStep const *) {}
	virtual void visit(CommandIncrementNoProgressStep const *) {}
	virtual void visit(CommandMoveUpdateStep const *) {}
	virtual void visit(CommandMoveLosStep const *) {}
	virtual void visit(CommandMoveUnlockRoutineStep const *) override {}
	virtual void visit(CommandUpdateFlockingReached const *) override {}
	virtual void visit(CommandMoveStepSinceUpdateIncrementStep const *) {}
	virtual void visit(CommandNewTargetStep const *) {}
	virtual void visit(CommandNextStep const *) {}
	virtual void visit(CommandUpdateLastIdStep const *) override {}
	virtual void visit(CommandResourceChangeStep const *) {}
	virtual void visit(CommandSetPositionFromStep const *) {}
	virtual void visit(CommandSpawnStep const *) {}
	virtual void visit(CommandStorageStep const *) {}
	virtual void visit(CommandUpdateLastPosStep const *) {}
	virtual void visit(CommandWindUpDiffStep const *) {}
	virtual void visit(EntityAttackStep const *) {}
	virtual void visit(EntityBuffStep const *) {}
	virtual void visit(EntityConditionalBuffStep const *) {}
	virtual void visit(EntityFrozenStep const *) {}
	virtual void visit(EntityMoveStep const *) {}
	virtual void visit(EntityUpdateReloadAbilityStep const *) override {}
	virtual void visit(EntityUpdateWaitingStep const *) {}
	virtual void visit(FlyingCommandSpawnStep const *) {}
	virtual void visit(FlyingCommandPopStep const *) {}
	virtual void visit(MissingResourceStep const *) {}
	virtual void visit(PlayerAddBuildingModel const *) {}
	virtual void visit(PlayerAddCostBonusStep const *) {}
	virtual void visit(PlayerAddOptionStep const *) {}
	virtual void visit(PlayerAddTimeProdBonusStep const *) {}
    virtual void visit(PlayerAttackModAllStep const *) {}
	virtual void visit(PlayerBuffAllStep const *) {}
	virtual void visit(PlayerConditionalBuffAllStep const *) {}
	virtual void visit(PlayerLevelUpUpgradeStep const *) {}
	virtual void visit(PlayerPopOptionStep const *) {}
	virtual void visit(PlayerProducedUpgradeStep const *) {}
	virtual void visit(PlayerSpawnStep const *) {}
	virtual void visit(PlayerSpendResourceStep const *) {}
	virtual void visit(PlayerUpdateBuildingCountStep const *) override {}
	virtual void visit(ProductionPaidStep const *) {}
	virtual void visit(ProductionProgressionStep const *) {}
	virtual void visit(ProjectileMoveStep const *) override {}
	virtual void visit(ProjectileSpawnStep const *) override {}
	virtual void visit(ResourceSlotStep const *) {}
	virtual void visit(ResourceSpawnStep const *) {}
	virtual void visit(StateAddConstraintPositionStep const *) override {}
	virtual void visit(StateDrawStep const *) override {}
	virtual void visit(StateFreeHandleStep const *) override {}
	virtual void visit(StateRemoveConstraintPositionStep const *) override {}
	virtual void visit(StateTemplePositionAddStep const *) override {}
	virtual void visit(StateTemplePositionRemoveStep const *) {}
	virtual void visit(StateWinStep const *) {}
	virtual void visit(TeamVisionStep const *) {}
	virtual void visit(TickingStep const *) {}
	virtual void visit(TimerDataUpdateStep const *) {}
	virtual void visit(TriggerCountChange const *) {}
	virtual void visit(TriggerEnableChange const *) {}
	virtual void visit(TriggerEntityAddStep const *) {}
	virtual void visit(TriggerEntityResetStep const *) {}
	virtual void visit(TriggerSpawn const *) {}
	virtual void visit(TriggerStepCountChange const *) {}
	virtual void visit(UnitHarvestQuantityStep const *) {}
	virtual void visit(UnitDataStep const *) {}
	virtual void visit(UnitHarvestTypeStep const *) {}
	virtual void visit(VisionChangeStep const *) {}

	State const &getState() const { return _state; }
private:
	State const &_state;
	ScoreTracker &_scoreTracker;
	/// statefull data
	std::unordered_set<Handle> _finishedHandles;
	std::unordered_map<Handle, Fixed> _buildingProgress;

	std::unordered_set<Handle> _diedHandles;
	std::unordered_map<Handle, Fixed> _hpChange;
};

}

#endif
