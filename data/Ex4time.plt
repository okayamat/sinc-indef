set term postscript eps enhanced "Times-Roman" 20
set output "Ex4time.eps"
set size 0.825
set logscale y
set key spacing 1
set key bottom left
set xrange [0:0.04]
set yrange [1e-16:100]
set xlabel "{/Times-Roman=24 time [s]}"
set ylabel "{/Times-Roman=24 maximum error}"
plot "Ex4SE1.dat" using 3:2 w lp title "(SE1)" lt 1 lw 1.2 pt 2 ps 1.2, "Ex4SE2.dat" using 3:2 w lp title "(SE2)" lt 1 lw 1.2 pt 8 ps 1.2, "Ex4SE3.dat" using 3:2 w lp title "(SE3)" lt 1 lw 1.2 pt 4 ps 1.2, "Ex4DE1.dat" using 3:2 w lp title "(DE1)" lt 1 lw 1.2 pt 3 ps 1.2, "Ex4DE2.dat" using 3:2 w lp title "(DE2)" lt 1 lw 1.2 pt 10 ps 1.2, "Ex4DE3.dat" using 3:2 w lp title "(DE3)" lt 1 lw 1.2 pt 6 ps 1.2
