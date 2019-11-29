INC += -I./
LIB += -L /usr/lib/mysql -lmysqlclient -ljson -levent -lpthread
OBJ_DIR = ./obj
OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/fun.o
OBJ_EXT = .o
CXX_EXT = .cpp


main : $(OBJ)
	@echo
	@echo "Linking..."
	g++ -o main $(LIB) $(OBJ) 
	@echo

$(OBJ_DIR)/%$(OBJ_EXT) : %$(CXX_EXT)
	@echo
	@echo "Compiling $< ==> $@..."
	@-mkdir -p $(OBJ_DIR)
	g++ $(INC) -c $< -o $@

cc:
	@echo
	@echo "Removeing..."
	-rm -rf $(OBJ_DIR) ./bin main
	@echo
