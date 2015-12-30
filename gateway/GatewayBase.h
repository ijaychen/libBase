#ifndef BASE_GATEWAY_GATEWAY_H
#define BASE_GATEWAY_GATEWAY_H

#include "../net/Listener.h"
#include "../net/Client.h"
#include "../WorldPacket.h"
#include <unistd.h>
#include <set>

namespace base
{
	namespace net
	{
		class Listener;
	}
    namespace gateway
    {
        class GatewayBase : public net::Listener::EventHandler
        {
        public:
            DISABLE_COPY(GatewayBase)

            GatewayBase() : listener_(nullptr), mempool_(nullptr), max_connections_(1000){}
            ~GatewayBase() ;
			void SetMemoryPool(memory::MemoryPool* mempool)
			{
				mempool_ = mempool;
			}
			uint32_t GetMaxConnections() const
			{
				return max_connections_;
			}
			
			base::memory::MemoryPool* GetMempool() const
			{
				return mempool_;
			}
           
            // 设置并启动网关
            // ipaddr 绑定的侦听地址
            // port 绑定的端口
            // max_connections 最大连接数，越过最大连接数时，将拒绝连接
            bool Setup(const char* ipaddr, int port, uint32_t max_connections = 3000);

            // 开始清理数据，关闭所有连接 (注意，需要等待所有客户端都已经全部清除后，方可释放gateway)
            //void BeginCleanup(boost::function<void()> cb) ;
            // 广播
            virtual void Broadcast(WorldPacket& packet) = 0;
            // 关闭侦听器
            void StopListener();
			
			virtual void OnListenerAccept(net::Listener* sender, int clientfd) = 0;    
		protected:
            net::Listener* listener_;
            base::memory::MemoryPool* mempool_;
            // 所有客户端列表            
            uint32_t max_connections_;
        };
    }
}

#endif // GATEWAY_H
