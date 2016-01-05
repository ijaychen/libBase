#ifndef _TIMER_QUEUE_CPP_
#define _TIMER_QUEUE_CPP_

#include "TimerQueue.h"
namespace base
{
	TimerQueue::~TimerQueue()
	{
		for (TimerMap::iterator it = _timer_map.begin();
			 it != _timer_map.end();
			 ++it)
		{
			delete (*it).second;
		}

		_pq.clear();
		_timer_map.clear();
	}

	uint32_t TimerQueue::add_timer(uint32_t interval, Callback *cb, uint16_t periodic)
	{
		// create a timer
		Timer *timer = new Timer(interval, cb, periodic);
		timer->_timer_id = _timerid_seq++;

		// add to containers
		_pq.push(timer);
		_timer_map[timer->_timer_id] = timer;

		// return timer id
		return timer->_timer_id;
	}


	int TimerQueue::cancel_timer(uint32_t timer_id)
	{
		return cancel_timer_i(timer_id);
	}


	int TimerQueue::change_timer(uint32_t timer_id, uint32_t interval, uint16_t periodic)
	{
		Timer *timer;
		//find timer
		timer = find_timer(timer_id);
		if ( timer )
		{
			// update timer data
			timer->_interval = interval;
			timer->_periodic = periodic;

			//update due_time
			timer->_due_time = now64() + timer->_interval;

			// adjust timer node if the timer is NOT expired
			if ( !timer->_expired )
				_pq.adjust_node(timer);

			return 0;
		}
		else
		{
			return -1;
		}
	}


	uint32_t TimerQueue::time_left(uint32_t timer_id)
	{
		//find timer
		Timer *timer = find_timer(timer_id);
		if ( timer )
		{
			int64_t d = timer->_due_time - now64();
			return d > 0 ? uint32_t(d) : 0;
		}
		else
		{
			return 0;
		}
	}


	int TimerQueue::cancel_timer_i(uint32_t timer_id, int remove_from_q)
	{
		Timer *timer;

		//find timer
		timer = find_timer(timer_id, 1);
		if ( timer )
		{
			if ( timer->_expired ) // the timer is expired
			{
				// mark for pending removing
				timer->_periodic = 0;
			}
			else // the timer is NOT expired
			{
				if (remove_from_q) _pq.remove(timer);
				delete timer;
			}

			return 0;
		}
		else
		{
			return -1;
		}
	}


	Timer *TimerQueue::find_timer(uint32_t timer_id, int remove_from_map)
	{
		Timer *timer;

		TimerMap::iterator it = _timer_map.find(timer_id);
		if (it == _timer_map.end())
		{
			timer = 0;
		}
		else
		{
			timer = (*it).second;

			/*
			 * at this stage, if we should remove the timer from map,
			 *  we mean to delete the timer.
			 * so, only remove the timer from map if requested AND the timer is NOT expired.
			 * if the timer is already expired,
			 *  we'll set "periodic" to false in cancel_timer_i() for pending removal
			 */
			if (remove_from_map && !timer->_expired)
			{
				_timer_map.erase(it);
			}
		}
		return timer;
	}


	int TimerQueue::expire(uint64_t cur_time)
	{
		Timer *timer;
		{
			// check running state and queue size
			if ( _pq.empty() ) return -1;

			// check top timer
			timer = _pq.top();

			// calc timeout
			int64_t to = (int64_t)(timer->_due_time - cur_time);

			// check timeout
			if (to > 0) return -1;

			// pop it and mark for expiration
			_pq.pop();
			timer->_expired = 1;

			//reschedule periodic timer
			if ( timer->_periodic )
				timer->_due_time += timer->_interval;
		}

		// invoke callback
		if ( timer->_cb ) timer->_cb->run();

		// handle timer completion
		// reset timer expiration status
		timer->_expired = 0;

		// check periodic
		if ( timer->_periodic )
		{
			// re-add timer to queue if it's periodic,
			_pq.push(timer);
		}
		else
		{
			// delete one shot timer
			cancel_timer_i(timer->_timer_id, 0);
		}
		return 0;
	}
}
#endif
