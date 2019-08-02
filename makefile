
CXX	= c++
LINK = link
SRC = src
BLD = build
INCLUDE = -I include/ 
    
all: clean create_dirs $(BLD)/generic.o $(BLD)/inventory_item.o $(BLD)/weapon.o $(BLD)/unit.o $(BLD)/main.o $(BLD)/main.exe
noclean: $(BLD)/weapon.o $(BLD)/main.o $(BLD)/unit.o $(BLD)/main.exe

$(BLD)/inventory_item.o: $(SRC)/inventory_item.cpp
	$(CXX) -c $** -o $@ $(INCLUDE)

$(BLD)/generic.o: $(SRC)/generic.cpp
	$(CXX) -c $** -o $@ $(INCLUDE)

$(BLD)/main.o: $(SRC)/main.cpp
	$(CXX) -c $** -o $@ $(INCLUDE)

$(BLD)/unit.o: $(SRC)/unit.cpp
	$(CXX) -c $**  -o $@ $(INCLUDE)

$(BLD)/weapon.o: $(SRC)/weapon.cpp
	$(CXX) -c $**  -o $@ $(INCLUDE)
    
$(BLD)/main.exe: $(BLD)/main.o $(BLD)/unit.o $(BLD)/weapon.o $(BLD)/generic.o $(BLD)/inventory_item.o  include/shared.hpp
	$(CXX) $** -o $@ $(INCLUDE)
    
clean:
    @if exist $(BLD) rmdir /S /Q $(BLD)

create_dirs:
    @if not exist $(BLD) mkdir $(BLD)
