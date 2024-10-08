# Name
NAME	= philo

# Compiler and Flags
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
# Includes
INC	=	-I ./includes/

# Source files
SRC_DIR	=	sources/
SRC		=	check.c \
			error.c \
			handle_mutex_thread.c \
			init.c \
			main.c \
			observer.c \
			simulation_utils.c \
			simulation.c \
			utils.c
SRCS	=	$(addprefix $(SRC_DIR), $(SRC))

# Object files
OBJ_DIR	= obj/
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_DIR), $(OBJ))

# Build rules
all: $(OBJ_DIR) $(NAME)

# Create object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compile object files from source files
$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

# Linking rule
$(NAME): $(OBJS)
	@echo "Compiling Philo..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(INC)
	@echo "Philosophers ready."

clean:
	@echo "Removing .o object files..."
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Removing Philo..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
