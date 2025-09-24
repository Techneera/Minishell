CC = cc

CFLAGS = -Wall -Werror -Wextra -Iinclude 

NAME = minishell
#		DIRECTORIES		#

SRC_DIR = src
OBJ_DIR = obj
UNIT_DIR = unit

#		PATHS			#

SRCS = shell.c
OBJS = $(SRCS:.c=.o)

#		RULESET			#


