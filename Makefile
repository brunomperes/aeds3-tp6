######################################################################
# TRABALHO PRÁTICO 6 - Paralelismo
# Algoritmos e Estruturas de Dados III
#
# Bruno Maciel Peres
# brunomperes@dcc.ufmg.br
######################################################################


# Especifica o compilador
CC = gcc

# Especifica as opções do compilador, habilita aviso sobre erros
CFLAGS = -Wall -pg -g3

# Comando terminal para limpar sem confirmação
RM = rm -f

# Lista dos ficheiros (arquivos) objetos necessários para o programa final
OBJS = main.o io.o heuristica.o timerlinux.o lista.o

# Nome do executável
MAIN = tp6

# Argumentos para execução
ARGS = -i input.txt -o output.txt 4

# Opções para execução do Valgrind
VOPS = --tool=helgrind
#VOPS = --leak-check=full --show-reachable=yes

# Compilação do programa e passos das ligações de dependências
$(MAIN): $(OBJS)
	@echo ""
	@echo " --- COMPILANDO PROGRAMA ---"
	@$(CC) $(CFLAGS) $(OBJS) -lm -o $(MAIN) -lpthread 
	@echo ""

%.o: %.c %.h
	@echo ""
	@echo " --- COMPILANDO OBJETO \"$@\""
	@$(CC) $(CFLAGS) $< -c 

clean:
	$(RM) $(MAIN) *.o
	$(RM) gmon.out

run: $(MAIN)
	./$(MAIN) $(ARGS)
	
valgrind: $(MAIN)
	valgrind $(VOPS) ./$(MAIN) $(ARGS)

gprof: $(MAIN)
	./$(MAIN) $(ARGS)
	gprof -b -p $(MAIN)

