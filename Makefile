NAME = TestGL

CC = gcc
CC++ = c++
FLAGS = -Werror -Wextra -Wall
DEPFLAGS = -MMD -MP #creaazione dei file .d
REL_FLAGS = -g -O2 -DNDEBUG #la flag O2 è un buon compromesso tra velocità e affidabilità DNDEBUG ignora le chiamate ad assert
DEV_FLAGS = -g -D_GLIBCXX_DEBUG -O0 #Flags per lo sviluppo
CPPVER = -std=c++20 #scelta questa versione di cpp come compromesso tra compatibilità con opengl e feature di cpp
LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl #librerie per far funzionare opengl

INCLUDES = -I./includes
CPP_SRCS = $(addprefix src/, main.cpp)
C_SRCS   = $(addprefix src/, glad.c)
OBJ_DIR = build
CPP_OBJ = $(addprefix $(OBJ_DIR)/, $(CPP_SRCS:.cpp=.o))
C_OBJ   = $(addprefix $(OBJ_DIR)/, $(C_SRCS:.c=.o))
OBJ  = $(CPP_OBJ) $(C_OBJ)
DEPS = $(OBJ:.o=.d) #regola per la creazione di file dipendenze .d utilissimi per quando ci saranno tanti header inclusi ovunque così da non essere costretti a fare make re e run tutte le volte che si modifica un header

ACTIVE_FLAGS = $(DEV_FLAGS)

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC++) $(FLAGS) $(CPPVER) $(DEPFLAGS) $(ACTIVE_FLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(DEPFLAGS) $(ACTIVE_FLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJ)
	$(CC++) $(FLAGS) $(CPPVER) $(DEPFLAGS) -o $(NAME) $^ $(LIBS)

run: all
	clear; ./$(NAME)

rerun:
	fclean
	run

relrun: ACTIVE_FLAGS = $(REL_FLAGS)
relrun: $(OBJ)
	$(CC++) $(FLAGS) $(CPPVER) -o $(NAME) $^
	clear; ./$(NAME)

rerelrun: fclean relrun

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

gdb: $(NAME)
	clear ; gdb ./$(NAME)


rum: $(NAME)
	@clear
	@echo "Segmentation Fault (Core Dumped)"
	@sleep 2
	@clear
	@echo "nah scherzavo"
	@sleep 1
	@clear; ./$(NAME)

-include $(DEPS)

.PHONY: all run rerun relrun rerelrun clean fclean re gdb rum 
