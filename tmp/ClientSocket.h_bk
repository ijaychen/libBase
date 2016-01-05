#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "../net/TcpSocket.h"
#include "../Logger.h"
#include <string>

#define CMSG_LOGIN 15

class ClientSocket : public base::net::TcpSocket
{
public:
	ClientSocket(base::memory::MemoryPool& mempool) : TcpSocket(mempool)
	{
		
	}

	void ProcPacket(base::WorldPacket & pack)
	{
		uint16_t opcode = pack.GetOpcode();
		LOG_DEBUG("ProcPacket(opcode:%d)\n", opcode);
		switch(opcode)
		{
		case CMSG_LOGIN:
		{
			std::string server_name, account_name, passwd;
			server_name = pack.ReadString();
			account_name = pack.ReadString();
			passwd = pack.ReadString();
			LOG_DEBUG("server_name:%s, account_name:%s,password:%s", server_name.c_str(),account_name.c_str(),passwd.c_str());
			base::WorldPacket injectedPacket(pack);
			uint32_t actorId = 80000000;
			SendInjectedPacket(injectedPacket, &actorId, sizeof(actorId));
			break;
		}		
		default:
		break;	
		}		
	}	
};

#endif