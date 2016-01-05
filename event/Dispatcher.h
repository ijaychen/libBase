#ifndef BASE_EVENT_DISPATCHER_H
#define BASE_EVENT_DISPATCHER_H

#include "../utils/IntrusiveList.h"
#include <cstddef>
#include <stdint.h>
#include <map>
#include <queue>
#include <sys/epoll.h>

namespace base
{
    namespace event
    {
        class EventIO;
       
        // 事件派发器　（定时器事件，ＩＯ事件)
        class Dispatcher
        {      
        public:
            static Dispatcher& instance() {
                static Dispatcher ins;
                return ins;
            }
            ~Dispatcher();         

            // 开始事件主循环
            void Dispatch();

            // 强制清除所有事件
            void Clear();

            // 正常退出（直到没有未决的事件）
            void NormalExit();
			
			bool IsErase(EventIO* evi)
			{
				return !io_list_.contains(evi);
			}
        private:
            Dispatcher();         
            utils::IntrusiveList<EventIO> io_list_;
            std::queue<EventIO*> closed_io_list_;
            bool exit_;           
            int epfd_;
            int wait_;
            static const int EVT_BUF_SIZE = 256;
            epoll_event evtbuf_[EVT_BUF_SIZE];
            friend class EventIO;
        };
    }
}

extern base::event::Dispatcher* g_dispatcher;

#endif // EVENTDISPATCHER_H
