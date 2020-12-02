#include <cstdint>
#include <iostream>

// the asm function is called adder()
// we use the fixed length types to be sure to have byte 8 bits !
extern "C" std::uint8_t adder(uint8_t,uint8_t);

int main() {
	uint8_t a=3; // this is unsigned char from C++POV
	uint8_t b=5;
	// as uint8_t is just a typedef the compiler will show 'j' (unsigned char) and not an int
	// so cast the return to int...
	std::cout<<"adder returns "<<(int)adder(a,b)<<"\n";
	return 0;
}