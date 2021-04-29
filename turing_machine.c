#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#define EMPTY_TAPE_SYMBOL '$empty_symbol'
#define TAPE_GROW_CONSTANT 10

// A tape
typedef struct Tape_t {
    // The number of items in this tape
    size_t size;
    // The pointer to the data.
    char *data;
    // The index offset by which requested indexes have to be subtracted.
    size_t index_offset;
} Tape_t;


size_t tape_get_real_index(Tape_t *tape, ssize_t index) {
    return index + tape->index_offset;
}

// Grows the tape on the left side by TAPE_GROW_CONSTANT elements.
void tape_grow_left(Tape_t *tape) {
    char *old_data = tape->data;
    size_t old_size = tape->size;

    // Allocate new memory
    tape->size += TAPE_GROW_CONSTANT;
    tape->data = malloc(sizeof(char) * tape->size);
    if (tape->data == NULL) {
        perror("tape_get_real_index: malloc new tape.data");
        exit(EXIT_FAILURE);
    }

    // Copy old tape
    memcpy(tape->data + TAPE_GROW_CONSTANT, old_data, old_size);
    tape->index_offset += TAPE_GROW_CONSTANT;
    free(old_data);

    // Fill with empty symbol
    for (size_t i = 0; i < TAPE_GROW_CONSTANT; i++) {
        tape->data[i] = EMPTY_TAPE_SYMBOL;
    }
}

// Grows the tape on the right side by TAPE_GROW_CONSTANT elements.
void tape_grow_right(Tape_t *tape) {
    // Allocate new memory
    size_t old_size = tape->size;
    tape->size += TAPE_GROW_CONSTANT;
    tape->data = realloc(tape->data, sizeof(char) * tape->size);
    if (tape->data == NULL) {
        exit(EXIT_FAILURE);
    }
    // Fill with empty symbol
    for (size_t i = old_size; i < tape->size; i++) {
        tape->data[i] = EMPTY_TAPE_SYMBOL;
    }
}

// Grows the tape if index is out of bounds.
void tape_grow_if_necessary(Tape_t *tape, ssize_t index) {
    // real_index could be negative, when the tape is not
    // large enough on the left side, so use ssize_t.
    ssize_t real_index = tape_get_real_index(tape, index);
    if (real_index < 0) {
        tape_grow_left(tape);
    } else if (real_index >= tape->size) {
        tape_grow_right(tape);
    }
}

// Return the item at index of the tape.
char tape_get(Tape_t *tape, ssize_t index) {
    tape_grow_if_necessary(tape, index);
    size_t real_index = tape_get_real_index(tape, index);
    return tape->data[real_index];
}

// Sets the item at index to the value in the tape.
void tape_set(Tape_t *tape, ssize_t index, char value) {
    tape_grow_if_necessary(tape, index);
    size_t real_index = tape_get_real_index(tape, index);
    tape->data[real_index] = value;
}


// Prints the tape to stdout
void print_tape(Tape_t *tape, ssize_t highlight_index) {
    size_t real_highlight_index = tape_get_real_index(tape, highlight_index);
    // Get first not-empty tape item
    size_t left = 0;
    for (size_t i = 0; i < tape->size; i++) {
        if (tape->data[i] != EMPTY_TAPE_SYMBOL) {
            left = i;
            break;
        }
    }
    if (left >= 1) {
        // Show one additional item
        left--;
    }

    // Get last not-empty tape item
    size_t right = 0;
    for (size_t i = tape->size - 1; i >= 0; i--) {
        if (tape->data[i] != EMPTY_TAPE_SYMBOL) {
            right = i;
            break;
        }
    }
    if (real_highlight_index > right) {
        // Show the real highlight index in any case
        right = real_highlight_index;
    }
    if (right < (tape->size) - 1) {
        // Show one additional item
        right++;
    }

    // Print tape from left to right
    for (size_t i = left; i <= right; i++) {
        if (i == real_highlight_index) {
            printf("[%c]", tape->data[i]);
        } else {
            putchar(tape->data[i]);
        }
        putchar(' ');
    }
    printf("\n");
}

// Init the tape by allocating memory and copying the initial data
void init_tape(Tape_t *tape, char const *const init_data[], int const init_data_size) {
    if (init_data_size <= 0) {
        printf("ERROR: Empty tape\n");
        exit(EXIT_FAILURE);
    }
    tape->index_offset = 0;
    tape->size = init_data_size;
    tape->data = malloc(sizeof(char) * tape->size);

    // Copy init data to the tape
    for (size_t i = 0; i < init_data_size; i++) {
        // Copy only the first byte from each argument
        tape->data[i] = *init_data[i];
    }
}

int main(int const argc, char const *const argv[]) {
    Tape_t tape;
    init_tape(&tape, argv + 1, argc - 1);
    char state = '$start';
    ssize_t index = 0;

    bool running = true;
    while (running) {
        char read = tape_get(&tape, index);
        if ($debug) {
            printf("State: %c\nTape: ", state);
            print_tape(&tape, index);
            putchar('\n');
        }
        const unsigned short key = state << 8 | read;
        switch (key) {
            $transitions
            default:
                running = 0;
                break;
        }
    }

    printf("Terminated in state: %c\nTape: ", state);
    print_tape(&tape, index);
}