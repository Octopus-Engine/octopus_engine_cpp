# ============================================================================
# CMake configuration
# ============================================================================

cmake_minimum_required (VERSION 3.14.0)

# ============================================================================
# Game Engine configuration
# ============================================================================


add_library(octopus
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/building/BuildingAutoBuildCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/building/BuildingBlueprintCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/building/BuildingCancelCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/building/BuildingRallyPointCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/building/BuildingUnitCancelCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/building/BuildingUnitProductionCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/building/BuildingUpgradeProductionCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/Command.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/Commandable.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/CommandHelpers.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/CommandQueue.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/CommandVar.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/data/AttackMoveData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/debug/DebugCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/entity/EntityAbilityCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/entity/EntityAttackCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/entity/EntityAttackMoveCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/entity/EntityBuffCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/entity/EntityBuildingCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/entity/EntityFlockMoveCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/entity/EntityMoveCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/entity/EntityRallyPointCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/entity/EntityWaitCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/flying/eot/DamageOverTime.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/flying/CommandEffectOverTime.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/flying/CommandSpawnUnit.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/player/PlayerChoseOptionCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/spawner/AreaSpawnerCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/spawner/ProceduralTerrain.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/spawner/SpawningGrid.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/unit/UnitDropCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/command/unit/UnitHarvestCommand.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/logger/Logger.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/orca/OrcaManager.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/orca/rvo/Agent.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/orca/rvo/KdTree.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/orca/rvo/Obstacle.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/orca/rvo/RVOSimulator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/CommandSerialization.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/crc/FastCRC.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/StateDump.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/controller/Controller.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/controller/Metrics.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/controller/event/EventCollection.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/controller/score/ScoreTracker.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/controller/trigger/Listener.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/controller/trigger/Trigger.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/buff/AnyBuff.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/buff/Buff.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/buff/Buffer.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/buff/CompositeBuff.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/buff/ConditionalBuff.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/buff/ModifierAoEBuff.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/buff/ModifierBuff.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/buff/TimedBuff.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/effects/PeriodicAreaOfDamage.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/effects/ProjectileEffect.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/Building.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/Entity.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/Resource.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/Unit.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/attackModifier/AttackModifier.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/attackModifier/AttackModifierData.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/attackModifier/modifiers/AoEModifier.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/attackModifier/modifiers/ChainingModifier.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/attackModifier/modifiers/DotModifier.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/attackModifier/modifiers/LifeStealModifier.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/entity/attackModifier/modifiers/SelfDamageModifier.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/model/requirements/Requirements.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/model/entity/BuildingModel.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/model/entity/EntityModel.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/model/entity/TempleModel.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/model/entity/UnitModel.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/player/Player.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/player/upgrade/StepUpgradeNoOpGenerator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/player/upgrade/Upgrade.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/player/upgrade/StepUpgradeTyppedBuffGenerator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/projectile/Projectile.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/vision/PatternHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/vision/VisionGrid.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/vision/VisionHandler.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/Handle.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/state/State.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/ConflictPositionSolver.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/ConstraintPosition.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/Step.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/StepAdditionVisitor.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/TickingStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/CommandDataWaypointStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/CommandIncrementNoProgressStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/CommandMoveUpdateStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/CommandNewTargetStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/CommandQueueStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/CommandSetPositionFromStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/CommandSubAttackStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/CommandUpdateLastPosStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/CommandWindUpDiffStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/MissingResourceStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/ProductionPaidStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/ProductionProgressionStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/data/CancelUnitProductionStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/data/TimerDataUpdateStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/flying/FlyingCommandPopStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/flying/FlyingCommandSpawnStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/command/harvest/CommandHarvestStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/building/BuildingAutoBuildStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/building/BuildingCancelStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/building/BuildingRemoveRallyPointStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/building/BuildingSetRallyPointStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/building/BuildingStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/EntityAttackStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/EntityFrozenStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/EntityHitPointChangeStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/EntityMoveStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/EntityUpdateReloadAbilityStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/EntityUpdateWaitingStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/buff/EntityBuffStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/modifier/AttackModifierStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/spawn/EntitySpawnStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/spawn/UnitSpawnStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/spawn/BuildingSpawnStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/entity/spawn/ResourceSpawnStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerAddBuildingModel.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerAddOptionStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerAttackModAllStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerBuffAllStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerConditionalBuffAllStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerLevelUpUpgradeStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerPopOptionStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerProducedUpgradeStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerSpawnStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerSpendResourceStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/PlayerUpdateBuildingCountStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/bonus/PlayerAddCostBonusStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/player/bonus/PlayerAddTimeProdBonusStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/projectile/ProjectileMoveStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/projectile/ProjectileSpawnStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/resource/ResourceSlotStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/state/StateAddConstraintPositionStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/state/StateDrawStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/state/StateFreeHandleStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/state/StateRemoveConstraintPositionStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/state/StateTemplePositionAddStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/state/StateTemplePositionRemoveStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/state/StateWinStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/team/TeamVisionStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/trigger/TriggerCountChange.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/trigger/TriggerEnableChange.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/trigger/TriggerEntityAddStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/trigger/TriggerEntityResetStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/trigger/TriggerSpawn.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/trigger/TriggerStepCountChange.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/unit/UnitDataStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/unit/UnitHarvestStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/step/vision/VisionChangeStep.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/graph/CanonicalDijkstra.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/graph/Graph.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/graph/Grid.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/graph/GridNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/graph/PathManager.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/library/Library.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/utils/Binary.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/utils/DynamicBitset.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/utils/Fixed.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/utils/FixedPoint.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/utils/RandomGenerator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/utils/ThreadPool.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/utils/Vector.cc
)

target_link_libraries(octopus PRIVATE Boost::graph)

target_include_directories(octopus
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
)

add_subdirectory(test)

# ============================================================================
# Version management
# ============================================================================

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/utils/Version.cc.in ${CMAKE_BINARY_DIR}/Version.cc.in COPYONLY)

ADD_CUSTOM_TARGET(GenerateVersionCPP
                  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake_utils/Generate.cmake
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

target_sources(octopus PRIVATE ${CMAKE_BINARY_DIR}/Version.cc)
ADD_DEPENDENCIES(octopus GenerateVersionCPP)

# ============================================================================
# Installation
# ============================================================================

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/${PROJECT_NAME}-${PROJECT_VERSION}" CACHE PATH "${PROJECT_NAME} default install path" FORCE)
endif ()

install (TARGETS octopus EXPORT ${PROJECT_NAME}-export)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" # source directory
        DESTINATION "include/" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.hh" # select header files
		PATTERN "*Graph.hh" EXCLUDE # exclude this deprecated file (exposing boost headers)
)

set (ConfigPackageLocation "lib/cmake/${PROJECT_NAME}-${PROJECT_VERSION}")
