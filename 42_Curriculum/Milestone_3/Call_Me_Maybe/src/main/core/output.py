# Built-in modules
from json import dump, loads
from pathlib import Path


def write_output(output_file: Path, llm_output: list[str]) -> None:
    """Write LLM output as formatted JSON to a file.

    Each element of ``llm_output`` is parsed as a JSON string and the
    resulting list is dumped to ``output_file`` as indented JSON.
    """
    try:
        with open(output_file, 'w') as f:
            llm_formated = [loads(obj) for obj in llm_output]
            dump(llm_formated, fp=f, indent=4)
    except FileNotFoundError:
        print("\033[91mError:\n Folder 'data/output' not found !!\033[0m")
        quit()
