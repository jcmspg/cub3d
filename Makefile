# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/20 20:25:15 by joamiran          #+#    #+#              #
#    Updated: 2026/04/09 02:52:27 by hladeiro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror -O3
#CFLAGS = -O3 
UNAME_S := $(shell uname)

# MLX behavior flags
# MLX_COMPAT_FIX=1 applies local header prototype fixes for stricter compilers.
# Set to 0 on 42 evaluators if their minilibx builds as-is.
MLX_COMPAT_FIX ?= 1

SRC_DIR = ./srcs
OBJ_DIR = ./obj
INC_DIR = ./includes

# Automatically find all .c files in subdirectories of srcs
SRC = $(shell find $(SRC_DIR) -name "*.c")
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

LIBFT_DIR = ./extLibs/libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_REPO = https://github.com/jcmspg/libft

PMFP_DIR = ./extLibs/poormanfixedpoint
PMFP = $(PMFP_DIR)/libpoormansfixed.a
PMFP_REPO = https://github.com/jcmspg/poor-mans-FixedPoint-lib

MLX_DIR = ./extLibs/minilibx-linux
MLX = $(MLX_DIR)/libmlx.a
MLX_TAR = ./minilibx-linux.tgz
MLX_STAMP = $(MLX_DIR)/.from_tgz
MLX_REPO = https://github.com/42Paris/minilibx-linux.git
MLX_FLAGS= -L $(MLX_DIR) -lmlx -framework OpenGL -framework AppKit -lz


ifeq ($(UNAME_S), Linux)
	MLX_DIR = ./extLibs/minilibx-linux
	MLX = $(MLX_DIR)/libmlx.a
	MLX_TAR = ./minilibx-linux.tgz
	MLX_STAMP = $(MLX_DIR)/.from_tgz
	MLX_FLAGS= -L $(MLX_DIR) -lmlx -lm -lXext -lX11
endif

INCLUDES = -I$(INC_DIR) -I$(LIBFT_DIR) -I$(PMFP_DIR) -I$(MLX_DIR)

LDFLAGS = -L$(LIBFT_DIR) -lft -L$(PMFP_DIR) -lpoormansfixed -L$(MLX_DIR) $(MLX_FLAGS)

# Default target
all: deps $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) deps
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT): | check_libft
	$(MAKE) -C $(LIBFT_DIR)

$(PMFP): | check_pmfp
	$(MAKE) -C $(PMFP_DIR)

deps: $(LIBFT) $(PMFP) $(MLX)

.NOTPARALLEL: deps check_libft check_pmfp

$(MLX_STAMP):
	rm -rf $(MLX_DIR)
	mkdir -p ./extLibs
	@if [ -f "$(MLX_TAR)" ]; then \
		echo "📦 Extracting MLX from $(MLX_TAR)..."; \
		tar -xzf $(MLX_TAR) -C ./extLibs; \
	else \
		echo "📥 $(MLX_TAR) not found, cloning $(MLX_REPO)..."; \
		git clone --depth 1 $(MLX_REPO) $(MLX_DIR); \
	fi
	@if [ ! -d "$(MLX_DIR)" ] && [ -d "./extLibs/minilibx-linux" ]; then \
		mv ./extLibs/minilibx-linux $(MLX_DIR); \
	fi
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "❌ MLX extraction failed: expected $(MLX_DIR)"; \
		exit 1; \
	fi
	@touch $(MLX_STAMP)

