set samples 100
set isosamples 100
set xyplane 0

set xlabel "STRIDES" font ", 16"
set ylabel "SIZE (KB, log_2)" font ", 16"
set zlabel "MBs/sec" offset -3, 0 font ",16" rotate parallel

set tics font ", 12"

set pm3d
splot [:17][4:17] "mountain.dat" u 1:2:3 matrix nonuniform with lines lc 0 notitle

