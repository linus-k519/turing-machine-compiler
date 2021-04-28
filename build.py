import subprocess
from pathlib import Path
from string import Template
from typing import Dict

TURING_MACHINE_TEMPLATE_FILENAME = 'turing_machine.c'
DEFAULT_MACHINE_PARAMS = {
    'start': '1',
    'empty_symbol': '_',
}


def create_template_params(machine_params, transitions) -> Dict[str, str]:
    """Creates template params from the transitions and the machine parameters."""

    params = DEFAULT_MACHINE_PARAMS.copy()
    params.update(machine_params)
    params['transitions'] = transitions
    return params


def compile_to_binary(binary_filename: Path, params: Dict[str, str]):
    """Compiles the params to a binary named binary_filename."""

    # Read turing machine template file
    with open(TURING_MACHINE_TEMPLATE_FILENAME) as file:
        body_template = Template(file.read())
    body = body_template.substitute(params)

    # Run c compiler
    process = subprocess.Popen(['cc', '-x', 'c', '-O2', '-o', binary_filename, '-'], stdin=subprocess.PIPE)
    stdout_data = process.communicate(body.encode('ascii'))
    assert stdout_data == (None, None)
