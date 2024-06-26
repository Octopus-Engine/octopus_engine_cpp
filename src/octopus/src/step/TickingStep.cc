#include "TickingStep.hh"

#include "state/State.hh"
#include "state/entity/buff/Buff.hh"
#include "state/entity/Entity.hh"
#include "state/player/Player.hh"
#include "logger/Logger.hh"

namespace octopus
{

void TickingStep::apply(State &state_p) const
{
	Logger::getDebug() << "TickingStep :: apply "<<std::endl;
	for(Entity *ent_l : state_p.getEntities())
	{
		if(!ent_l->_alive)
		{
			continue;
		}
		++ent_l->_reload;

		for(auto &&pair_l : ent_l->_reloadAbilities)
		{
			++pair_l.second;
		}

		if(state_p.getStepApplied() % 100 == 0)
		{
			ent_l->_hp = std::min(ent_l->_hp + ent_l->getHpRegeneration(), ent_l->getHpMax());
		}

		// Handle buffs
		for(auto &&pair_l : ent_l->_timeSinceBuff)
		{
			std::string const &id_l = pair_l.first;
			unsigned long &time_l = pair_l.second;
			AnyBuff const &buff_l = ent_l->_registeredBuff[id_l];
			unsigned long const duration_l = get_duration(buff_l);
			// need to apply buff
			if(time_l == 0)
			{
				apply_buff(buff_l, *ent_l);
			}
			// need to revert buff (only when we go the exact duration to avoid reverting multiple times)
			if(time_l+1 == duration_l+1 && duration_l != 0)
			{
				revert_buff(buff_l, *ent_l);
			}
			// increment time
			++ time_l;
		}
	}

	state_p.unfoldQueuedFreeHandles();
}

void TickingStep::revert(State &state_p, SteppableData const *data_p) const
{
	Logger::getDebug() << "TickingStep :: revert "<<std::endl;
	TickingData const *data_l = static_cast<TickingData const *>(data_p);

	if(data_l)
	{
		state_p.refoldQueuedFreeHandles(data_l->_queuedFreeHandles);
	}
	else
	{
		state_p.refoldQueuedFreeHandles({});
	}

	for(Entity *ent_l : state_p.getEntities())
	{
		if(ent_l->_reload > 0)
		{
			--ent_l->_reload;
		}

		for(auto &&pair_l : ent_l->_reloadAbilities)
		{
			if(pair_l.second > 0)
			{
				--pair_l.second;
			}
		}

		// reset hp
		if(data_l && ent_l->_handle.index < data_l->_oldHp.size())
		{
			ent_l->_hp = data_l->_oldHp[ent_l->_handle.index];
		}

		// Handle buffs
		for(auto &&pair_l : ent_l->_timeSinceBuff)
		{
			std::string const &id_l = pair_l.first;
			unsigned long &time_l = pair_l.second;
			AnyBuff const &buff_l = ent_l->_registeredBuff[id_l];
			unsigned long const duration_l = get_duration(buff_l);
			// need to revert buff
			if(time_l == 1)
			{
				revert_buff(buff_l, *ent_l);
			}
			// need to apply back buff (only when we go the exact duration to avoid reverting multiple times)
			if(time_l == duration_l+1 && duration_l != 0)
			{
				apply_buff(buff_l, *ent_l);
			}

			if(time_l > 0)
			{
				// decrement time
				-- time_l;
			}
		}
	}
}

bool TickingStep::isNoOp() const
{
	return false;
}

SteppableData * TickingStep::newData(State const &state_p) const
{
	if(state_p.getStepApplied() % 100 != 0)
	{
		return nullptr;
	}
	TickingData * data_l = new TickingData();
	data_l->_oldHp.resize(state_p.getEntities().size());
	for(Entity *ent_l : state_p.getEntities())
	{
		data_l->_oldHp[ent_l->_handle.index] = ent_l->_hp;
	}
	data_l->_queuedFreeHandles = state_p.getFrontQueuedHandles();
	return data_l;
}


} // namespace octopus

