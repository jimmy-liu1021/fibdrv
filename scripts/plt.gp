reset
set xlabel 'F(n)'
set ylabel 'time (ns)'
set title 'Fast doubling runtime'
set term png enhanced font 'Verdana,10'
set output 'plot.png'
set grid
plot [0:100][0:800] \
'plot' using 1:2 with linespoints linewidth 2 title "Kernel Space",\
'' using 1:3 with linespoints linewidth 2 title "User space",\
'' using 1:4 with linespoints linewidth 2 title "Sys call",