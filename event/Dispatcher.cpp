#include "Dispatcher.h"
#include "EventIO.h"
#include "../AutoreleasePool.h"
#include "../Logger.h"
#include <unistd.h>
#include <sys/epoll.h>

base::event::Dispatcher* g_dispatcher = nullptr;

namespace base
{
    namespace event
    {
        using namespace std;

        /// EventDispatcher
        Dispatcher::Dispatcher()
            : exit_(false), epfd_(-1), wait_(50)
        {
            assert(g_dispatcher == nullptr);
            epfd_ = epoll_create(9999);
            errno_assert(epfd_ > 0);           
            g_dispatcher = this;
        }

        Dispatcher::~Dispatcher()
        {
            close(epfd_);
            g_dispatcher = nullptr;
        }

        void Dispatcher::NormalExit()
        {
            exit_ = true;
        }

        void Dispatcher::Clear()
        {
            // TODO
        }

        void Dispatcher::Dispatch()
        {           
			//LOG_DEBUG("Dispatch(io_list size:%d)\n",io_list_.size());
            int n = 0;
            //while (true) 
			{                 
                if (exit_) 
				{
                    if (io_list_.empty() && closed_io_list_.empty()) 
					{                        
                        //break;
                    } 					
                }
                n = epoll_wait(epfd_, evtbuf_, EVT_BUF_SIZE, wait_);
                if (n <= -1) 
				{
                    // error
                    // TODO error handle
                } 
				else if (n == 0) 
				{
                    // timeout
                } 
				else 
				{
                    // handle io event
                    for (int i = 0; i < n; ++i) 
					{
						LOG_DEBUG("epoll_wait size:%d\n", n);
                        EventIO* evobj = static_cast<EventIO*>(evtbuf_[i].data.ptr);
                        if (!evobj->closed() && ((evtbuf_[i].events & EPOLLIN) | (evtbuf_[i].events & EPOLLERR))) 
						{
                            evobj->OnEventIOReadable();
                        }
                        if (!evobj->closed() && (evtbuf_[i].events & EPOLLOUT)) 
						{
                            evobj->OnEventIOWriteable();
                        }
                    }
                }

                while (!closed_io_list_.empty()) {
                    EventIO* cur = closed_io_list_.front();
                    cur->OnEventIOClose();
                    io_list_.erase(cur);
                    cur->Release();
                    closed_io_list_.pop();
                }
            }
        }
    }
}
