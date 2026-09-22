# Built-in modules
from enum import Enum, auto
from re import match, compile

STRING_PATTERN = compile(r'^"(?:[^"\\\x00-\x1f]|\\ \
                         [nrtbf"\\/]|\\u[0-9a-fA-F]{4})*"$')
NUMBER_PATTERN = compile(r'^-?\d+\.\d+$')
INTEGER_PATTERN = compile(r'^-?\d+$')
VALID_BOOLEAN = [True, False]


class NumState(Enum):
    """Represents the state of the generated number"""
    START = auto()
    IN_MINUS = auto()
    IN_DOT = auto()
    INT_DIGITS = auto()
    FRAC_DIGITS = auto()
    DONE = auto()


class StrState(Enum):
    """Represents the state of the generated string"""
    NOT_FINISHED = auto()
    FINISHED = auto()


def get_num_state(num: str, prompt: str, param_type: str) -> NumState:
    """Determine the decoding state of an accumulated numeric string.

    Checks `num` against `INTEGER_PATTERN`/`NUMBER_PATTERN` and against
    the numeric tokens present in `prompt` (with trailing "." or "?"
    stripped) to decide whether decoding is complete. If not complete,
    falls back to classifying `num` by its current shape (start of a
    negative sign, trailing decimal point, in-progress fractional
    digits, or in-progress integer digits).

    Args:
        num: The accumulated numeric string decoded so far.
        prompt: The source prompt text to check `num` against.
        param_type: Either "number" or "integer"; selects which
            pattern/comparison rules apply.

    Returns:
        NumState: `NumState.DONE` if `num` matches a number present in
        `prompt`; otherwise one of `NumState.START`,
        `NumState.IN_MINUS`, `NumState.IN_DOT`,
        `NumState.FRAC_DIGITS`, or `NumState.INT_DIGITS` describing
        the in-progress decoding state.
    """
    if not num:
        return NumState.START

    clean_prompt = prompt.rstrip(".?")

    if param_type == "integer":
        if match(INTEGER_PATTERN, num) and num in clean_prompt.split():
            return NumState.DONE
        elif match(INTEGER_PATTERN, num) and num not in clean_prompt.split():
            return NumState.INT_DIGITS
    elif param_type == "number":
        prompt_float = []

        for word in clean_prompt.split():
            try:
                prompt_float.append(str(float(word)))
            except ValueError:
                prompt_float.append(word)
        if match(NUMBER_PATTERN, num) and num in prompt_float:
            return NumState.DONE

    if num == "-":
        return NumState.IN_MINUS
    elif num[-1] == ".":
        return NumState.IN_DOT
    elif "." in num and num[-1].isdigit():
        return NumState.FRAC_DIGITS
    else:
        return NumState.INT_DIGITS


def get_str_state(param: str, available_word: list[str]) -> StrState:
    """Determine whether a candidate string value has been fully matched.

    Args:
        param: The accumulated candidate string decoded so far.
        available_word: The pool of extracted candidate words/phrases
            to check `param` against.

    Returns:
        StrState: `StrState.FINISHED` if `param` is present in
        `available_word`; otherwise `StrState.NOT_FINISHED`.
    """
    if param in available_word:
        return StrState.FINISHED
    else:
        return StrState.NOT_FINISHED
