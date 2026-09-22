# Built-in modules
from pathlib import Path
from json import loads, JSONDecodeError
from typing import Any, TypeAlias

# Local modules
from .exceptions import CustomError

FunctionsList: TypeAlias = list[dict[str, Any]]
FunctionLookup: TypeAlias = dict[str, dict[str, Any]]


def load_json(json_file: Path) -> list[dict[str, Any]]:
    """Load and parse a JSON file into a list of dictionaries.

    Args:
        json_file (Path): Path to the JSON file to load.

    Returns:
        list[dict[str, Any]]: The parsed JSON content.

    Raises:
        CustomError: If the file is empty, does not exist, or contains
            malformed JSON.
    """
    try:
        with open(json_file) as f:
            raw_content = f.read()
            if not raw_content:
                raise CustomError(f"File {json_file} is empty")
            content: list[dict[str, str]] = loads(raw_content)
            return content
    except FileNotFoundError as e:
        raise CustomError(f"File not found: {e}") from e
    except JSONDecodeError as e:
        raise CustomError(f"Wrong JSON format in {json_file}: {e}") from e


def build_function_lookup(functions_json: FunctionsList) -> FunctionLookup:
    """Build and validate a name-keyed lookup table of function definitions."""
    function_lookup = {func["name"]: func for func in functions_json}

    return validate_functions(function_lookup)


def validate_functions(functions_lookup: FunctionLookup) -> FunctionLookup:
    """Validate that all parameter and return types are accepted types.

    Checks every function definition in ``functions_lookup`` and
    ensures each parameter's ``"type"`` and the function's
    ``"returns"["type"]`` are one of ``"string"`` or ``"number"``.

    Args:
        functions_lookup (FunctionLookup): Mapping of function name to
            its definition dict, each expected to contain
            ``"parameters"`` (mapping of param name to a dict with a
            ``"type"`` key) and ``"returns"`` (a dict with a
            ``"type"`` key).

    Returns:
        FunctionLookup: The same ``functions_lookup``, unchanged, if
            all types are valid.

    Raises:
        CustomError: If any parameter type or return type is not in
            ``["string", "number"]``.
    """
    accepted_types = ["string", "number", "integer", "boolean"]

    for item in functions_lookup:
        for param in functions_lookup[item]["parameters"]:
            var_type = functions_lookup[item]["parameters"][param]["type"]
            if var_type not in accepted_types:
                raise CustomError(f"Wrong parameter type: '{var_type}'."
                                  f" Accepted types {accepted_types}")
        if functions_lookup[item]["returns"]["type"] not in accepted_types:
            return_type = functions_lookup[item]["returns"]["type"]
            raise CustomError(f"Wrong return type: '{return_type}'."
                              f" Accepted types {accepted_types}")
    return functions_lookup
