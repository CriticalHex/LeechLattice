TARGET = LeechLattice.exe #the name of the compiled executable
SRC_FILES = main.cpp Lattice.cpp

OBJECTS = $(SRC_FILES:.cpp=.o) #signifies a relationship between the src_files and their to be compiled .o files

INCLUDE_PATH = -I"C:\mingw64\include" #for user installed libraries

LIBS = -lsfml-graphics -lsfml-window -lsfml-system #for user installed libraries, these are specifically for SFML
LIB_PATH = #-L"C:\mingw64\include\libs" #for user installed libraries
CXX = g++ #the compiler name

all: $(TARGET) #when you run the make command from terminal as "make" this is called, sends to the target command

#the target command, compiles the executable, relies on the .o files, so calls their creator
#the internal line is the command line command you would type to compile if you didn't have CMake
$(TARGET): $(OBJECTS) 
	$(CXX) -o $@ $^ $(LIB_PATH) $(LIBS)

#the .o compile command, compiles the .o files, relies on the .cpp files as specified at the bottom
#the internal line is the command line command you would type to compile if you didn't have CMake
%.o: %.cpp
	$(CXX) -c $< -o $@ $(INCLUDE_PATH)

#if you call make as "make clean" this would be called, deleting the compiled files.
clean:
	@$(RM) $(OBJECTS) $(TARGET)

#"make run" would execute the compiled executable
run:
	@$(TARGET)

#signifies which commands should only be run when passed as arguments like above
.PHONY: clean run

#the dependency tree, main.o needs main.cpp, which needs the two classes it uses,
#so those get compiled, but they depend on something, etc
main.o: main.cpp Lattice.o
Lattice.o: Lattice.cpp