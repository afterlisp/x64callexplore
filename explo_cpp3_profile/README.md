# using chrono

In this exploration we want to measure cpu consumed by our functions to choose between different implementations.

Boost
=====

Boost is a very large library that helps and accelerate coding in C++. This is a good choice to avoid the pain to implement some complex structure and it is generally
templated. It also works under Windows and Linux, therefore if portability is needed, Boost would be the good choice.

To use Boost we need to build and install it on our PC.
Here are our notes about such installation :
Note about building boost to mingw64
First from : file:///C:/Tools/boost_1_74_0/boost_1_74_0/index.html
and after : file:///C:/Tools/boost_1_74_0/boost_1_74_0/more/getting_started/windows.html#the-boost-distribution

```
unzip Boost into a directory in my case c:\Tools\          (note that Boost will create a bin.v2 directory automatically and place a lot of file/compile/ and so on)
c:\Tools\boost_1_74_0\boost_1_74_0>mkdir build
c:\Tools\boost_1_74_0\boost_1_74_0>bootstrap.bat gcc
Building Boost.Build engine
c:\Tools\boost_1_74_0\boost_1_74_0>b2 --prefix="c:\Tools\boost_1_74_0\build"
Performing configuration checks
.....performs a lot of check with yes / no 
...display patience.... :-)
... compile a lot ....
Note that compiler supports sse3 and avx2
After that show a msg like :

The Boost C++ Libraries were successfully built!
```


The following directory should be added to compiler include paths:

    C:\Tools\boost_1_74_0\boost_1_74_0

The following directory should be added to linker library paths:

    C:\Tools\boost_1_74_0\boost_1_74_0\stage\lib
	
Each lib is decline in 32 and 64 bits, for instance in stage\lib you will find :
debug libs with -d before x32 or x64
libboost_chrono-mgw9-mt-d-x32-1_74.a
libboost_chrono-mgw9-mt-d-x64-1_74.a
release without -d 
libboost_chrono-mgw9-mt-x32-1_74.a
libboost_chrono-mgw9-mt-x64-1_74.a


libboost_regex-mgw9-mt-d-x32-1_74.a
libboost_regex-mgw9-mt-d-x64-1_74.a
In our case we will use x64...

	
So when you use g++ you will always need to put : -I"C:\Tools\boost_1_74_0\boost_1_74_0" -L"C:\Tools\boost_1_74_0\boost_1_74_0\stage\lib"

And when you use a library you have only to write the name without prefix lib and postfix .a as
-llibboost_chrono-mgw9-mt-d-x64-1_74    if you have included chrono..


First use test_boost to check boost is ok





Measure duration
=================
There are two ways to measure durations :

Wall-clock time: By using wall-clock time the execution of the program is measured in the same way as 
if we were using an external (wall) clock. This approach does not consider the interaction between programs.

CPU time: In this case we only count the time that a program is actually running on the CPU. 
If a program (P1) is co-scheduled with another one (P2), and we want to get the CPU time for P1, 
this approach does not include the time while P2 is running and P1 is waiting for the CPU (as opposed to the wall-clock time approach).

We consider to compare fast that using a wall clock time could be sufficient at this step of our work.


Wall clock time
===============

If we use C++11 we could use :
```

auto start = std::chrono::system_clock::now();

/* do some work */

auto end = std::chrono::system_clock::now();
auto elapsed = end - start;
std::cout << elapsed.count() << '\n';

Time resolution could be passed with :
// this constructs a duration object using milliseconds
auto elapsed =
    std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

// this constructs a duration object using seconds
auto elapsed =
    std::chrono::duration_cast<std::chrono::seconds>(end - start);
```

	
Generally we will not use  std::chrono:system_clock()

