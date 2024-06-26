#include "EventCollection.hh"

#include "EventEntityModelDied.hh"
#include "EventEntityModelFinished.hh"

#include "logger/Logger.hh"
#include "state/entity/Building.hh"
#include "step/building/BuildingStep.hh"
#include "step/entity/spawn/BuildingSpawnStep.hh"
#include "step/entity/spawn/EntitySpawnStep.hh"
#include "step/entity/spawn/UnitSpawnStep.hh"
#include "step/entity/EntityHitPointChangeStep.hh"
#include "step/unit/UnitHarvestStep.hh"

namespace octopus
{

EventCollection::EventCollection(State const &state_p, ScoreTracker &scoreTracker_p) : _state(state_p), _scoreTracker(scoreTracker_p)
{}

EventCollection::~EventCollection()
{
	for(EventEntityModelDied * event_l : _listEventEntityModelDied)
	{
		delete event_l;
	}
	for(EventEntityModelFinished * event_l : _listEventEntityModelFinished)
	{
		delete event_l;
	}
}


void EventCollection::visit(BuildingStep const *step_p)
{
	Building const * ent_l = static_cast<Building const *>(_state.getEntity(step_p->_handle));
	Logger::getDebug() << "EventCollection :: visit BuildingStep " << step_p->_handle <<std::endl;

	// track the progresse that have been done just before
	_buildingProgress[step_p->_handle] += step_p->_delta;

	// if building over and not triggered yet for this building
	// check that building is over
	// and that is was not over before applying the step
	if(ent_l->_buildingProgress >= ent_l->_buildingModel._buildingTime
	&& ent_l->_buildingProgress - _buildingProgress[step_p->_handle] < ent_l->_buildingModel._buildingTime
	&& _finishedHandles.find(step_p->_handle) == _finishedHandles.end())
	{
		Logger::getDebug() << "\ttrigger"<<std::endl;
		_finishedHandles.insert(step_p->_handle);
		_listEventEntityModelFinished.push_back(new EventEntityModelFinished(*ent_l, ent_l->_model, ent_l->_player));
	}
}

void EventCollection::visit(BuildingSpawnStep const *step_p)
{
	if(step_p->isForceAlive())
	{
		Logger::getDebug() << "EventCollection :: visit BuildingSpawnStep " << step_p->getHandle() <<std::endl;
		Logger::getDebug() << "\ttrigger"<<std::endl;
		_listEventEntityModelFinished.push_back(new EventEntityModelFinished(*_state.getEntity(step_p->getHandle()), step_p->getModel()._model, step_p->getModel()._player));
	}
}

void EventCollection::visit(EntitySpawnStep const *step_p)
{
	Logger::getDebug() << "EventCollection :: visit EntitySpawnStep " << step_p->getHandle() <<std::endl;
	Logger::getDebug() << "\ttrigger"<<std::endl;
	_listEventEntityModelFinished.push_back(new EventEntityModelFinished(*_state.getEntity(step_p->getHandle()), step_p->getModel()._model, step_p->getModel()._player));
}

void EventCollection::visit(UnitSpawnStep const *step_p)
{
	Entity const * ent_l = _state.getEntity(step_p->getHandle());
	Logger::getDebug() << "EventCollection :: visit UnitSpawnStep " << step_p->getHandle() <<std::endl;
	Logger::getDebug() << "\ttrigger"<<std::endl;
	_listEventEntityModelFinished.push_back(new EventEntityModelFinished(*_state.getEntity(step_p->getHandle()), step_p->getModel()._model, step_p->getModel()._player));
	// update score
	_scoreTracker.add_unit_produced(ent_l->_player);
}

void EventCollection::visit(EntityHitPointChangeStep const *step_p)
{
	Entity const * ent_l = _state.getEntity(step_p->_handle);

	// we rewind the changes
	Fixed newHp_l = ent_l->_hp - _hpChange[step_p->_handle];
	Fixed oldHp_l = newHp_l - step_p->_delta;

	_hpChange[step_p->_handle] += step_p->_delta;

	Logger::getDebug() << "EventCollection :: visit EntityHitPointChangeStep " << step_p->_handle
					   << " : "<<oldHp_l << " => " << newHp_l <<std::endl;

	if(ent_l->_hp <= Fixed::Zero()
	&& newHp_l <= Fixed::Zero() && oldHp_l > Fixed::Zero()
	&& _diedHandles.find(step_p->_handle) == _diedHandles.end())
	{
		Logger::getDebug() << "\ttrigger"<<std::endl;
		_diedHandles.insert(step_p->_handle);
		_listEventEntityModelDied.push_back(new EventEntityModelDied(*ent_l, ent_l->_model, ent_l->_player));
		// update score
		_scoreTracker.add_unit_lost(ent_l->_player);
	}
}

void EventCollection::visit(UnitHarvestDropStep const *step_p)
{
	Entity const * ent_l = _state.getEntity(step_p->_handle);
	Fixed const & qty_l = step_p->_qty;
	_scoreTracker.add_harvested_resource(ent_l->_player, qty_l.to_int());
}


}
