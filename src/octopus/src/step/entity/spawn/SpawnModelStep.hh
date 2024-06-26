#ifndef __SPAWN_MODEL_STEP__
#define __SPAWN_MODEL_STEP__

#include "state/entity/buff/Buff.hh"
#include "state/entity/Entity.hh"
#include "state/State.hh"
#include "state/player/Player.hh"
#include "state/Handle.hh"
#include "step/Steppable.hh"
#include "step/entity/buff/EntityBuffStep.hh"
#include "utils/Vector.hh"

namespace octopus
{

class AbstractSpawnModelStep : public Steppable
{
public:
	Entity const * getEntityTemplate() const { return _template; }

protected:
	Entity const * _template { nullptr };
};

template<typename class_t>
class SpawnModelStep : public AbstractSpawnModelStep
{
public:
	SpawnModelStep(Handle const &handle_p, class_t const &model_p, bool forceAlive_p=false) : _handle(handle_p), _model(model_p), _forceAlive(forceAlive_p)
	{
		_template = &_model;
	}

	virtual void apply(State &state_p) const override
	{
		if(!state_p.hasEntity(_handle))
		{
			Handle check_l = state_p.addEntity(new class_t(_model));
			if(check_l != _handle)
			{
				throw std::logic_error("Spawn Step did not have the same handle across multiple states");
			}
		}

		Entity * ent_l = state_p.getEntity(this->_handle);
		if(!ent_l->_model._isBuilding
		|| _forceAlive)
		{
			ent_l->_alive = true;
		}
		else
		{
			ent_l->_alive = false;
		}
		if(ent_l->_alive)
		{
			updateGrid(state_p, ent_l, true);
			updateVisionGrid(state_p, ent_l, true);
			updateExplorationGrid(state_p, ent_l, true);
		}

		Player *player_l = state_p.getPlayer(ent_l->_player);
		// enable player buffs to the spawned entity
		for(AnyBuff const &buff_l : player_l->_mapBuffs[ent_l->_model._id])
		{
			ent_l->_timeSinceBuff[get_id(buff_l)] = 0;
			ent_l->_registeredBuff[get_id(buff_l)] = buff_l;
		}
		for(AnyBuff const &buff_l : player_l->_mapBuffs[""])
		{
			ent_l->_timeSinceBuff[get_id(buff_l)] = 0;
			ent_l->_registeredBuff[get_id(buff_l)] = buff_l;
		}
		for(ConditionalBuff const &buff_l : player_l->_mapConditionalBuffs[ent_l->_model._id])
		{
			// use step to avoid duplicated code
			EntityConditionalBuffStep buffStep_l(ent_l->_handle, buff_l);
			buffStep_l.apply(state_p);
		}

		auto &&itModifier_l = player_l->_mapModifiers.find(ent_l->_model._id);
		if(itModifier_l != player_l->_mapModifiers.end())
		{
			ent_l->_attackMod = itModifier_l->second;
		}
	}
	virtual void revert(State &state_p, SteppableData const *) const override
	{
		if(state_p.getEntities().back()->_handle != this->_handle)
		{
			throw std::logic_error("Spawn Step revert is incoherrent (steps seem to be reverted in the wrong order)");
		}
		// unspawn but do not delete
		Entity * ent_l = state_p.getEntity(this->_handle);
		state_p.getEntities().pop_back();
		delete ent_l;
	}

	virtual bool isNoOp() const override
	{
		return false;
	}

	Handle const &getHandle() const { return _handle; }
	class_t const &getModel() const { return _model; }
	bool isForceAlive() const { return _forceAlive; }
protected:
	Handle const _handle {0};
	bool const _forceAlive;

	class_t _model;
};


}

#endif
