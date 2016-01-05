#ifndef _TIMER_QUEUE_H_
#define _TIMER_QUEUE_H_

#include <tr1/unordered_map>
#include "Common.h"
#include "PriorityQueue.h"
#include "Callback.h"
#include "utils/Time.h"


using namespace std::tr1;
namespace base
{
	/**
	 * @class Timer
	 */
	class Timer : public PQNode<Timer>
	{
	public:
		Timer(uint32_t interval, Callback *cb, uint16_t periodic = 0)
			: _timer_id(0)
			, _interval(interval)
			, _due_time(now64() + _interval)
			, _cb(cb)
			, _periodic(periodic)
			, _expired(0)
		{}

		~Timer()
		{
			if (_cb) delete _cb;
		}

	public:
		/// PQ Node comparer
		int compare(const Timer *node) const
		{
			if (_due_time < node->_due_time)
				return -1;
			else if (_due_time > node->_due_time)
				return 1;
			else
				return 0;
		}

	public:
		/// timer id. 0 if uninitialized
		uint32_t _timer_id;

		/// timer interval in milliseconds
		uint32_t _interval;

		/// due time to timeout
		uint64_t _due_time;

		/// callback
		Callback *_cb;

		/// is periodic ?
		uint16_t _periodic;

		/// is expired ?
		uint16_t _expired;
	};

	/**
	 * @class TimerQueue
	 */
	class TimerQueue
	{
	public:
		TimerQueue()
			: _timerid_seq(1)
		{}

		~TimerQueue();

	public:
		/**
		 * @param periodic  whether the timer is periodic
		 *
		 * @retval uint32_t  added timer id
		 */
		uint32_t add_timer(uint32_t interval, Callback *cb, uint16_t periodic = 0);

		/// cancel timer if found in timerqueue
		inline int cancel_timer(uint32_t timer_id);

		/// change timer if found in timerqueue
		int change_timer(uint32_t timer_id, uint32_t interval, uint16_t periodic);

		/// get lefttime of a timer in milliseconds
		uint32_t time_left(uint32_t timer_id);

		/**
		 * expire timers by "cur_time"
		 *
		 * this method will never block
		 *
		 * @retval  0 a timer is successfully expired
		 * @retval -1 no timer is expired
		 */
		int expire(uint64_t cur_time);

	private:
		/// cancel timer implementation
		int cancel_timer_i(uint32_t timer_id, int remove_from_q = 1);

		/// find timer according to timer id
		Timer *find_timer(uint32_t timer_id, int remove_from_map = 0);

	private:
		PriorityQueue<Timer> _pq;

		typedef unordered_map<uint32_t, Timer *> TimerMap;
		TimerMap _timer_map;

		/// timer id sequence. start with 1
		uint32_t _timerid_seq;
	};
}
#endif
