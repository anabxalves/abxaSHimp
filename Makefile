CC = gcc #setando compilador em gcc
SRCS = shell.c mySHfunc.c # setando sources (main e biblioteca)
OBJS = $(SRCS:.c=.o) # setando objetos (arquivos do source são transformando de .c em .o)
TARGET = shell # nome do executável resultante (conforme requisito)

$(TARGET): $(OBJS) # target : prerequisito
	$(CC) -o $@ $^
# @ => target atual
# ^ => todos os prerequisitos

%.o: %.c # target : prerequisito
	$(CC) -c -o $@ $<
# target atual
# prerequisito específico de cada .o (que seria seu correspondente .c)

.PHONY: clean
# phony utilizado pois não temos um arquivo específico e sim um target para alguma ação específica (nesse caso o clean definido abaixo)

clean:
	rm $(OBJS) $(TARGET)
