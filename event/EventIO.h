#ifndef BASE_EVENT_EVENTIO_H
#define BASE_EVENT_EVENTIO_H

#include <cstddef>
#include "../utils/IntrusiveList.h"
#include "../Common.h"

namespace base
{
    namespace event
    {
        enum IOEventType {
            IO_NONE             = 0,    // 无
            IO_READABLE         = 1,    // 可读
            IO_WRITEABLE        = 2,    // 可写
        };

        // IO事件
        // 只支持epoll ET模式触发
        class EventIO : public Object
        {
        public:
            EventIO() : m_ioevt(IO_NONE), 
				m_fd(-1), 
				m_closed(false) {}
            virtual ~EventIO();

            // 是否已关闭
            bool closed() const {
                return m_closed;
            }
            // 事件所关注的文件描述符
            int fd() const {
                return m_fd;
            }
            // 将事件加入到派发器中，开始侦听
            void AddToDispatcher(int fd, int evt);
            // 更新IO事件
            void ModifyIOEvent(int ioevt);
            // 关闭fd
            virtual void Close();

        private:
            void CloseFD();
            // fd可读时
            virtual void OnEventIOReadable() = 0;
            // fd可写时
            virtual void OnEventIOWriteable() = 0;
            // fd关闭时
            virtual void OnEventIOClose() = 0;
			
            int m_ioevt;
            int m_fd;
            bool m_closed;
            INTRUSIVE_LIST(EventIO)
            friend class Dispatcher;
        };
    }
}

#endif // IOEVENT_H
