/*
 *
 */
 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <thread>
#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>



// external code (from nasm win64)
// several asm functions that add 100 + two parameters
// we use different implementations
extern "C" std::uint8_t adder(uint8_t,uint8_t);       // add 8 bits
extern "C" std::uint32_t adder2(uint32_t,uint32_t);   // add with lea on EAX (32 bits)
extern "C" std::uint32_t adder3(uint32_t,uint32_t);   // add 32 bits
extern "C" std::uint64_t adder4(uint64_t,uint64_t);   // add 64 bits

const unsigned int N_CALL_LOOPS=100;              // How many loops we want in main (number of samples)
const long long int N_CALL_ASM=1000000000;		// How many calls of asm function


/**
 * test a sleep with boost chrono high resolution clock
 *
 * @param seconds : number of seconds to sleep
 * @return the countime in ns (nanoseconds)
*/

/**
 * test a 8 bits add with boost chrono high resolution clock
 *
 * calls in a loop an asm function that does some addition in 8 bits as add(a,b)
 *
 * @param none
 * @return the countime in ns (nanoseconds)
*/
unsigned long long test_add_8bits(std::ostream& os){
	// now try our tiny asm function
	uint8_t a=3; // this is unsigned char from C++POV
	uint8_t b=5;
	// do call
	{
		/* boost::timer::auto_cpu_timer auto_timer( os,9, "%t;"
                                                    "%w;"
                                                    "%p" ) ;*/
		boost::timer::auto_cpu_timer auto_timer( os,9, "%s;"
                                                    "%w;"
                                                    "%u" ) ;
		for (long long int m=0;m<N_CALL_ASM;m++) {
			int radder=adder(a,b);
		}
	}
	return 0;
}

/**
 * test a 32 bits add using asm LEA trick with boost chrono high resolution clock
 *
 * calls in a loop an asm function that does some addition using LEA to do a 3 members addition
 *
 * @param none
 * @return the countime in ns (nanoseconds)
*/
unsigned long long test_add_lea32bits(std::ostream& os){
	// test lea trick
	uint32_t c2=3;
	uint32_t d2=5;
	// do call
	{
		boost::timer::auto_cpu_timer auto_timer( os,9, "%s;"
                                                    "%w;"
                                                    "%u" ) ;
		for (long long int n=0;n<N_CALL_ASM;n++) {
			int radder2=adder2(c2,d2);
		}
	}
	return 0;
}

/**
 * test a 32 bits add with boost chrono high resolution clock
 *
 * calls in a loop an asm function that does some addition in 32 bits as add(a,b)
 *
 * @param none
 * @return the countime in ns (nanoseconds)
*/
unsigned long long test_add_32bits(std::ostream& os){
	
	// test now with classic code
	uint32_t c2=3;
	uint32_t d2=5;
	// do call
	{
		boost::timer::auto_cpu_timer auto_timer( os,9, "%s;"
                                                    "%w;"
                                                    "%u" ) ;
		for (long long int o=0;o<N_CALL_ASM;o++) {
			int radder3=adder3(c2,d2);
		}
	}
	
	return 0;
}

/**
 * test a 64 bits add with boost chrono high resolution clock
 *
 * calls in a loop an asm function that does some addition in 64 bits as add(a,b)
 *
 * @param none
 * @return the countime in ns (nanoseconds)
*/
unsigned long long test_add_64bits(std::ostream& os){
	// test now with classic code
	uint64_t c3=3;
	uint64_t d3=5;
	// do call
	{
		boost::timer::auto_cpu_timer auto_timer( os,9, "%s;"
                                                    "%w;"
                                                    "%u" ) ;
		for (long long int p=0;p<N_CALL_ASM;p++) {
			int radder4=adder4(c3,d3);
		}
	}
	
	return 0;
}




int main()
{
	// create a file to put all samples
	std::ofstream fsamples("samples.csv");
	// write header
	fsamples << "add8-sys;add8-wall,add8-user;lea-sys;lea-wall;lea-user;add32-sys;";
	fsamples << "add32-wall;add32-user;add64-sys;add64-wall;add64-user" << "\n";
	// repeat the test N times and store results
	for (unsigned int fi=0;fi<N_CALL_LOOPS;fi++) {
		//constexpr int N = 1024*1024*128 ;
		test_add_8bits(fsamples);
		fsamples << ";";
		test_add_lea32bits(fsamples);
		fsamples << ";";
		test_add_32bits(fsamples);
		fsamples << ";";
		test_add_64bits(fsamples);
		fsamples << "\n";
	}
	fsamples.close();
	return 0;

}
