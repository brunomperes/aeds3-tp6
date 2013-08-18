set terminal postscript eps 28 enhanced color
set style fill solid 1.00 border
set encoding utf8
set output "memoria.eps"
set style fill solid 1.00 border
set title "Quantidade de memória utilizada"
set xrange[0:]
set yrange[0:]
set xlabel "Tamanho t da entrada"
set ylabel "Memória utilizada (bytes)"
plot 'Analise.txt' using 1:2 notitle with linespoints pointsize 0.4 pt 7
