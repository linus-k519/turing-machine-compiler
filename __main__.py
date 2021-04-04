import argparse
from pathlib import Path
import transition
import build
import parsing


parser = argparse.ArgumentParser(prog='turing-machine-compiler', description='Turing Machine Compiler')
parser.add_argument('filename', type=Path, metavar='FILE', help='The path to the turing machine file.')
parser.add_argument('-o', '--output', type=Path, metavar='OUTPUT', help='The name of the output binary.')


def main():
    args = parser.parse_args()

    with open(args.filename) as file:
        tm_file = file.readlines()
    transitions, machine_params = parsing.machine(tm_file)
    transitions = transition.build_all(transitions)
    template_params = build.create_template_params(machine_params, transitions)
    binary_filename = args.output if args.output else args.filename.with_suffix('')
    build.compile_to_binary(binary_filename, template_params)


if __name__ == '__main__':
    main()
