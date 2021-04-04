from functools import lru_cache, reduce
from typing import List, Dict, Iterable
from string import Template

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
        transition['move'] = 'index--;'
    elif transition['move'] in ['right', 'r']:
        transition['move'] = 'index++;'
    else:
        transition['move'] = ''

    template = _load_template()
    return template.substitute(transition)


def build_all(transitions: Iterable[Transition]) -> str:
    """Builds all transitions into their c code."""

    return reduce(lambda acc, cur: acc + build(cur), transitions, '')
