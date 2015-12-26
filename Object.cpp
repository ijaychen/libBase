#include <typeinfo>
#include "Object.h"
#include "AutoreleasePool.h"
namespace base
{
	uint32_t g_object_count = 0;

	Object::Object() : m_reference_count(1)
	{
		++g_object_count;
	}

	Object::~Object()
	{
		--g_object_count;
	}

	void Object::Retain()
	{
		++m_reference_count;
	}

	void Object::Release()
	{
		--m_reference_count;
		if(m_reference_count == 0)
		{
			delete this;
		}
	}

	void Object::AutoRelease()
	{
		PoolManager::Instance()->AddObject(this);
	}
}
