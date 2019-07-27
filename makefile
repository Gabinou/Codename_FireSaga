

CXX	= c++
LINK = link
SRC = src
BLD = build
INCLUDE = -I include/ 


# main.o:$(SRC)/main.cpp
	# $(CXX) $(SRC)/main.cpp  -o $(BLD)/main.o $(INCLUDE)

unit.o:$(SRC)/unit.cpp
	$(CXX) $(SRC)/unit.cpp  -o $(BLD)/unit.o $(INCLUDE)

# weapon.o:$(SRC)/weapon.cpp
	# $(CXX) $(SRC)/weapon.cpp  -o $(BLD)/weapon.o $(INCLUDE)
    
# main.exe:$(BLD)/main.o
	# $(CXX) $(BLD)/main.o $(BLD)/unit.o $(BLD)/weapon.o -o $(BLD)/main.exe $(INCLUDE)
