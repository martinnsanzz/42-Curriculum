# Built-in modules
from typing import Any, Optional, TypeAlias
from json import dumps

# Installed modules
from pydantic import BaseModel, Field

FunctionLookup: TypeAlias = dict[str, dict[str, Any]]


class Prompt(BaseModel):
    """Represent a single user prompt and its function-calling state.

    Holds the raw prompt text plus the (optionally) selected function
    name and extracted parameters, and builds the chat-formatted
    system/user prompts sent to the LLM at each stage.

    Attributes:
        prompt (str): The raw user prompt text.
        name (str): Name of the function selected for this
            prompt.
        parameters (dict[str, Any]): Extracted parameter
            values for the selected function.
    """
    prompt: str = Field(default="")
    name: str = Field(default="")
    parameters: dict[str, Any] = Field(default={})

    def sys_prompt(self, state: str,
                   function_names: Optional[str] = "",
                   param_spec: Optional[str] = None,
                   param_position: int = 0) -> str:
        """Build a chat-formatted prompt for the current pipeline stage."""
        ordinal = {1: "FIRST", 2: "SECOND", 3: "THIRD"}.get(
            param_position, f"{param_position}th"
            )

        extracted_val: Any = {}

        if self.parameters:
            extracted_val = [param for param in self.parameters.values()]

        system_content_func = (
            "You are a function-selection system. Given a "
            "user request, respond with only the name of the single "
            "most suitable function from the list below. Do not include "
            "any explanation, punctuation, or extra text — output only "
            "the function name exactly as written.\n\n"
            "Available functions:\n"
            f"{function_names}"
            "Use 'None' if you consider the prompt doesnt match "
            "any available functions"
        )
        system_content_param = (
            "You are a parameter-extraction system. Do not include any "
            "explanation, reasoning, or extra text. Output only the parameter"
            "value.\n"
            f"Extract the {ordinal} relevant value in the user request "
            f"for this parameter '{param_spec}', matching the required "
            "type exactly.\n\n"
            "Already extracted values from prompt: "
            f"{extracted_val if self.parameters else "None. "}"
            f"{f"\nDO NOT EXTRACT AGAIN {extracted_val}" if extracted_val
               else ""}"
            f"\nParameter required - {param_spec} = \n"
        )
        system_content = system_content_func if state == "func" \
            else system_content_param
        return (
            f"<|im_start|>system\n{system_content}<|im_end|>\n"
            f"<|im_start|>user\n{self.prompt}<|im_end|>\n"
            "<|im_start|>assistant"
        )

    def get_output(self) -> str:
        """Serialize the prompt's result fields to a JSON object string.

        Manually builds a JSON string containing ``prompt``, ``name``,
        and ``parameters``, each individually JSON-encoded via
        ``dumps``.
        """
        prompt_str = f'"prompt": {dumps(self.prompt)}, '
        func_str = f'"name": {dumps(self.name)}, '
        param_str = f'"parameters": {dumps(self.parameters)}'

        return "{" + prompt_str + func_str + param_str + "}"
