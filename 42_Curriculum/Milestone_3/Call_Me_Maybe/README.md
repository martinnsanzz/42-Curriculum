*This project has been created as part of the 42 curriculum by masanz-s*

[Martin GitHub](https://github.com/martinnsanzz)

# Call-Me-Maybe
*'Hey, I just met you, and this is crazy.' Carly Ray Jepsen (2011)*


## Description

**Call-Me-Maybe** is a python based project from the 42 curriculum. The goal of this project is 
to create a function calling engine utilizing an llm_sdk provided by the subject using the 
model *'Qwen/Qwen3-0.6B'* from HuggingFace website. 

The engine turns natural-language prompts into structured, schema-compliant function calls,
without ever letting the language model "hallucinate" free-form text. Given a prompt such as
*"What is the sum of 40 and 2?"*, the program does not answer the question directly — it resolves
it to a function name (`fn_add_numbers`) and a set of correctly typed arguments 
(`{"a": 40.0, "b": 2.0}`).

The core challenge is that the model (a small, ~0.6B-parameter LLM accessed through
the `llm_sdk` package) is not reliable enough to produce valid JSON. Instead of prompting and
hoping, this project drives the model's generation **token by token**, masking its output logits
at every step so that only tokens consistent with a valid function name, a valid argument type,
and the content of the original prompt can ever be selected. The result is output that is always
syntactically valid and always grounded in something the user actually wrote — accuracy is
enforced structurally, not statistically.

For every prompt in the input file, the engine resolves a function name and its parameters, then writes one JSON object per prompt to `data/output/function_calling_results.json`.

---

## Instructions
The project uses `uv` for dependency management and a `Makefile` to wrap the common commands.

**Make file commands:**

- `make install`: Creates a virtual environment and syncs dependencies.
- `make run`: Runs the program using the following flags;

```
--functions_definition ${FUNC_DEF}
--input ${INPUT}
--output ${OUTPUT}
```

Where:
- *{FUNC_DEF}* is the path to the JSON file of the available functions
definitions (`data/input/functions_definition.json`)
- *{INPUT}* is the path to the JSON file of the user
prompt (`data/input/function_calling_tests.json`)
- *{OUTPUT}* is the path to the JSON file where the output of the program will be 
written (`data/output/function_calling_results.json`).

By default `--functions_definition`, `--input`, and `--output` point to the files under 
`data/input/` and `data/output/` described above. If `--test` is supplied, it is used in place 
of `--input`. Passing `--visual_mode True` prints a title banner and a per-prompt breakdown (function name, extracted parameters, timing, and any error) as the engine runs, followed by aggregate stats.

A few other Makefile targets are available:

- `make debug`: Runs the program with the same flags through *pdb* or *pudb* if *DEBUGGER=pudb*,
allowing you to step through execution interactively.
- `make test`: Runs the program with a *prompt_test.json* file to test individual things/prompts
- `make visual`: Runs the program with *VISUAL_MODE=TRUE* flag which displays generation information
step by step for more clarity and proper error messages.
- `make lint`: Runs `flake8` and `mypy`  with some extra flags and reports any issues without
failing the build.
- `make lint-strict`: Enforces both tools more rigorously and will fail on any violation.
- `make clean`: Removes cache directories (`__pycache__`, `.mypy_cache`), compiled `.pyc` files.
- `make fclean` performs the same cleanup and additionally deletes the virtual
environment entirely. 
**Note: If the venv is active in your shell at that point, remember to run `deactivate` afterward.**

---

## Additional requirements
### Algorithm explanation
When I started thinking into how to get the *functions names* and the *parameters* and generating
consisten json format I realized one big thing. The llm has a total of 150k possible tokens and
to generate correct json format the llm needs to constantly know where in the json schema it is
which would exceed the 5 minute runtime limit by a lot since it needs to think for every loop the
possibility of each token and select only one that would be still consider a valid json if choosen
and that would be a lot of steps even before attempting to extract the function names and 
the parameters. 

Also by checking the vocab.json file from the llm I realized that most of those tokens are not
even valid ASCII since there's chinese characters, emojis and a lot of tokens that im not even
going to print. 

After this thinking I decided to make the json format myself inside the `prompt class` and make
the llm only choose the *function names* and the *parameters* that way I could reduce the allowed
tokens to the type ["string", "bool", "number", "integer].

For this I filtered the token by first replacing the `\u0120 - Ġ` character which in Qwen language
model represents a leading `" " - space` for the actual space character `" "` for easier usage and
masking the vocabs that match the regex of the asked type, based on json types.

```python
    match type:
        case "string":
            pattern = compile(r'^[\x20-\x7e]+$')
        case "integer":
            pattern = compile(r'^-?\d+$')
        case "number":
            pattern = compile(r'^-?\d*\.?\d*$')
        case "boolean":
            pattern = compile(r'^[truefals]+$')
```

This reduces the 150k available vocab words to:
- number -> 13
- integer -> 10
- string -> 90913
- boolean -> 846

With this in place I divided the constrained decoding into 2 parts.

<u><b>Function name</b></u>

To get the correct function name I make a list with all the available functions names. I pass
the prompt to the llm and create a numpy array of the same size as the available tokens seeting
all the indexes to -np.inf.

Then I loop through the available candidates and restore the original probability given to the
candidates that fit as a valid prefix for the available function names. Once all the logits are
selected I get the maximum score and append it to the accumulated value until it machtes a valid
function name. The llm is the one responsible to choose the correct function based on the prompt.
Which looks accurate most of the time if there's a small amount of available functions.

<u><b>Parameters name</b></u>

For the parameters I used the same loop logic but instead of a list of function names I divided
the prompt in words with some conditions. If theres words / string with `' '` or `" "` and there's
not a `": "` in the prompt I just pass a list of the words between quotes to choose from since
those are most likely the desired parameter.

For number / integer type I extract the numbers within the prompt and format it in a list
depending on the desired parameter type since numbers can have decimal but integers are whole
numbers. Again the llm is the one choosing something from the list which I would say is 100%
accurate for simple prompts `Greet john` -> `john` is choosen 100% of the time or prompts with
numbers. For long prompts like regex or paths the llm only sometimes puts the parameter in the
correct place but is always 100% correct type.

### Design decisions
An important implementation for this project in order to reduce the amount of small_llm
instances was to instantiate one small_llm class once in the main file `cli.py` and pass
it to the `Engine` and use it accross all prompts. This made the code quite faster since 
creating a new llm instance every time I had to use it lets say per `Prompt` object would
make the program run slower since it needs to load many parameters and it uses many heavy
libraries like `Transformers` or `Torch`.

Another design that help me out in the late stages of the project was the `Parameter` class
which helped isolating each parameter type in a separate function and redirect the type I
wanted based on one condition. This made testing and finding errors easier since they
were now all separated in different functions.

The states for numbers and strings I thought it was gonna be more helpfull to do certain
things based on where I was in the accumulated token but I dont think its being as usefull
since I only check if its *DONE* to break the loop but I base this on a condition if
the accumulated tokens so far match a word in the prompt.

The split prompt logic was very usefull but was very likely to cause errors and had many
edge cases depending if it was a string / number / integer. Overall I think it made the llm
answer more precisely since I only pass the allowed words that can be used at that specific
moment removing the already used ones which works perfect for numbers but for strings is
more likely to cause mistakes since the choosing of the correct parameter for each case is
up to intuition.

### Performance analysis
As explained in the algorithm explanation performance varies based on the available vocab which
gets masked based on the type we are trying to get.

```python
    match type:
        case "string":
            pattern = compile(r'^[\x20-\x7e]+$')
        case "integer":
            pattern = compile(r'^-?\d+$')
        case "number":
            pattern = compile(r'^-?\d*\.?\d*$')
        case "boolean":
            pattern = compile(r'^[truefals]+$')
```

This reduces the 150k available vocab words to:
- number -> 13
- integers -> 10
- string -> 90913
- boolean -> 846

For very lengthy prompts the execution time increases due to having to encode and decode more
tokens.

Also the size of the dependencies from the llm are quite big which makes installation time longer.
For this I avoid `make fclean` as much as possible since creating the `.venv` with all
dependencies every time takes a lot of time. Another thing to take into consideration is the size
of the `.venv` which is 4.9Gb with everything install. The maximum space we have in the home
directory at Codam is 10Gb so correct clean-up is required since not enough space means the
program not running for missing dependencies. A solution for this would be to put the project in
`goinfree/` which is local and runs faster in comparison to `sgoingfree`.

*Note: I didnt do it because I want all my project in one github repository.*

Also speed on running this program is based on the computers CPU generation and architecture.

### Challenges faced
During the development of this project many challenges where faced, specially when dealing with the
constrained decoding. Initially I wasn't passing a system prompt which resulted in the llm talking
non-sense constantly and not printing what I wanted, showing some weird patterns such as repeating
the prompt over an over. 

This made me discover *llm commands* such as:
- `<|im_start|>system...<|im_end|>` -> For system commands
- `<|im_start|>user...<|im_end|>` -> For the actual user prompt
- `<|im_start|>assitant` -> What the assystant needs to write down
- \<think>...\</think> -> To avoid initial random text of the llm

This helped me get more clear and more accurate results (Not 100% correct all of the time)

The biggest challenge faced that I couldn't overcome 100% was the llm choosing the correct
parameter from the prompt at the required moment. Example would be that for source string instead of 
choosing the string of the prompt in `' '` when there's multiple quoted words, the llm choose a random 
quoted word which I think is the limitation of the context. Or when needing to choose a path it chooses
a random word regardless of passing in the system prompt examples of how a path should look like. 
With my implementation I always provide the quoted words/strings first as the available words to 
choose from but is up to the llm and the context to decide which is not always 100% right when many are
available. 

Regardless of changing the prompt, avoiding the `think / nothink` command or providing examples like
other peers I wasnt able to pass all 100% moulinette tests or provided tests. Regardless of the
parameters not being 100% precise the function names always is correct maybe because is simpler to
interpret what function should go with a prompt.

An extra step I did when testing for the last times, I made it so it selects the longest
"word / string" if the parameter name is source-string or path since they tend to be longer
than a separate word. This makes the llm always choose the correct one in those specific
cases.

### Testing strategy
Due to the layer of complexity of this projct and the slow import time for the *Small_LLM_Model*
multiple testing strategies where used.

For the initial steps of the project where the llm wasn't being used the regular `make debug`
command was used to test the flags, and the *.json files* where being loaded
and validated correctly.

Once the llm was involved and loading times drastically increased archaic aproaches where used,
such as `print()` to see specific values inside loops and see what the logits and llm thinking was
and an error.log file to output the extensive text or the 150k tokens in each iteration since the
terminal was big enough to scroll back and see everthing.

Also in order to test different wrong outputs and empty files or prompts the `make test` command 
was used. In `data/test/prompt_test.json` I would test (Wrong json, wrong types, empty prompts,
super complex promtps) to be able to account for most errors.

### Example usage
Install dependencies and run against the default input files:

```bash
make install
make run # or make visual for step-by-step output
```

Run against a custom set of files directly:

```bash
uv run python -m src --functions_definition data/input/functions_definition.json --input data/input/function_calling_tests.json --output data/output/function_calling_results.json
```

Given the input:

`data/input/functions_calling_tests.json`
```json
[
  { "prompt": "What is the sum of 2 and 3?" },
  { "prompt": "Reverse the string 'hello'" }
]
```

`data/input/functions_definition.json`
```json
[
  {
    "name": "fn_add_numbers",
    "description": "Add two numbers together and return their sum.",
    "parameters": {
      "a": {
        "type": "number"
      },
      "b": {
        "type": "number"
      }
    },
    "returns": {
      "type": "number"
    }
  },
  {
    "name": "fn_greet",
    "description": "Generate a greeting message for a person by name.",
    "parameters": {
      "name": {
        "type": "string"
      }
    },
    "returns": {
      "type": "string"
    }
  }
]
```

The program produces:

`data/output/function_calling_result.json`
```json
[
    {
        "prompt": "What is the sum of 2 and 3?",
        "name": "fn_add_numbers",
        "parameters": {"a": 2.0, "b": 3.0}
    },
    {
        "prompt": "Reverse the string 'hello'",
        "name": "fn_reverse_string",
        "parameters": {"s": "hello"}
    }
]
```

---

## Resources
This is a list of multiple resources use through out the life-cycle of the project

### LLM
- [Constrained Decoding](https://www.aidancooper.co.uk/constrained-decoding/)
- [Implementing Constrained Decoding](https://medium.com/@albersj66/part-6-implementing-constrained-decoding-for-phi-3-vision-2c72a1be6a17)
- [Function calling with LLMs](https://www.promptingguide.ai/applications/function_calling)
- [Understand Tokens](https://learn.microsoft.com/en-us/dotnet/ai/conceptual/understanding-tokens)
- [LLMs -WIKI](https://en.wikipedia.org/wiki/Large_language_model)
- [How do LLMs work](https://medium.com/data-science-at-microsoft/how-large-language-models-work-91c362f5b78f)
- [Structured Output from LLMs](https://www.youtube.com/watch?v=xpvFinvqRCA)
- [Qwen Model](https://huggingface.co/Qwen/Qwen3-0.6B)
- [Qwen Docs](https://qwen.readthedocs.io/en/latest/getting_started/concepts.html)
- [How Constrained Decoding Works](https://medium.com/@sebuzdugan/make-invalid-json-impossible-how-constrained-decoding-actually-works-5a512106396a)

### VENV
- [UV and Environments](https://docs.astral.sh/uv/pip/environments/)
- [Writing your pyproject.toml](https://packaging.python.org/en/latest/guides/writing-pyproject-toml/)

### Python Specifics
- [Argparse library](https://docs.python.org/3/library/argparse.html)
- [Argparse tutorial](https://www.youtube.com/watch?v=tirLko5urBo)
- [Working with JSON data in Python](https://realpython.com/python-json/)
- [Using Qwen in Python](https://qwen.readthedocs.io/en/stable/getting_started/quickstart.html)
- [Pytorch Doc](https://pytorch.org/)
- [Regular Expressions in Python](https://www.youtube.com/watch?v=wnuBwl2ekmo)
- [Python RegEx](https://www.w3schools.com/python/python_regex.asp)

---

## AI Usage

**AI was NOT used to generate code.** All function implementations were written by Martin™.

**Where AI was used:**
- Explaining complex concepts related to LLM's
- Help fix unprecedented errors due to the LLM probability of choosing logits
- Suggested constrained decoding approaches for more accurate results
- Provided commands and guidance into optimizing the computer for installing all dependencies
- Writting accurate docstrings
- Suggesting best way of organizing the files and functions within them for clarity and
readability

This README.md file was done by **human fingers, sweat and tears**. No AI wrote a single
line of text :)

---