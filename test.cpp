#include <iostream>
#include "Object.h"
#include "WorldPacket.h"
#include "utils/IntrusiveList.h"

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

int main()
{
	test();
	cout << "g_object_count: " << base::g_object_count << endl;
	return 0;
}
