# turing-machine-compiler

A compiler that translates deterministic Turing machines into C code and then compiles it.

[Live demo](https://linuskmr.github.io/turing-machine-compiler/) (Interpreter)

## Turing machine file (.tm)

This file describes the properties of a Turing machine. Besides transitions, the start state and the empty symbol can
be specified.

> NOTE: States and symbols may only contain one ASCII character.

### Machine parameters

| Required | Synopsis              | Default          |
| -------- | --------------------- | ---------------- |
| No       | START _state_         | START 1          |
| No       | EMPTY_SYMBOL _symbol_ | EMPTY_SYMBOL _   |

### Transitions

FROM _current_state_ READ _symbol_a_ WRITE _symbol_b_ GOTO _next_state_ MOVE _left/right/none_

### Example

This machine doubles the number of ones on the tape.

```
START 1
EMPTY_SYMBOL 0
FROM	1	READ	1	WRITE	0	GOTO	2	MOVE	right
FROM	1	READ	0	WRITE	0	GOTO	6	MOVE	none
FROM	2	READ	1	WRITE	1	GOTO	2	MOVE	right
FROM	2	READ	0	WRITE	0	GOTO	3	MOVE	right
FROM	3	READ	1	WRITE	1	GOTO	3	MOVE	right
FROM	3	READ	0	WRITE	1	GOTO	4	MOVE	left
FROM	4	READ	1	WRITE	1	GOTO	4	MOVE	left
FROM	4	READ	0	WRITE	0	GOTO	5	MOVE	left
FROM	5	READ	1	WRITE	1	GOTO	5	MOVE	left
FROM	5	READ	0	WRITE	1	GOTO	1	MOVE	right
```

## Compiler

Compile a turing machine file to an executable binary.

Requirements: python3, gcc

```
usage: turing-machine-compiler [-h] [-o OUTPUT] [--tape-padding TAPE_PADDING]
                               [-d DEBUG]
                               FILE

Turing Machine Compiler

positional arguments:
  FILE                  The path to the turing machine file.

optional arguments:
  -h, --help            show this help message and exit
  -o OUTPUT, --output OUTPUT
                        The name of the output binary.
  --tape-padding TAPE_PADDING
                        The number of elements the tape is padded with
  -d DEBUG, --debug DEBUG
                        Lets the machine print debugging messages
```

### Example

Compile the machine in the file `machine.tm`.

```bash
./main.py machine.tm
```

## Executable binary

The executable takes the tape items as command line arguments.

### Example

Running the machine with a tape of `[1, 1, 0]`.

```
./machine 1 1 0
```
