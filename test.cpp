#include <iostream>
#include "Object.h"
#include "WorldPacket.h"
#include "utils/IntrusiveList.h"
#include "memory/MemoryPool.h"
#include "tmp/Gateway.h"
#include "event/Dispatcher.h"

using namespace std;

class Test : public base::Object
{
public:
	Test(uint32_t id) : m_id(id)
	{
		cout << m_id << "test...\n";
	}
	
	void Print()
	{
		cout << m_id << endl;
	}
private:
	uint32_t m_id;	
	INTRUSIVE_LIST(Test)
};

void test()
{
	base::utils::IntrusiveList<Test> test_list;
	Test test(1);
	Test test5(5);
	test_list.push_front(&test);
	test_list.push_front(&test5);
	base::WorldPacket pack(139, 128);
	pack.WriteUShort(1);
	pack.print();
	
	cout << "======================\n";
	
	Test* pTest = test_list.front();
	while(pTest)
	{
		pTest->Print();
		pTest = pTest->list_next();
	}
}

void testNet()
{
	base::memory::MemoryPool* mempool = new base::memory::MemoryPool ( 64, 1280 );
	Gateway::CreateInstance();
	
	//Gateway gateway(*mempool);
	sGateway.SetMemoryPool(mempool);
	sGateway.Setup("192.168.0.92", 11888);
	while(1)
	{
		g_dispatcher->Dispatch();
		usleep(50);
	}
	SAFE_DELETE(mempool);
}

int main()
{
	test();
	testNet();
	Gateway::DestroyInstance();
	cout << "g_object_count: " << base::g_object_count << endl;
	return 0;
}
