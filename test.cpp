#include <iostream>
#include "Object.h"
#include "WorldPacket.h"
using namespace std;

class Test : public base::Object
{
public:
	Test()
	{
		cout << "test...\n";
	}
};

void test()
{
	Test test;
	base::WorldPacket pack(139, 128);
	pack.WriteUShort(1);
	pack.print();
}

int main()
{
	test();
	cout << "g_object_count: " << base::g_object_count << endl;
	return 0;
}
