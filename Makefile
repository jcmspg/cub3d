# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/20 20:25:15 by joamiran          #+#    #+#              #
#    Updated: 2025/07/26 21:01:24 by joamiran         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cuboid
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = ./srcs
OBJ_DIR = ./obj
INC_DIR = ./includes

SRC = $(shell find $(SRC_DIR) -name "*.c")
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

LIBFT_DIR = ./extLibs/libft
LIBFT = $(LIBFT_DIR)/libft.a

PMFP_DIR = ./extLibs/poormanfixedpoint
PMFP = $(PMFP_DIR)/libpoormansfixed.a

MLX_DIR = ./extLibs/minilibx-linux
MLX = $(MLX_DIR)/libmlx.a
MLX_REPO = https://github.com/42Paris/minilibx-linux.git

INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR) -I$(PMFP_DIR) -I$(MLX_DIR)

LDFLAGS = -L$(LIBFT_DIR) -lft -L$(PMFP_DIR) -lpoormansfixed -L$(MLX_DIR) -lmlx -lm -lXext -lX11

# Default target
all: $(LIBFT) $(PMFP) $(MLX) $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(PMFP):
	$(MAKE) -C $(PMFP_DIR)

$(MLX): | check_mlx
	$(MAKE) -C $(MLX_DIR)

# Check if minilibx exists in the specified directory if not clone it automatically
# This ensures the project can be built even if minilibx is not present
check_mlx:
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "🔍 minilibx-linux not found, cloning..."; \
		git clone $(MLX_REPO) $(MLX_DIR); \
		echo "✅ minilibx-linux cloned successfully."; \
	else \
		echo "✅ minilibx-linux already exists."; \
	fi

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(INCLUDES) $(LDFLAGS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(PMFP_DIR) clean
	@if [ -d "$(MLX_DIR)" ] && [ -f "$(MLX_DIR)/libmlx.a" ]; then $(MAKE) -C $(MLX_DIR) clean; fi

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(PMFP_DIR) fclean
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# Remove minilibx completely (useful for fresh clone)
mlx_clean:
	@if [ -d "$(MLX_DIR)" ]; then \
		echo "🗑️  Removing $(MLX_DIR)..."; \
		rm -rf $(MLX_DIR); \
		echo "✅ minilibx-linux removed."; \
	else \
		echo "$(MLX_DIR) not found."; \
	fi

.PHONY: all clean fclean re check_mlx mlx_clean

