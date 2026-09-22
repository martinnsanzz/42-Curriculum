# Built-in modules
from typing import Self, Callable, Any
from re import compile

# Installed modules
from pydantic import BaseModel, ConfigDict, model_validator
from llm_sdk import Small_LLM_Model
import numpy as np

# Local modules
from .states import NumState, StrState, get_num_state, get_str_state
from .prompt_class import Prompt

VALID_BOOLEAN = ["true", "false"]
QUOTES = ["'", '"']

MAX_DECODE_NUM = 15
MAX_DECODE_STR = 40
MAX_DECODE_BOOL = 6


class Parameter(BaseModel):
    """Extract a single typed parameter value via constrained LLM decoding.

    Wraps a `Prompt` and a `Small_LLM_Model` instance to decode one
    parameter of a given `param_type` token-by-token, masking the model's
    logits at each step so only candidates consistent with the source
    prompt can be selected.

    Attributes:
        prompt_obj: The `Prompt` instance holding the source prompt text
            and any already-resolved parameter values.
        param_name: Name of the parameter being extracted.
        param_type: One of "number", "integer", "string", or "boolean".
        id_to_token: Mapping from vocabulary token id to its string form.
        small_llm: The local model SDK instance used for encoding and
            logit generation, or ``None`` if not yet attached.
    """
    model_config = ConfigDict(arbitrary_types_allowed=True)

    prompt_obj: Prompt
    param_name: str
    param_type: str
    id_to_token: dict[int, str]
    small_llm: Small_LLM_Model

    @model_validator(mode="after")
    def check_type(self) -> Self:
        """Validate that `param_type` is one of the supported types.

        Returns:
            Self: The validated model instance, unchanged.

        Raises:
            ValueError: If `param_type` is not one of "number", "integer",
                "string", or "boolean".
        """
        valid_types = ["number", "integer", "string", "boolean"]

        if self.param_type not in valid_types:
            raise ValueError("Incorrect parameter type.\n"
                             f" - Supported: {valid_types}\n"
                             f" - Found: {self.param_type}")
        return self

    def get_param(self, candidates: dict[int, str],
                  system_prompt: str) -> Any:
        """Dispatch parameter extraction to the type-specific decoder.

        Args:
            candidates: Mapping from candidate next-token id to its string
                form, as offered by the decoder for the current step.
            system_prompt: The system prompt text supplied to the LLM for
                this decoding step.

        Returns:
            int | float | str | bool: The decoded value, typed according
            to `self.param_type`.
        """
        type_to_func: dict[str, Callable[[dict[int, Any], str], Any]] = {
                "number": self.get_number,
                "integer": self.get_number,
                "string": self.get_string,
                "boolean": self.get_boolean,
        }

        return type_to_func[self.param_type](candidates, system_prompt)

    def get_number(self, candidates: dict[int, str],
                   system_prompt: str) -> float | int:
        """Decode a "number" or "integer" parameter via greedy decoding.

        At each step, masks the model's logits so that only tokens which
        would extend `accumulated` into a prefix of some numeric string
        found in the prompt (via `extract_nums_from_prompt`) survive,
        then greedily appends the highest-scoring surviving token until
        `get_num_state` reports `NumState.DONE`.

        Args:
            candidates: Mapping from candidate next-token id to its
                string form, as offered by the decoder for the current
                step.
            system_prompt: The system prompt text supplied to the LLM for
                this decoding step.

        Returns:
            float | int: The decoded value as a ``float`` when
            `self.param_type` is "number", or as an ``int`` when
            `self.param_type` is "integer".

        Raises:
            RuntimeError: If every candidate is masked to ``-inf`` at some
                step (no candidate extends toward any number in the
                prompt).
            TimeoutError: If decoding does not reach `NumState.DONE`
                within `MAX_DECODE_NUM` steps.
        """
        state = NumState.START
        accumulated = ""
        step_count = 0

        prompt = self.prompt_obj.prompt
        tokenization = self.small_llm.encode(system_prompt).tolist()[0]

        num_in_prompt = self.extract_nums_from_prompt(prompt, self.param_type)

        while state != NumState.DONE:
            logits = self.small_llm.get_logits_from_input_ids(tokenization)
            np_array = np.array(logits)
            np_array[:] = -np.inf

            for token_id, candidate in candidates.items():
                total = accumulated + candidate
                if any(name.startswith(total) for name in num_in_prompt):
                    np_array[token_id] = logits[token_id]

            if np.all(np.isneginf(np_array)):
                raise RuntimeError(
                    f"No valid candidate for {self.param_name!r} "
                    f"(type={self.param_type}); accumulated={accumulated!r}"
                )

            best_id = int(np.argmax(np_array))
            best_str = self.id_to_token.get(best_id, "")
            accumulated += best_str

            tokenization.append(best_id)

            state = get_num_state(accumulated, prompt, self.param_type)

            step_count += 1
            if step_count == MAX_DECODE_NUM:
                raise TimeoutError(
                    f"Exceeded {MAX_DECODE_NUM} steps "
                    f"for {self.param_name!r}; "
                    f"accumulated={accumulated!r}")

        return float(accumulated) if self.param_type == "number" \
            else int(accumulated)

    def get_string(self, candidates: dict[int, str],
                   system_prompt: str) -> str:
        """Decode a "string" parameter via greedy prefix-masked decoding.

        At each step, masks the model's logits so that only tokens which
        would extend `accumulated` into a prefix of some candidate word
        or phrase found in the prompt (via `extract_words_from_prompt`)
        survive, then greedily appends the highest-scoring surviving
        token until `get_str_state` reports `StrState.FINISHED`. The
        result has any surrounding single or double quotes stripped.

        Args:
            candidates: Mapping from candidate next-token id to its
                string form, as offered by the decoder for the current
                step.
            system_prompt: The system prompt text supplied to the LLM for
                this decoding step.

        Returns:
            str: The decoded string value, with surrounding quotes
            stripped.

        Raises:
            RuntimeError: If every candidate is masked to ``-inf`` at some
                step (no candidate extends toward any word in the
                prompt).
            TimeoutError: If decoding does not reach `StrState.FINISHED`
                within `MAX_DECODE_STR` steps.
        """
        state = StrState.NOT_FINISHED
        accumulated = ""
        step_count = 0

        prompt = self.prompt_obj.prompt
        tokenization = self.small_llm.encode(system_prompt).tolist()[0]

        if self.param_name in ["source_string", "path"]:
            words_in_prompt = self.extract_words_from_prompt(prompt, True)
        else:
            words_in_prompt = self.extract_words_from_prompt(prompt)

        while state != StrState.FINISHED:
            logits = self.small_llm.get_logits_from_input_ids(tokenization)
            np_array = np.array(logits)
            np_array[:] = -np.inf

            for token_id, candidate in candidates.items():
                total = accumulated + candidate
                if any(name.startswith(total) for name in words_in_prompt):
                    np_array[token_id] = logits[token_id]

            if np.all(np.isneginf(np_array)):
                raise RuntimeError(
                    f"No valid candidate for {self.param_name!r} "
                    f"(type={self.param_type}); accumulated={accumulated!r}"
                )

            best_id = int(np.argmax(np_array))
            best_str = self.id_to_token.get(best_id, "")
            accumulated += best_str

            tokenization.append(best_id)

            state = get_str_state(accumulated, words_in_prompt)

            step_count += 1
            if step_count == MAX_DECODE_STR:
                raise TimeoutError(
                    f"Exceeded {MAX_DECODE_STR} steps "
                    f"for {self.param_name!r}; "
                    f"accumulated={accumulated!r}")

        return accumulated.strip("'\"")

    def get_boolean(self, candidates: dict[int, str],
                    system_prompt: str) -> bool:
        """Decode a "boolean" parameter via greedy prefix-masked decoding.

        At each step, masks the model's logits so that only tokens which
        would extend `accumulated` into a prefix of ``"true"`` or
        ``"false"`` (`VALID_BOOLEAN`) survive, then greedily appends the
        highest-scoring surviving token until `accumulated` exactly
        matches one of `VALID_BOOLEAN`.

        Args:
            candidates: Mapping from candidate next-token id to its
                string form, as offered by the decoder for the current
                step.
            system_prompt: The system prompt text supplied to the LLM for
                this decoding step.

        Returns:
            bool: The decoded boolean value.

        Raises:
            RuntimeError: If every candidate is masked to ``-inf`` at some
                step (no candidate extends toward "true" or "false").
            TimeoutError: If `accumulated` does not match a value in
                `VALID_BOOLEAN` within `MAX_DECODE_BOOL` steps.
        """
        accumulated = ""
        step_count = 0

        tokenization = self.small_llm.encode(system_prompt).tolist()[0]

        while accumulated not in VALID_BOOLEAN:
            logits = self.small_llm.get_logits_from_input_ids(tokenization)
            np_array = np.array(logits)
            np_array[:] = -np.inf

            for token_id, candidate in candidates.items():
                total = accumulated + candidate
                if any(valid.startswith(total) for valid in VALID_BOOLEAN):
                    np_array[token_id] = logits[token_id]

            if np.all(np.isneginf(np_array)):
                raise RuntimeError(
                    f"No valid candidate for {self.param_name!r} "
                    f"(type={self.param_type}); accumulated={accumulated!r}"
                )

            best_id = int(np.argmax(np_array))
            best_str = self.id_to_token.get(best_id, "")
            accumulated += best_str

            tokenization.append(best_id)

            step_count += 1
            if step_count == MAX_DECODE_BOOL:
                raise TimeoutError(
                    f"Exceeded {MAX_DECODE_BOOL} steps "
                    f"for {self.param_name!r}; "
                    f"accumulated={accumulated!r}")
        if accumulated == "false":
            return False
        return True

    def extract_nums_from_prompt(self, prompt: str,
                                 param_type: str) -> list[str]:
        """Extract candidate numeric substrings from the prompt.

        Finds all integer/decimal substrings in `prompt` via regex, then
        excludes any whose `float` value already matches a resolved
        numeric parameter on `self.prompt_obj.parameters`. For
        `param_type` "integer", only whole-number candidates are
        returned. For "number", decimal candidates are returned when
        present; otherwise whole numbers are returned, each normalized
        to its `float`-string form and deduplicated.

        Args:
            prompt: The source prompt text to search for numeric
                substrings.
            param_type: Either "number" or "integer"; selects which
                subset of extracted numbers is returned.

        Returns:
            list[str]: Candidate numeric substrings from `prompt`,
            filtered and normalized according to `param_type`.
        """
        parameters = self.prompt_obj.parameters
        number_pattern = compile(r'-?\d+(?:\.\d+)?')
        nums = number_pattern.findall(prompt)

        used_values: Any = {}
        if parameters:
            used_values = {float(v) for v in parameters.values()
                           if isinstance(v, (int, float))}
        remaining = [n for n in nums if float(n) not in used_values]

        if param_type == "integer":
            return [n for n in remaining if "." not in n]

        decimals = [n for n in remaining if "." in n]
        whole = [n for n in remaining if "." not in n]

        if decimals and param_type == "number":
            return decimals
        return list({str(float(n)) for n in whole})

    def extract_words_from_prompt(self, prompt: str,
                                  special_str: bool = False) -> list[str]:
        """Extract candidate string values from the prompt.

        Prefers quoted substrings (single- or double-quoted) when
        present and `prompt` contains no colon, excluding any quoted
        span whose stripped content matches an already-resolved string
        parameter value. If `prompt` contains ``": "``, falls back to
        everything after the first occurrence of that separator.
        Otherwise, strips out already-resolved string parameter values
        (bare or quoted) from `prompt` and returns the remaining
        whitespace-separated tokens.

        Args:
            prompt: The source prompt text to extract string candidates
                from.

        Returns:
            list[str]: Candidate string values extracted from `prompt`.
        """
        parameters = self.prompt_obj.parameters
        used_values: Any = {}

        if parameters:
            used_values = parameters.values()

        dbl_quoted_pattern = compile(r'"[^"]*"')
        sgl_quoted_pattern = compile(r"'[^']*'")

        dbl_quoted = dbl_quoted_pattern.findall(prompt)
        sgl_quoted = sgl_quoted_pattern.findall(prompt)

        extracted_vals = [val for val in used_values if isinstance(val, str)]

        if (dbl_quoted or sgl_quoted) and ":" not in prompt:
            total = dbl_quoted + sgl_quoted

            remaining = [sub for sub in total if sub.strip("'\"")
                         not in extracted_vals]
            if special_str and remaining:
                return [max(remaining, key=len)]
            if remaining:
                return remaining
        elif ": " in prompt:
            return [prompt.split(": ")[1]]

        clean_prompt = prompt
        for extracted in extracted_vals:
            clean_prompt = clean_prompt.replace(f'"{extracted}"', "")
            clean_prompt = clean_prompt.replace(f"'{extracted}'", "")
            clean_prompt = clean_prompt.replace(extracted, "")

        if special_str:
            return [max(clean_prompt.split(), key=len)]
        return [w for w in clean_prompt.split() if w]
