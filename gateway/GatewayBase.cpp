#include "GatewayBase.h"

namespace base
{
    namespace gateway
    {
        GatewayBase::~GatewayBase()
        {
            SAFE_RELEASE(listener_);
        }

        bool GatewayBase::Setup(const char* ipaddr, int port, uint32_t max_connections)
        {
            max_connections_ = max_connections;
            listener_ = new net::Listener(*this);
            return listener_->Bind(ipaddr, port);
        }
		
		void GatewayBase::StopListener()
        {
            if (listener_) {
                listener_->Close();
                SAFE_RELEASE(listener_);
            }
        }
		
		/*
		 void GatewayBase::OnUserClientConnect(ClientPtr client)
        {
            clients_.insert(client);
            if (!handler_.HandleAcceptUserClient(client)) {
                client->Close();
            }
        }

        void GatewayBase::OnUserClientClose(ClientPtr client)
        {
            if (client->session()) {
                client->session()->OnUserClientClose();
            }
            handler_.OnUserClientClose(client);
            if (client->session()) {
                client->ResetSession();
            }
            size_t r = clients_.erase(client);
            debug_assert(r == 1);
            client->Release();
            CheckIfCleanupFinish();
        }
		  	
		void GatewayBase::BeginCleanup(boost::function<void()> cb)
        {
            is_cleanup_ = true;
            cleanup_finsh_cb_ = cb;
            StopListener();
            for (client_set_t::iterator it = clients_.begin(); it != clients_.end(); ++it) {
                (*it)->Close();
            }
            CheckIfCleanupFinish();
        }
		*/
        

       
        
        
    }
}
