set loadpath("C://Perso//gitprojects//x64callexplore//explo_cpp3_profile")
set datafile separator ";"
# use samples.csv as
# add8-proc;add8-wall,add8-cpu-us;lea-proc;lea-wall;lea-cpu-us;add32-proc;add32-wall;add32-cpu-us;add64-proc;add64-wall;add64-cpu-us
#      0        1         2        3         4         5         6            7           8           9          10         11
# proc are 0,3,6,9
# wall are 1,4,7,10
# cpu usage are 2,5,8,11

set multiplot layout 3,1 title "System Wall User in seconds" font ",14"
set xtics rotate 2
set bmargin 5
#
set title "System"
set key autotitle columnhead
plot "samples.csv" using 0:1 with linespoints, "samples.csv" using 0:4 with linespoints, "samples.csv" using 0:7 with linespoints, "samples.csv" using 0:10 with linespoints
#
set title "Wall"
set key autotitle columnhead
plot "samples.csv" using 0:2 with linespoints, "samples.csv" using 0:5 with linespoints, "samples.csv" using 0:8 with linespoints, "samples.csv" using 0:11 with linespoints
#
set title "User"
set key autotitle columnhead
plot "samples.csv" using 0:3 with linespoints, "samples.csv" using 0:6 with linespoints, "samples.csv" using 0:9 with linespoints, "samples.csv" using 0:12 with linespoints
#
unset multiplot
#
#
#
#