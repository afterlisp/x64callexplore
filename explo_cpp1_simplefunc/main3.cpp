
#include <cstdint>
#include <iostream>

// the asm function is called myfunc()
// we use the fixed length types to be sure to have 32 bits !
extern "C" std::int64_t myfunc(void);

int main() {
	std::cout<<"myfunc returns "<<myfunc()<<"\n";
	return 0;
}