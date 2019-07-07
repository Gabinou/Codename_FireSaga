
CXX	= g++
LINK = link
SRC = src
BLD = build
INCLUDE = -I include/ 
build/main.exe:src/main.c
    $(CXX) $(SRC)/main.c  -o $(BLD)/main.exe $(INCLUDE)