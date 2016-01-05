#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include "../WorldPacket.h"
#include "../net/Client.h"
#include "../Logger.h"
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>
namespace base
{
	namespace net
	{
		class TcpSocket : public Client
		{
		public:
			TcpSocket(memory::MemoryPool& mempool) : Client(mempool){}
			
			virtual void OnConnect()
			{
				LOG_DEBUG("OnConnect=======\n");
			}
			
			void SendPacket(WorldPacket& packet)
			{
				packet.WriteHead();
				Send((const char *)packet.GetBuffer(), packet.GetSize());
			}
			
			void SendInjectedPacket(const WorldPacket &packet, const void *buf, uint32_t len)
			{
				uint16_t opcode = packet.GetOpcode();
				uint16_t size = packet.GetSize();
				WorldPacket injectedPacket(opcode, size + len);
				const uint8_t * src = (const uint8_t *)packet.GetBuffer();
				injectedPacket.WriteBuffer((const uint8_t*)buf, len);
				injectedPacket.WriteBuffer(src + WorldPacket::HEAD_SIZE, size - WorldPacket::HEAD_SIZE);
				SendPacket(injectedPacket);
			}
			
			virtual void OnConnectFail(int eno, const char* reason)	{
				LOG_ERROR("socket connect fail(eno:%d,%s)", eno, reason);
			}
			virtual void OnClose(){	}
			virtual void ProcPacket(WorldPacket & pack) = 0;
			virtual void OnReceive(std::size_t count)
			{		
				// 解析数据包
				while (count >= WorldPacket::HEAD_SIZE) {			
					//LOG_DEBUG("OnReceive(count:%d)",count);
					CopyReceive(m_pkHead.data, sizeof(m_pkHead));
					LOG_DEBUG("m_pkHead.head.size:%d",m_pkHead.head.size);
					if (m_pkHead.head.size >= WorldPacket::HEAD_SIZE) {
						if (count >= m_pkHead.head.size) {
							
							uint16_t opCode = m_pkHead.head.opCode;			
							WorldPacket pkin(opCode, m_pkHead.head.size);
							pkin.Resize(m_pkHead.head.size);
					
							PopReceive((char*)pkin.GetBuffer(), m_pkHead.head.size);
							count -= m_pkHead.head.size;
							ProcPacket(pkin);
						} else {
							// 不足以够成一个完整的数据包
							break;
						}
					} else {
						
					}
				}
			}
			private:
				WorldPacket::PacketHead m_pkHead; 
		};
	}
}
#endif