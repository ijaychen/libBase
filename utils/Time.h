#ifndef _TIMER_H_
#define _TIMER_H_

#include "../Common.h"
#include <sys/time.h>
namespace base
{
	inline uint64_t now64()
	{
		struct timeval tv;
		gettimeofday(&tv, 0);
		return (uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}

	inline uint32_t now()
	{
		return now64();
	}

	inline uint32_t tick()
	{
		return now64();
	}
}
#endif
