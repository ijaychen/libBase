#ifndef BASE_AUTORELEASEPOOL_H
#define BASE_AUTORELEASEPOOL_H

#include <vector>
#include <stack>
#include "Object.h"
#include "Singleton.h"
namespace base
{
    class AutoReleasePool
    {
    public:
        AutoReleasePool() {
            m_objects.reserve(20);
        }
        ~AutoReleasePool() {
            Clear();
        }

        void Clear() {
            if (!m_objects.empty()) {
                for (std::vector<Object*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it) {
                    (*it)->Release();
                }
                m_objects.clear();
            }
        }

        void AddObject(Object* obj) {
            m_objects.push_back(obj);
        }

    private:
        std::vector<Object*> m_objects;
    };

    class PoolManager : public Singleton<PoolManager>
    {
		friend class Singleton<PoolManager>;
    public:
        void AddObject(Object* obj) {
            if (m_pools.empty()) {
                Push();
            }
            m_pools.top()->AddObject(obj);
        }

        void Push() {
            AutoReleasePool* pool = new AutoReleasePool;
            m_pools.push(pool);
        }
        void Pop() {
            if (m_pools.size() > 1) {
                delete m_pools.top();
                m_pools.pop();
            } else {
                if (m_pools.size() == 1) {
                    m_pools.top()->Clear();
                }
            }
        }

    private:
        PoolManager(){}
        ~PoolManager()
		 {
			while (!m_pools.empty()) 
			{
				delete m_pools.top();
				m_pools.pop();
			}
		}
        std::stack<AutoReleasePool*> m_pools;
    };
}

#endif // AUTORELEASEPOOL_H
