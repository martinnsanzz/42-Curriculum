# Built-in Modules
from argparse import Namespace, ArgumentParser, ArgumentError
from pathlib import Path
import os

# Local Modules
"""This makes all loading bars not show on terminal"""
os.environ["HF_HUB_DISABLE_PROGRESS_BARS"] = "1"
from llm_sdk import Small_LLM_Model # noqa
from .classes import Prompt # noqa
from .core import load_json, build_function_lookup, Engine, write_output, CustomError # noqa


def _main() -> None:
    """Run the CLI entry point for the function-calling engine.

    Parses command-line arguments, builds the engine, runs the prompt
    loop, and writes the result to the output file. Any
    ``ArgumentError``, ``KeyError``, or ``CustomError`` raised during
    this process is caught, printed as a formatted error message, and
    terminates the program via ``quit()``.
    """
    try:
        args = args_parser()

        engine = setup_engine(args)
        llm_output = engine.prompt_loop()

        write_output(args.output, llm_output)
    except ArgumentError as e:
        print(f"\033[91mIncorrect argument on command line:\n   - {e}\033[0m")
        quit()
    except KeyError:
        print("\033[91mExpected key in '.json file': 'prompt'\033[0m")
        quit()
    except CustomError as e:
        print(f"\033[91mERROR:\n   - {e}\033[0m")
        quit()


def args_parser() -> Namespace:
    """Parse command-line arguments for the function-calling engine.

    Returns:
        Namespace: Parsed arguments containing ``functions_definition``,
            ``input``, ``output``, ``visual_mode``, and ``test``.

    Raises:
        ArgumentError: If unrecognized command-line arguments remain
            after parsing.
    """
    parser = ArgumentParser()
    parser.add_argument("--functions_definition",
                        type=Path,
                        default="data/input/functions_definition.json")
    parser.add_argument("--input",
                        type=Path,
                        default="data/input/function_calling_tests.json")
    parser.add_argument("--output",
                        type=Path,
                        default="data/output/function_calling_results.json")
    parser.add_argument("--visual_mode",
                        type=bool,
                        default=False)
    parser.add_argument("--test",
                        type=Path,
                        default=None)

    args, argv = parser.parse_known_args()

    if argv:
        raise ArgumentError(None,
                            "Correct usage: src [--functi"
                            "ons_definition <FUNCTIONS_DEFINITION>] "
                            "[--input <INPUT>] [--output <OUTPUT>]"
                            )
    return args


def setup_engine(args: Namespace) -> Engine:
    """Build and configure an Engine from parsed command-line arguments.

    Loads the prompt dataset (``args.test`` if provided, otherwise
    ``args.input``) and the function definitions file, converts each
    raw prompt into a ``Prompt`` object with trailing "?" and "."
    characters removed, and assembles the function lookup table.

    Args:
        args (Namespace): Parsed arguments as returned by
            ``args_parser``. Must expose ``test``, ``input``,
            ``functions_definition``, and ``visual_mode``.

    Returns:
        Engine: An engine initialized with the loaded prompts, the
            function lookup table, a new ``Small_LLM_Model`` instance,
            and the requested visual mode.
    """
    small_llm = Small_LLM_Model()

    prompt_json = load_json(args.test) if args.test else load_json(args.input)
    functions_json = load_json(args.functions_definition)

    functions_lookup = build_function_lookup(functions_json)

    prompts: list[Prompt] = []
    for item in prompt_json:
        prompt = Prompt(prompt=item["prompt"])
        prompts.append(prompt)

    engine = Engine(
        prompts=prompts,
        functions_lookup=functions_lookup,
        small_llm=small_llm,
        visual_mode=args.visual_mode
    )
    return engine
