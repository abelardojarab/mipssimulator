COMP            = g++ 
FLAGS           = -g -Wall
LIBS            =  
OBJ_LIST_SIMULATOR        = main.o processor.o instruction.o
OBJ_LIST_ASSEMBLER       = assembler.o
TARGET_SIMULATOR     = simulator
TARGET_ASSEMBLER    = assembler

all: $(TARGET_SIMULATOR) $(TARGET_ASSEMBLER)

sim: $(TARGET_SIMULATOR)

asm: $(TARGET_ASSEMBLER)

$(TARGET_SIMULATOR): $(OBJ_LIST_SIMULATOR)
	$(COMP) $(FLAGS) -o $(TARGET_SIMULATOR) $(OBJ_LIST_SIMULATOR) $(LIBS)

$(TARGET_ASSEMBLER): $(OBJ_LIST_ASSEMBLER)
	$(COMP) $(FLAGS) -o $(TARGET_ASSEMBLER) $(OBJ_LIST_ASSEMBLER) $(LIBS)

%.o: %.cpp %.h
	$(COMP) $(FLAGS) -c $<

clean:
	rm -f *.o *~ core $(TARGET_NAME)
