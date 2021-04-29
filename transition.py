from functools import lru_cache, reduce
from string import Template
from typing import Dict, Iterable

TEMPLATE_FILE = 'transition.c'
Transition = Dict[str, str]


def valid(text: Transition) -> bool:
    """Checks if a transition is correct."""
    keys = ['from', 'read', 'write', 'goto', 'move']
    return all([key in text for key in keys])


@lru_cache(maxsize=1)
def _load_template() -> Template:
    """Loads the transition template from file."""
    with open(TEMPLATE_FILE) as file:
        return Template(file.read())


def build(transition: Transition) -> str:
    """Builds one transition to its c code."""
    if transition['move'] in ['left', 'l']:
        transition['move'] = 'l'
    elif transition['move'] in ['right', 'r']:
        transition['move_'] = 'r'
    else:
        transition['move'] = '_'

    template = _load_template()
    return template.substitute(transition)


def build_all(transitions: Iterable[Transition]) -> str:
    """Builds all transitions into their c code."""

    return reduce(lambda acc, cur: acc + build(cur), transitions, '')
