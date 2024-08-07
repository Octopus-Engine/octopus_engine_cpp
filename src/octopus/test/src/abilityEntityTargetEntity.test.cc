#include <gtest/gtest.h>

#include <logger/Logger.hh>

#include <controller/Controller.hh>
#include <state/State.hh>
#include <state/model/entity/UnitModel.hh>
#include <step/entity/spawn/EntitySpawnStep.hh>
#include <step/player/PlayerSpawnStep.hh>
#include <step/player/PlayerSpendResourceStep.hh>
#include "command/entity/EntityAbilityCommand.hh"
#include "command/entity/EntityMoveCommand.hh"

///
/// This test suite aims at checking that we can have custom abilities
/// targetting a point
///

using namespace octopus;

// spawn 10 bloc
void customAbilityTargetEntity(Step &step_p, State const &, Handle const &, Handle const &, Vector const &)
{
	std::map<std::string, Fixed> map_l;
	map_l["bloc"] = -10.;

	step_p.addSteppable(new PlayerSpendResourceStep(0, map_l));
}

TEST(abilityEntityTargetEntityTest, simple)
{
	EntityModel model_l { false, 1., 1., 10. };
	model_l._abilities["spawn_res"] = {"spawn_res", "spawn_res", customAbilityTargetEntity, alwaysOkChecker, false, true, 2};
	model_l._abilityReloadTime["spawn_res"] = 5;

	Entity ent1_l { { 3, 3. }, false, model_l};
	Entity ent2_l { { 3, 9. }, false, model_l};

	Controller controller_l({
		new PlayerSpawnStep(0, 0),
		new EntitySpawnStep(Handle(0), ent1_l),
		new EntitySpawnStep(Handle(1), ent2_l)
	}, 1.);

	// query state
	State const * state_l = controller_l.queryState();

	EXPECT_NEAR(0., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);

	controller_l.commitCommand(new EntityAbilityCommand(Handle(0), Handle(1), Vector(), "spawn_res"));
	// update time to 1 seconds (1)
	controller_l.update(1.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(0., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);

	// update time to 1 seconds (2)
	controller_l.update(1.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(0., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);

	// update time to 6 seconds (6)
	controller_l.update(4.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(0., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(7., to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);

	// update time to 1 seconds (5)
	controller_l.update(1.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(10., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(7., to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);
}


TEST(abilityEntityTargetEntityTest, moving)
{
	EntityModel model_l { false, 1., 1., 10. };
	model_l._abilities["spawn_res"] = {"spawn_res", "spawn_res", customAbilityTargetEntity, alwaysOkChecker, false, true, 2};
	model_l._abilityReloadTime["spawn_res"] = 5;

	Entity ent1_l { { 3, 3. }, false, model_l};
	Entity ent2_l { { 3, 9. }, false, model_l};

	Controller controller_l({
		new PlayerSpawnStep(0, 0),
		new EntitySpawnStep(Handle(0), ent1_l),
		new EntitySpawnStep(Handle(1), ent2_l)
	}, 1.);

	// query state
	State const * state_l = controller_l.queryState();

	EXPECT_NEAR(0., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);

	controller_l.commitCommand(new EntityAbilityCommand(Handle(0), Handle(1), Vector(), "spawn_res"));
	controller_l.commitCommand(new EntityMoveCommand(Handle(1), Handle(1), {3, 12}, 0, {{3, 12}}, true));
	// update time to 1 seconds (1)
	controller_l.update(1.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(0., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);

	// update time to 1 seconds (2)
	controller_l.update(1.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(0., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);

	// update time to 7 seconds (9)
	controller_l.update(7.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(0., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(10., to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);

	// update time to 1 seconds (5)
	controller_l.update(1.);

	// updated until synced up
	while(!controller_l.loop_body()) {}

	state_l = controller_l.queryState();

	EXPECT_NEAR(10., to_double(getResource(*state_l->getPlayer(0), "bloc")), 1e-5);
	EXPECT_NEAR(3., to_double(state_l->getEntity(Handle(0))->_pos.x), 1e-5);
	EXPECT_NEAR(10., to_double(state_l->getEntity(Handle(0))->_pos.y), 1e-5);
}
