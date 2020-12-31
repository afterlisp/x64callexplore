
/***********************************************************************************************
 *         Code to check usage of countime in ns for several asm calls
 *         This is relative work to disambiguate which could be the fastest (relative)
 *         This is not a true profiling nor a true benchmark suite...
 *         Use as a quick illustration of how to instrument some calls to choose the best one
 *
 * Note 1: We use Boost as it is the more portable code (Windows/Linux) that we could write
 *        the assembly used is not portable as it is Windows specific calling convention
 *
 * Note 2: Define DEBUG with -D during call of g++ if you want some console trace
 ************************************************************************************************/

// include_files

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> //include all types plus i/o


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
unsigned long long test_sleep(int seconds){
	// init timer
    auto start = std::chrono::high_resolution_clock::now();
	//                                   FIRST SEQUENCE
	// start 
    start = std::chrono::high_resolution_clock::now();
	// do
    sleep(seconds);
	// stop
	unsigned long long countime=std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now()-start ).count();
	#ifdef DEBUG
	std::cout << "RUN TIME sleep " << countime << " ns" << std::endl;
	#endif
	return countime;
}

/**
 * test a sleep with a classical wall time from boost posix_time 
 *
 * @param seconds : number of seconds to sleep
 * @return the countime in ns (nanoseconds)
*/
unsigned long long test_sleep2(int seconds){
	   // Get the current time before executing Task
    boost::posix_time::ptime start = boost::posix_time::second_clock::local_time();
    // do
    sleep(seconds);
    // Get the current time after executing Task
    boost::posix_time::ptime end = boost::posix_time::second_clock::local_time();
    // Get the Time Difference by subtracting start time from end time
    boost::posix_time::time_duration dur = end - start;
    std::cout<<"Time Diff = "<<dur<<std::endl;
    // Getting the Time Difference in Total Nano Seconds only
	#ifdef DEBUG
    std::cout<<"Time Diff in Total Nano Seconds "<<dur.total_nanoseconds()<<std::endl;
	#endif
	return (long long) dur.total_nanoseconds();
}

/**
 * test a 8 bits add with boost chrono high resolution clock
 *
 * calls in a loop an asm function that does some addition in 8 bits as add(a,b)
 *
 * @param none
 * @return the countime in ns (nanoseconds)
*/
unsigned long long test_add_8bits(){
	// init timer
    auto start = std::chrono::high_resolution_clock::now();
	// now try our tiny asm function
	uint8_t a=3; // this is unsigned char from C++POV
	uint8_t b=5;
	// as uint8_t is just a typedef the compiler will show 'j' (unsigned char) and not an int
	// so cast the return to int...
	// start
	start = std::chrono::high_resolution_clock::now();
	// do call
	for (long long int m=0;m<N_CALL_ASM;m++) {
		int radder=adder(a,b);
	}
	// stop
	unsigned long long countime=std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now()-start ).count();
	#ifdef DEBUG
	std::cout << "RUN TIME 10**9 loop adder " << countime << " ns" << std::endl;
	#endif
	return countime;
}

/**
 * test a 32 bits add using asm LEA trick with boost chrono high resolution clock
 *
 * calls in a loop an asm function that does some addition using LEA to do a 3 members addition
 *
 * @param none
 * @return the countime in ns (nanoseconds)
*/
unsigned long long test_add_lea32bits(){
	// init timer
    auto start = std::chrono::high_resolution_clock::now();
	// test now with lea code
	uint32_t c=3;
	uint32_t d=5;
	// start
	start = std::chrono::high_resolution_clock::now();
	// do call
	for (long long int n=0;n<N_CALL_ASM;n++) {
		int radder2=adder2(c,d);
	}
	// stop
	unsigned long long countime=std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now()-start ).count();
	#ifdef DEBUG
	std::cout << "RUN TIME 10**9 loop adder2 " << countime << " ns" << std::endl;
	#endif
	return countime;
}

/**
 * test a 32 bits add with boost chrono high resolution clock
 *
 * calls in a loop an asm function that does some addition in 32 bits as add(a,b)
 *
 * @param none
 * @return the countime in ns (nanoseconds)
*/
unsigned long long test_add_32bits(){
	// init timer
    auto start = std::chrono::high_resolution_clock::now();
	// test now with classic code
	uint32_t c2=3;
	uint32_t d2=5;
	// start
	start = std::chrono::high_resolution_clock::now();
	// do call
	for (long long int o=0;o<N_CALL_ASM;o++) {
		int radder3=adder3(c2,d2);
	}
	// stop
	unsigned long long countime=std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now()-start ).count();
	#ifdef DEBUG
	std::cout << "RUN TIME 10**9 loop adder3 " << countime << " ns" << std::endl;
	#endif
	return countime;
}

/**
 * test a 64 bits add with boost chrono high resolution clock
 *
 * calls in a loop an asm function that does some addition in 64 bits as add(a,b)
 *
 * @param none
 * @return the countime in ns (nanoseconds)
*/
unsigned long long test_add_64bits(){
	// init timer
    auto start = std::chrono::high_resolution_clock::now();
	// test now with classic code
	uint64_t c3=3;
	uint64_t d3=5;
	// start
	start = std::chrono::high_resolution_clock::now();
	// do call
	for (long long int p=0;p<N_CALL_ASM;p++) {
		int radder4=adder4(c3,d3);
	}
	// stop
	unsigned long long countime=std::chrono::duration_cast<std::chrono::nanoseconds>( std::chrono::high_resolution_clock::now()-start ).count();
	#ifdef DEBUG
	std::cout << "RUN TIME 10**9 loop adder4 " << countime << " ns" << std::endl;
	#endif
	return countime;
}

/**
 * the main
 *
 * call several times each asm function and produces a CSV ; separated file with 
 * the following header : add8;add32;add64;addlea that represents each assembly functions
 * the result coulb be used with gnuplot.
 * also present the time elapsed with high resolution timer or classical timer (only if DEBUG is defined)
 *
 * @param none
 * @return the countime in ns (nanoseconds)
*/

int main(){
	// create a file to put all samples
	std::ofstream fsamples("samples.csv");
	// write header
	fsamples << "add8;add32;add64;addlea" << "\n";
	// repeat the test N times and store results
	for (unsigned int fi=0;fi<N_CALL_LOOPS;fi++) {
		unsigned long long a=test_add_8bits();
		unsigned long long b=test_add_32bits();
		unsigned long long c=test_add_64bits();
		unsigned long long d=test_add_lea32bits();
		fsamples << a << ";" << b << ";" << c << ";" << d << "\n";
	}
	fsamples.close();
	test_sleep(2);
	test_sleep2(2);
	return 0;
}


