#include <stdio.h>
#include <stdlib.h>

#define TAPE_PADDING 10
#define EMPTY_TAPE_SYMBOL '$empty_symbol'

int tape_size = 0;
char* tape;

void print_tape() {
    for (size_t i = 0; i < tape_size; i++) {
        printf("%c ", tape[i]);
    }
    printf("\n");
}

char* init_tape(const char* const init_data[], const int init_data_size) {
    if (init_data_size <= 0) {
        printf("ERROR: Empty tape\n");
        exit(EXIT_FAILURE);
    }
    tape_size = TAPE_PADDING + init_data_size + TAPE_PADDING;
    tape = malloc(sizeof(char) * tape_size);

    // Set all padding fields before the initial tape data to the empty tape symbol
    for (size_t i = 0; i < TAPE_PADDING; i++) {
        tape[i] = EMPTY_TAPE_SYMBOL;
    }

    // Copy init data to the tape
    for (size_t i = 0; i < init_data_size; i++) {
        // Copy only the first byte from each argument
        tape[TAPE_PADDING + i] = *init_data[i];
    }

    // Set all padding fields after the initial tape data to the empty tape symbol
    for (size_t i = TAPE_PADDING+init_data_size; i < tape_size; i++) {
        tape[i] = EMPTY_TAPE_SYMBOL;
    }

    return tape;
}

int main(const int argc, const char* const argv[]) {
    tape = init_tape(argv+1, argc-1);
    size_t index = TAPE_PADDING;
    char state = '$start';

    int running = 1;
    while (running) {
        printf("State %c\t\tHead index %3zu\n", state, index);
        char read = tape[index];
        const unsigned short key = state << 8 | read;
        switch (key) {
            $transitions
            default:
                running = 0;
                break;
        }
    }

    print_tape();
}