set terminal postscript eps 28 enhanced color
set style fill solid 1.00 border
set encoding utf8
set output "paralelo.eps"
set style fill solid 1.00 border
set title "Tempo de execução algoritmo paralelo - 4 threads"
set xrange[0:]
set yrange[0:3]
set xlabel "n"
set ylabel "Tempo de execução (segs.)"
plot 'Analise-ncresc.txt' using 2:3 notitle with linespoints pointsize 0.5 pt 7
