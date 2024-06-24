#include <gtest/gtest.h>

#include <logger/Logger.hh>

#include <command/entity/EntityAttackMoveCommand.hh>
#include <controller/Controller.hh>
#include <step/entity/EntityMoveStep.hh>
#include <step/entity/spawn/UnitSpawnStep.hh>
#include <step/player/PlayerSpawnStep.hh>
#include <step/command/CommandQueueStep.hh>
#include <state/State.hh>

///
/// This test suite aims at checking that EntityAttackMoveCommand works properly
/// - Move an entity along the waypoints
/// - Attack an entity along the way
/// - Attack the target then terminate
///

using namespace octopus;

///
/// > 2 entities
///  - 3, 3 attack moving to 9, 3
///  - 11, 3
/// range is 3 and rays are 1
/// Entity must move from 3,3 to 6,3 to attack (3 steps) + 1 step to capture the target on 11,3
/// Entity then requires 3 steps of windup before attacking and then 10 seconds of reload + 3 seconds of windup to attack again
/// This means first damage should occur on step 7
/// Second damage should occur on step 20
///
///
TEST(attackMoveCommandTest, simple)
{
	octopus::UnitModel unitModel_l { false, 1., 1., 10. };

	Unit ent_l { { 11, 3.5 }, false, unitModel_l};
	ent_l._player = 1;
	ent_l._aggroDistance = 5.;

	Unit ent2_l { { 3, 3.5 }, false, unitModel_l};
	ent2_l._player = 0;
	ent2_l._aggroDistance = 5.;

	UnitSpawnStep * spawn0_l = new UnitSpawnStep(Handle(0), ent2_l);
	UnitSpawnStep * spawn1_l = new UnitSpawnStep(Handle(1), ent_l);

	// entity 0 attack entity 1
	EntityAttackMoveCommand * command_l = new EntityAttackMoveCommand(Handle(0), Handle(0), {9, 3.5}, 0, {{9, 3.5}});
	CommandSpawnStep * commandSpawn_l = new CommandSpawnStep(command_l);

	Controller controller_l({new PlayerSpawnStep(0, 0), new PlayerSpawnStep(1, 1), spawn0_l, spawn1_l, commandSpawn_l}, 1.);

	// query state
	State const * state_l = controller_l.queryState();

	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(3.5, to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);

	// update time to 1second (1)
	controller_l.update(1.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(4., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(3.5, to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);

	// update time to 2 seconds (3)
	controller_l.update(2.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(6., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(3.5, to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);
	EXPECT_NEAR(10., to_double(state_l->getEntity(Handle(1))->_hp), 1e-5);

	// update time to 3 seconds (6)
	controller_l.update(3.);
	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	// wind up should just be over but no damage still
	EXPECT_NEAR(10., to_double(state_l->getEntity(Handle(1))->_hp), 1e-5);


	// update time to 1 second (7)
	controller_l.update(1.);
	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	// damage has been done
	EXPECT_NEAR(7., to_double(state_l->getEntity(Handle(1))->_hp), 1e-5);

	// Next damage should be -> reload time + windup
	// 10 + 3 (13)

	// update time to 12 seconds (19)
	controller_l.update(12.);
	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	// damage has been done
	EXPECT_NEAR(7., to_double(state_l->getEntity(Handle(1))->_hp), 1e-5);

	// update time to 1 second (20)
	controller_l.update(1.);
	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	// damage has been done twice
	EXPECT_NEAR(4., to_double(state_l->getEntity(Handle(1))->_hp), 1e-5);
}
