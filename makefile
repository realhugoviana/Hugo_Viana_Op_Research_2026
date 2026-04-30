# Nom de l'exécutable
TARGET = flowSolver

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude

# Fichiers sources
SRCS = src/main.c src/graph.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Règle principale
all: $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation des .c en .o
%.o: %.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	rm -f $(OBJS)

# Nettoyage complet
fclean: clean
	rm -f $(TARGET)

# Recompilation complète
re: fclean all

.PHONY: all clean fclean re