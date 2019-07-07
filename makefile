
CXX	= g++
LINK = link
SRC = src
BLD = build
INCLUDE = -I include/ 
main.exe:$(SRC)/main.c
    $(CXX) $(SRC)/main.c  -o $(BLD)/main.exe $(INCLUDE)