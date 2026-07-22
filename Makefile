# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dievarga <dievarga@student.42barcelona.co  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/21 14:29:56 by dievarga          #+#    #+#              #
#    Updated: 2026/07/22 14:37:55 by dievarga         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	codexion
CC		=	cc
CFLAGS	=	-Wall -Werror -Wextra
SRCS	=	main.c parsing.c simulation.c queue.c actions.c arbitration.c utils.c
OBJS	=	$(SRCS:.c=.o)
HEADER	=	codexion.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
