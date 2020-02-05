#make file
#Correr com o comando "make" no terminal, optamos por não compilar com o std=c99 por utilizar funções não aceites nesta linguagem, como o getopt, sendo que nos foi dito sendo possível num horário de dúvidas

CC = gcc #compiler
CFLAGS = -g -Wall -O3
TARGET = tester #target file name

all: 
	$(CC) $(CFLAGS) PF054.c Proj_inter.c ordena.c -o $(TARGET) oraculo.o 

clean: 
	rm $(TARGET)
