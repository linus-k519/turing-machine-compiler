from typing import Dict, List, Iterable, Tuple

import transition

Transition = Dict[str, str]
MachineParams = Dict[str, str]


def is_machine_param(pairs: MachineParams) -> bool:
    """Checks if a list of pairs is a machine parameter."""

    compiler_params = ['start', 'empty_symbol']
    return any([key in compiler_params for key in pairs])


def get_pairs(line: List[str]) -> Dict[str, str]:
    """Collects key value pairs from a line.

    There may be several key value pairs in one line. These are separated by spaces.

    Example:
        line = 'language python version 3.9 answer 42'
        becomes
        {
            'language': 'python',
            'version': '3.9',
            'answer': 42'
        }
    """

    pairs = dict()
    for i in range(0, len(line), 2):
        key = line[i].lower()
        value = line[i + 1]
        pairs[key] = value
    return pairs


def machine(lines: Iterable[str]) -> Tuple[List[Transition], MachineParams]:
    """Parses a machine into its transitions and parameters"""

    transitions: List[Dict[str, str]] = list()
    machine_params: Dict[str, str] = dict()
    for line_nr, line in enumerate(lines):
        split_line = line.split()
        pairs = get_pairs(split_line)
        if transition.valid(pairs):
            transitions.append(pairs)
        elif is_machine_param(pairs):
            for key, value in pairs.items():
                machine_params[key] = value
        else:
            print(f'ERROR at line {line_nr}: Unknown parameter')
            exit()
    return transitions, machine_params
