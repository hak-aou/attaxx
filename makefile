CC=gcc
CFLAG=-Wall -ansi -pedantic
LDFLAGS=-lMLV
ALL=$(OBJ)attaxx_aoudia_bakhti.o

# .exe
EXE=game
BIN=./bin/
# .c
SRC=./src/
# .o
OBJ=./obj/

$(BIN)$(EXE): $(ALL)
	$(CC) -o $@ $^ $(CFLAG) $(LDFLAGS)

attaxx_aoudia_bakhti.o: $(SRC)attaxx_aoudia_bakhti.c

$(OBJ)%.o: $(SRC)%.c
	$(CC) -c $< -o $@ $(CFLAG)

clean:
	rm -f $(OBJ)*.o
	rm -f $(BIN)*