# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -pthread

# Directories
SRC_DIR = src
INC_DIR = includes
OBJ_DIR = obj

# Source files
SRCS = $(SRC_DIR)/malloc.c

# Object files
OBJS = $(OBJ_DIR)/malloc.o

# Executable name
NAME = malloc_test

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

# Default target
all: $(NAME)

# Create object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compile source files
$(OBJ_DIR)/malloc.o: $(SRC_DIR)/malloc.c | $(OBJ_DIR)
	@echo "$(GREEN)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Link executable
$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)Build complete!$(RESET)"

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)

# Clean everything
fclean: clean
	@echo "$(RED)Removing executable...$(RESET)"
	@rm -f $(NAME)

# Rebuild
re: fclean all

# Run the program
run: $(NAME)
	@./$(NAME)

# Debug with valgrind
valgrind: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

# Phony targets
.PHONY: all clean fclean re run valgrind
