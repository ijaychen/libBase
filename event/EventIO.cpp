#include "EventIO.h"
#include "Dispatcher.h"
#include "../Logger.h"
#include <unistd.h>

namespace base
{
    namespace event
    {
        using namespace std;

        EventIO::~EventIO()
        {
        }

        void EventIO::AddToDispatcher(int fd, int evt)
        {
            assert(!list_linked());
            m_ioevt = evt;
            if (fd != -1 && fd != m_fd) {
                if (m_fd != -1) {
                    close(m_fd);
                    // remote evt from epoll ?
                }
                m_fd = fd;
            }
            Dispatcher::instance().io_list_.push_front(this);
            int epfd = Dispatcher::instance().epfd_;
            epoll_event ee;
            ee.events = EPOLLET
                        | (m_ioevt & IO_READABLE ?  EPOLLIN : 0)
                        | (m_ioevt & IO_WRITEABLE ? EPOLLOUT : 0);
            ee.data.ptr = this;
            int r = epoll_ctl(epfd, EPOLL_CTL_ADD, m_fd, &ee);
            errno_assert(r == 0);
            Retain();
        }

        void EventIO::ModifyIOEvent(int ioevt)
        {
            assert(list_linked());
            int epfd = Dispatcher::instance().epfd_;
            m_ioevt = ioevt;
            epoll_event ee;
            ee.events = EPOLLET
                        | (m_ioevt & IO_READABLE ?  EPOLLIN : 0)
                        | (m_ioevt & IO_WRITEABLE ? EPOLLOUT : 0);
            ee.data.ptr = this;
            int r = epoll_ctl(epfd, EPOLL_CTL_MOD, m_fd, &ee);
            errno_assert(r == 0);
        }

        void EventIO::CloseFD()
        {
            if (m_fd != -1) {
                close(m_fd);
                m_fd = -1;
            }
        }

        void EventIO::Close()
        {
            if (!m_closed && list_linked()) {
                int epfd = Dispatcher::instance().epfd_;
                epoll_event ee;
                int r = epoll_ctl(epfd, EPOLL_CTL_DEL, m_fd, &ee);
                errno_assert(r == 0);
                Dispatcher::instance().closed_io_list_.push(this);
                m_closed = true;
            }
            CloseFD();
        }
    }
}
