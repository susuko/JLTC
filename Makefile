NAME = jltc.out

SRCS = main.c shutdown.c buzzer.c straight.c line_sensor.c
OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -I. -I./jeedPi -L./jeedPi
LIBS = -lwiringPi -ljeedPi -lpthread -lm

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
