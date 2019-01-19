CURR_PATH=$(shell pwd)
SERVER=$(CURR_PATH)/server
CLIENT=$(CURR_PATH)/client
DATAPOOL=$(CURR_PATH)/data_pool
LIB=$(CURR_PATH)/lib
COMM=$(CURR_PATH)/comm
WINDOW=$(CURR_PATH)/window

LD_FLAGS=-lpthread -ljsoncpp
LIB_PATH=-L$(LIB)/lib

INCLUDE=-I$(DATAPOOL) -I$(LIB)/include -I$(COMM) -I$(WINDOW)

cc=g++

ser_bin=chatServer
cli_bin=chatClient

ser_obj=$(shell ls $(SERVER)/*.cpp $(DATAPOOL)/*.cpp $(COMM)/*.cpp | sed -e 's/\/.*\///' -e 's/\.cpp/\.o/')
cli_obj=$(shell ls $(CLIENT)/*.cpp $(COMM)/*.cpp $(WINDOW)/*.cpp| sed -e 's/\/.*\///' -e 's/\.cpp/\.o/')

#去掉sed -e 's/\/.*\///' -e 's/.cc/.o'(正则表达式) nodir内置函数也可以去掉路径

.PHONY:all
all:$(ser_bin) $(cli_bin)

$(ser_bin):$(ser_obj)
	$(cc) -o $@ $^ $(LIB_PATH) $(LD_FLAGS)
$(cli_bin):$(cli_obj)
	$(cc) -o $@ $^ $(LIB_PATH) $(LD_FLAGS) -lncurses

%.o:$(SERVER)/%.cpp 
	$(cc) -c $< $(INCLUDE)      
%.o:$(CLIENT)/%.cpp
	$(cc) -c $< $(INCLUDE)      
%.o:$(DATAPOOL)/%.cpp
	$(cc) -c $< $(INCLUDE)     
%.o:$(COMM)/%.cpp 
	$(cc) -c $< $(INCLUDE) 
%.o:$(WINDOW)/%.cpp 
	$(cc) -c $< $(INCLUDE) 

.PHONY:clean
clean:
	rm -rf *.o $(ser_bin) $(cli_bin)

.PHONY:debug
debug:
	@echo $(ser_obj)
	@echo $(cli_obj)
