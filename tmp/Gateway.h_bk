#ifndef GATE_WAY_H
#define GATE_WAY_H

#include "../gateway/GatewayBase.h"
#include "../Logger.h"
#include "ClientSocket.h"
#include "../Singleton.h"
using namespace base;

typedef ClientSocket* ClientPtr;
typedef std::set<ClientPtr> client_set_t;
class Gateway : public base::gateway::GatewayBase, public base::Singleton<Gateway>
{
	friend class Singleton<Gateway>;
public:	
	virtual ~Gateway()
	{
		Release();
	}	
	virtual void OnListenerAccept(base::net::Listener* sender, int clientfd)
	{
		if (m_clients.size() >= GetMaxConnections()) 
		{
			LOG_WARN("gateway refuse connect because the maximum number of connections has been exceeded!\n");
			close(clientfd);
		} 
		else 
		{
			ClientSocket* client = new ClientSocket(*GetMempool());
			client->Connect(clientfd);
		}
	}
	
	virtual void OnClientConnect(ClientPtr client)
	{
		if(nullptr != client)
		{
			m_clients.insert(client);
		}
	}
	
	virtual void Broadcast(WorldPacket& pktout)
	{
		/*for (client_set_t::iterator it = clients_.begin(); it != clients_.end(); ++it) {
			(*it)->Send(pktout);
		}*/
	}
private:
	void Release()
	{
		StopListener();
		for (client_set_t::iterator it = m_clients.begin(); it != m_clients.end(); ++it) 
		{
			(*it)->Close();
		}		
	}
private:
	std::set<ClientPtr> m_clients;
};
#define sGateway (*Gateway::Instance())
#endif