Use std::chrono::steady_clock and not std::chrono::system_clock for measuring run time in C++11.
The reason is (quoting system_clock's documentation):

on most systems, the system time can be adjusted at any moment

while steady_clock is monotonic and is better suited for measuring intervals:

Class std::chrono::steady_clock represents a monotonic clock. The time points of this clock cannot decrease as physical time moves forward. 
This clock is not related to wall clock time, and is best suitable for measuring intervals.

Here's an example:
```
auto start = std::chrono::steady_clock::now();
// do something
auto finish = std::chrono::steady_clock::now();
double elapsed_seconds = std::chrono::duration_cast<
  std::chrono::duration<double> >(finish - start).count();
```


See now test_chrono.cpp
Note that results are put into sample.csv (see gnutplot) and plotsamples.gnuplot...



Moreover, since most modern systems are time-shared, different programs may compete for several computing resources (e.g., CPU). 
In such a case, another distinction can be made:

Wall-clock time: By using wall-clock time the execution of the program is measured in the same way as if we were using an external (wall) clock. This approach does not consider the interaction between programs.
CPU time: In this case we only count the time that a program is actually running on the CPU. If a program (P1) is co-scheduled with another one (P2), and we want to get the CPU time for P1, this approach does not include the time while P2 is running and P1 is waiting for the CPU (as opposed to the wall-clock time approach).
For measuring CPU time, Boost includes a set of extra clocks (see go further with CPU time)


Gnuplot
=======
As we want to compare different functions we will use a tool to draw some curves. Gnuplot is a classical tool that helps to produce nice graphs with little effort.
Gnuplot uses some scripts written in its own language.

Here are our notes about installation :
use gnuplot 5.2.8 from sourceforge (install the mingw version works fine on windows 10 64)
launch gnuplot in console mode
go where is the script here into C:\Perso\gitprojects\x64callexplore\explo_cpp3_profile
gnuplot>cd "C://Perso//gitprojects//x64callexplore//explo_cpp3_profile"
launch the script that draws curves with point the name of the script is plotsamples.gnuplot (this is a text file)
gnuplot>load("plotsamples.gnuplot")


Go further with CPU time
========================
EDIT: The example above can be used to measure wall-clock time. 
That is not, however, the only way to measure the execution time of a program. First, we can distinct between user and system time:

User time: The time spent by the program running in user space.
System time: The time spent by the program running in system (or kernel) space.
 A program enters kernel space for instance when executing a system call.
 
Depending on the objectives it may be necessary or not to consider system time as part of the execution time of a program. 
For instance, if the aim is to just measure a compiler optimization on the user code then it is probably better to 
leave out system time. On the other hand, if the user wants to determine whether system calls are a significant overhead,
 then it is necessary to measure system time as well.
 
C++11 does not have Cpu measure functions.
process_real_cpu_clock, captures wall clock CPU time spent by the current process.
process_user_cpu_clock, captures user-CPU time spent by the current process.
process_system_cpu_clock, captures system-CPU time spent by the current process. A tuple-like class process_cpu_clock, that captures real, user-CPU, and system-CPU process times together.
A thread_clock thread steady clock giving the time spent by the current thread (when supported by a platform).




Our Quick and Dirty Implementation
==================================


If you cannot use C++11, then have a look at chrono from Boost.

The best thing about using such a standard libraries is that their portability is really high (e.g., they both work in Linux and Windows). 
So you do not need to worry too much if you decide to port your application afterwards.


From Boost documentation "high_resolution_clock is the clock with the shortest tick period. It may be a synonym for system_clock or steady_clock"

There is a nice function that manage the high resolution clock :
```
boost::timer::auto_cpu_timer auto_timer( os,9, "%s;"
                                                    "%w;"
                                                    "%u" ) ;
```													
see https://www.boost.org/doc/libs/1_75_0/libs/timer/doc/cpu_timers.html

As explained this setup an automatic timer (no need to start) and when destroyed, it kept the values and push them into output stream passed (by default stdout).

The format of output is done into doc, as we want csv form (to be gnuplotted) we add some ;.
```
%s	times.system
%w	times.wall
%u	times.user
```

So if we want to measure our standard asm func we just do :
```
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
```

at the end of the function the results are published into outputstream os. 
Note the pseudo block to force creation and desctruction locally of the auto_cpu_timer.
note that we need to do a lot of operations (instead of the results are too smalls).

quite easy - take a look to test_cpuwall.cpp and that's ok.
you could gnuplot with plottimes.gnuplot with the same procedure already explained







