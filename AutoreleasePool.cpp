#include "AutoreleasePool.h"
#include <cassert>
#include <cstddef>

namespace base
{
    using namespace std;

    /// AutoReleasePool
    PoolManager* PoolManager::m_instance = NULL;

    void PoolManager::CreateInstance()
    {
        assert(m_instance == NULL);
        m_instance = new PoolManager();
    }

    void PoolManager::DestroyInstance()
    {
        assert(m_instance != NULL);
        delete m_instance;
        m_instance = NULL;
    }

    PoolManager::PoolManager()
    {
    }

    PoolManager::~PoolManager()
    {
        while (!m_pools.empty()) {
            delete m_pools.top();
            m_pools.pop();
        }
    }
}
