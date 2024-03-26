NAME = jltc.out

SRCS = \
	button.c \
	buzzer.c \
	control.c\
	distance.c \
	led.c \
	line.c \
	logger.c \
	main.c \
	motor.c \
	position.c \
	servo.c \
	shutdown.c \
	straight.c \
	vector.c

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
