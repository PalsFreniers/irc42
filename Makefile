NAME 	= ft_irc

# sources ---------------------------------------------------------------------#
	
FILES_SRC 	:=	main \
			class/Client \
			class/Server \
			function/loop

SRC_DIR		:=	src/
SRC		:= 	$(addprefix $(SRC_DIR), $(FILES_SRC:=.cpp))

OBJ_DIR		:=	obj/
OBJ		:=	$(addprefix $(OBJ_DIR), $(FILES_SRC:=.o))


# library ---------------------------------------------------------------------#

# includes --------------------------------------------------------------------#

FILES_HEADER 	:=	class/Client \
			class/Server \
			function/loop \
			includes
HEADER_DIR	:=	inc/
HEADER		:= 	$(addprefix $(HEADER_DIR), $(FILES_HEADER:=.hpp))
	
# flags -----------------------------------------------------------------------#

CXX		:=	c++
CXXFLAGS	:=	-Wall -Wextra -Werror -g -std=c++98
RM		:=	rm -rf

# colors ----------------------------------------------------------------------#

DEF_COLOR	:=	 \033[0;39m
GRAY		:=	 \033[0;90m
RED		:=	 \033[0;91m
GREEN		:=	 \033[0;92m
YELLOW		:=	 \033[0;93m
BLUE		:=	 \033[0;94m
MAGENTA		:=	 \033[0;95m
CYAN		:=	 \033[0;96m
WHITE		:=	 \033[0;97m

#------------------------------------------------------------------------------#
# rules                                                                        #
#------------------------------------------------------------------------------#

all: $(NAME)

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(BLUE)$(NAME) object files cleaned!$(DEF_COLOR)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(CYAN)$(NAME) executable files cleaned!$(DEF_COLOR)"

re: fclean all
	@echo "$(GREEN)cleaned and rebuilt for $(NAME)!$(DEF_COLOR)"

#------------------------------------------------------------------------------#
# compilation                                                                  #
#------------------------------------------------------------------------------#

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)$(NAME) compiling: $< $(DEF_COLOR)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@tput cuu1 && tput el

#------------------------------------------------------------------------------#
# linkage                                                                      #
#------------------------------------------------------------------------------#

$(NAME): $(OBJ) $(HEADER)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "$(RED)$(NAME) compiled!$(DEF_COLOR)"

#------------------------------------------------------------------------------#
# specifications                                                               #
#------------------------------------------------------------------------------#

.PHONY: all clean fclean re
