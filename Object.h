#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "Common.h"

namespace base
{
	class Object
	{
	public:
		Object();
		//对象引用次数
		uint32_t ReferenceCount() const 
		{
			return m_reference_count;
		}

		//是否单一引用
		bool IsSingleReference() const 
		{
			return m_reference_count == 1;
		}

		//释放一次引用
		void Release();
		//持有一次引用
		void Retain();
		//自动释放(在下次循环中调用Release)
		void AutoRelease();
	protected:
		virtual ~Object();

	private:
		uint32_t m_reference_count;
	};

extern uint32_t g_object_count;
}

#define SAFE_RELEASE(obj) do { if(obj != nullptr) { obj->Release(); obj == nullptr; }} while(0);

#endif

