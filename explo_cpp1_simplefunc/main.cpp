#include <iostream>

// the asm function is called myfunc()
extern "C" int myfunc(void);

int main() {
	std::cout<<"myfunc returns "<<myfunc()<<"\n";
	return 0;
}