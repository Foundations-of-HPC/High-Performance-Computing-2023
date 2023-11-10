reset
set terminal pngcairo size 1600,1000 dashlength 2 truecolor font "Garamond, 28"
#set terminal qt enhanced size 1200,1000

set key inside top left font ",22"
set tics font ",22"
set lmargin screen 0.08
set rmargin screen 0.95
set bmargin screen 0.12

set xlabel "N" font ",22" offset 0,0.5

unset yrange
unset xrange

array OPT[2]
OPT[1] = "O0 "
OPT[2] = "O3 "

array W[2]
W[1] = 3
W[2] = 1.5

array DT[2]
DT[1] = "-- __"
DT[2] = 1

array TYPE[3]
TYPE[1] = "naive"
TYPE[2] = "optimized"
TYPE[3] = "tailed"


# ---------------------------------------------

set output "timings.png"
set ylabel "timing (sec)" font ",22" offset 2


plot for[L = 1:2] for [i = 2:4] "timings" u 1:(column(i+(L-1)*3)) w lp ps 2 lw W[L] dt DT[L] title OPT[L].TYPE[i-1],\
     "" u 1:(1.5e-8*$1**3) w l lc 0 lw 2 dt '..' notitle,\
     "" u 1:(3e-9*$1**3) w l lc 0 lw 2 dt '..' notitle


# ---------------------------------------------

set output "timings_ratio.png"
set ylabel "timings / timings_{naive}" font ",22" offset 2

ref = 2
clr = 2
plot for[L = 1:2] for [i = 3:4] "timings" u 1:(column(i+(L-1)*3)/column(ref)) w lp ps 2 lw W[L] dt DT[L] lc ((L-1)*3+(i-1)) title OPT[L].TYPE[i-1]

# ---------------------------------------------

set output "timings_per_element.png"
set ylabel "timing per element (nsec)" font ",22"  offset 2

plot for[L = 1:2] for [i = 2:4] "timings" u 1:(column(i+(L-1)*3)/($1**3)*1e9) w lp ps 2 lw W[L] dt DT[L] title OPT[L].TYPE[i-1]


# ---------------------------------------------

set output "CPE.png"
set ylabel "CPE" font ",22"  offset 2

plot for[L = 1:2] for [i = 2:4] "CPEs" u 1:(column(i+(L-1)*3)) w lp ps 2 lw W[L] dt DT[L] title OPT[L].TYPE[i-1]


# ---------------------------------------------

set output "L1M.png"
set ylabel "Level 1 misses per element" font ",22"  offset 2

plot for[L = 1:2] for [i = 2:4] "L1Ms" u 1:(column(i+(L-1)*3)) w lp ps 2 lw W[L] dt DT[L] title OPT[L].TYPE[i-1]


# ---------------------------------------------

set output "IPC.png"
set key inside bottom left
set ylabel "IPC" font ",22"  offset 2
set yrange [:4]

plot for[L = 1:2] for [i = 2:4] "IPCs" u 1:(column(i+(L-1)*3)) w lp ps 2 lw W[L] dt DT[L] title OPT[L].TYPE[i-1]




set output
reset
