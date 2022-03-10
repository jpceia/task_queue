# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/04 23:37:29 by jceia             #+#    #+#              #
#    Updated: 2022/03/10 23:14:28 by jpceia           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = task_queue

INC_DIR     = inc
SRC_DIR     = src
OBJ_DIR     = obj

SRCS        = $(shell find $(SRC_DIR) -name "*.cpp" -type f)
OBJS        = $(SRCS:$(SRC_DIR)/%=$(OBJ_DIR)/%.o)

CXX          = clang++
RM           = rm -f

OS          = $(shell uname)

FLAGS_WARN  = -Wall -Werror -Wextra -pedantic-errors
FLAGS_INC   = -I$(INC_DIR)
FLAGS_DEBUG = -g -DDEBUG -fsanitize=address -D_GLIBCXX_DEBUG
FLAGS_OPT   = -O3

CXXFLAGS    = $(FLAGS_WARN) $(FLAGS_INC) -std=c++98 -pthread

# Compilation
$(OBJ_DIR)/%.o:    $(SRC_DIR)/%
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

all: $(NAME)

# Cleaning
clean:
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

# Rebuild
re: fclean all

# Debug build
debug:      CXXFLAGS += $(FLAGS_DEBUG)
debug:      all


.PHONY:     all clean fclean re debug