$(MLX): $(MLX_STAMP)
	@if [ "$(MLX_COMPAT_FIX)" = "1" ]; then \
		echo "🛠️  Applying MLX header compatibility fix..."; \
		sed -i 's/int[[:space:]]*mlx_get_color_value();/int\t\t\t\tmlx_get_color_value(t_xvar *xvar,int color);/' $(MLX_DIR)/mlx_int.h; \
		sed -i 's/int[[:space:]]*mlx_int_get_good_color();/int\t\t\t\tmlx_int_get_good_color(t_xvar *xvar,int color);/' $(MLX_DIR)/mlx_int.h; \
		sed -i 's/int[[:space:]]*mlx_int_anti_resize_win();/int\t\t\t\tmlx_int_anti_resize_win(t_xvar *xvar,Window win,int w,int h);/' $(MLX_DIR)/mlx_int.h; \
		sed -i 's/int[[:space:]]*mlx_int_wait_first_expose();/int\t\t\t\tmlx_int_wait_first_expose(t_xvar *xvar,Window win);/' $(MLX_DIR)/mlx_int.h; \
		sed -i 's/int[[:space:]]*mlx_int_rgb_conversion();/int\t\t\t\tmlx_int_rgb_conversion(t_xvar *xvar);/' $(MLX_DIR)/mlx_int.h; \
		sed -i 's/int[[:space:]]*mlx_int_deal_shm();/int\t\t\t\tmlx_int_deal_shm(t_xvar *xvar);/' $(MLX_DIR)/mlx_int.h; \
	else \
		echo "✅ MLX compatibility fixes disabled (MLX_COMPAT_FIX=0)"; \
	fi
	@sed 's#%%%%#/usr/include#g' $(MLX_DIR)/Makefile.mk > $(MLX_DIR)/makefile.gen
	$(MAKE) -C $(MLX_DIR) -f makefile.gen CFLAGS='-O3 -I/usr/include -std=gnu89'

# Check if libft exists in the specified directory if not clone it automatically
check_libft:
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "🔍 libft not found, cloning..."; \
		git clone $(LIBFT_REPO) $(LIBFT_DIR); \
		echo "✅ libft cloned successfully."; \
	else \
		echo "✅ libft already exists."; \
	fi

# Check if poormanfixedpoint exists in the specified directory if not clone it automatically
check_pmfp:
	@if [ ! -d "$(PMFP_DIR)" ]; then \
		echo "🔍 poormanfixedpoint not found, cloning..."; \
		git clone $(PMFP_REPO) $(PMFP_DIR); \
		echo "✅ poormanfixedpoint cloned successfully."; \
	else \
		echo "✅ poormanfixedpoint already exists."; \
	fi

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(INCLUDES) $(LDFLAGS) -o $(NAME)


clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(PMFP_DIR) clean
	@if [ -d "$(MLX_DIR)" ]; then \
	  if [ -f "$(MLX_DIR)/makefile.gen" ]; then \
	    $(MAKE) -C $(MLX_DIR) -f makefile.gen clean; \
	  elif [ -f "$(MLX_DIR)/Makefile" ]; then \
	    $(MAKE) -C $(MLX_DIR) clean; \
	  fi; \
	fi


fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(PMFP_DIR) fclean
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# Remove libraries completely (useful for fresh clone)
libft_clean:
	@if [ -d "$(LIBFT_DIR)" ]; then \
		echo "🗑️  Removing $(LIBFT_DIR)..."; \
		rm -rf $(LIBFT_DIR); \
		echo "✅ libft removed."; \
	else \
		echo "$(LIBFT_DIR) not found."; \
	fi

pmfp_clean:
	@if [ -d "$(PMFP_DIR)" ]; then \
		echo "🗑️  Removing $(PMFP_DIR)..."; \
		rm -rf $(PMFP_DIR); \
		echo "✅ poormanfixedpoint removed."; \
	else \
		echo "$(PMFP_DIR) not found."; \
	fi

mlx_clean:
	@if [ -d "$(MLX_DIR)" ]; then \
		echo "🗑️  Removing $(MLX_DIR)..."; \
		rm -rf $(MLX_DIR); \
		echo "✅ minilibx-linux removed."; \
	else \
		echo "$(MLX_DIR) not found."; \
	fi

.PHONY: all clean fclean re check_libft check_pmfp libft_clean pmfp_clean mlx_clean
