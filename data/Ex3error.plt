set term postscript eps enhanced "Times-Roman" 20
set output "Ex3error.eps"
set size 0.825
set logscale y
set key spacing 1
set xrange [0:100]
set yrange [1e-16:100]
set xlabel "{/Times-Italic=24 n}"
set ylabel "{/Times-Roman=24 maximum error}"
plot "Ex3SE1.dat" using 1:2 w lp title "(SE1)" lt 1 lw 1.2 pt 2 ps 1.2, "Ex3SE2.dat" using 1:2 w lp title "(SE2)" lt 1 lw 1.2 pt 8 ps 1.2, "Ex3SE3.dat" using 1:2 w lp title "(SE3)" lt 1 lw 1.2 pt 4 ps 1.2, "Ex3DE1.dat" using 1:2 w lp title "(DE1)" lt 1 lw 1.2 pt 3 ps 1.2, "Ex3DE2.dat" using 1:2 w lp title "(DE2)" lt 1 lw 1.2 pt 10 ps 1.2, "Ex3DE3.dat" using 1:2 w lp title "(DE3)" lt 1 lw 1.2 pt 6 ps 1.2
