set term postscript eps color enhanced "Times-Roman" 20
set output "Ex2error.eps"
set size 0.75
set logscale y
set key spacing 3
set xrange [0:100]
set yrange [1e-16:1]
set xlabel "{/Times-Italic=24 N}"
set ylabel "{/Times-Roman=24 maximum error}"
plot "Ex2Haber.dat" using 1:2 w lp title "Haber" lt 1 lw 1.2 pt 2 ps 1.2, "Ex2Stenger.dat" using 1:2 w lp title "Stenger" lt 2 lw 1.2 pt 4 ps 1.2, "Ex2MuhammadMori.dat" using 1:2 w lp title "Muhammad-Mori" lt 3 lw 1.2 pt 3 ps 1.2, "Ex2Okayama.dat" using 1:2 w lp title "This study" lt 4 lw 1.2 pt 6 ps 1.2
