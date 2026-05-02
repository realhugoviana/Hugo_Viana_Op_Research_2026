TARGET = flowSolver

CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude

SRCS = src/main.c \
       src/graph.c \
       src/ford_fulkerson.c \
       src/min_cost.c \
	   src/negative_cycle.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

TEST_SRCS = \
	tests/test_graph.c \
	tests/test_ford_fulkerson.c \
	tests/test_min_cost.c \
	tests/test_main.c \
	src/graph.c \
	src/ford_fulkerson.c \
	src/min_cost.c

test:
	$(CC) $(CFLAGS) $(TEST_SRCS) -o test_runner
	./test_runner

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET) test_runner

re: fclean all

.PHONY: all clean fclean re test