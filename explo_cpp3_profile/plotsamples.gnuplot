# Input file contains comma-separated values fields
set loadpath("C://Perso//gitprojects//x64callexplore//explo_cpp3_profile")
set datafile separator ";"
set key autotitle columnhead
plot "samples.csv" using 0:1 with linespoints, "samples.csv" using 0:2 with linespoints, "samples.csv" using 0:3 with linespoints, "samples.csv" using 0:4 with linespoints
	
