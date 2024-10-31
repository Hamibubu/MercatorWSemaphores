PROG1 = sems
PROG2 = nosems

CC = gcc
CFLAGS = -lm


all: $(PROG1) $(PROG2)

$(PROG1): $(PROG1).c
		@echo "\n[+] Compilando el programa con semáforos $(PROG1).c\n"
		$(CC) -o $(PROG1) $(PROG1).c $(CFLAGS)
$(PROG2): $(PROG2).c
		@echo "\n[+] Compilando el programa sin semáforos $(PROG2).c\n"
		$(CC) -o $(PROG2) $(PROG2).c $(CFLAGS)

clean:
		@echo "Limpiando archivos generados..."
		rm -f $(PROG1) $(PROG2)