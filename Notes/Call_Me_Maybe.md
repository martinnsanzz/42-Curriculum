# Call-Me-Maybe

## Concepts

### LLMs (Large Language Models)

A large language model (LLM) is an AI model (typically a *neural network*) trained on a vast
amount of text for natural language processing tasks, especially language generation. LLMs can
typically generate, summarize, translate, and analyze text in many contexts, and are a
foundational technology behind modern chatbots. Biased or inaccurate training data can make
an LLM's output less reliable.
[LLMs -WIKI](https://en.wikipedia.org/wiki/Large_language_model)

### Tokens
When you work with a large language model (LLM), text is first broken into units called tokens,
which are words, character sets, or combinations of words and punctuation, by a **tokenizer**.
During training, **tokenization** runs as the first step. The LLM analyzes the semantic
relationships between tokens, such as how commonly they're used together or whether they're used
in similar contexts. After training, the LLM uses those patterns and relationships to generate a
sequence of output tokens based on the input sequence.

Turn text into tokens:
    The set of unique tokens that an LLM is trained on is known as its vocabulary.

For example, consider the following sentence:

`I heard a dog bark loudly at a cat`

This text could be tokenized as:

- I
- heard
- a
- dog
- bark
- loudly
- at
- a
- cat

By having a sufficiently large set of training text, tokenization can compile a vocabulary of
many thousands of tokens.

#### Common tokenization methods

The specific tokenization method varies by LLM. Common tokenization methods include:

- **Word tokenization** (text is split into individual words based on a delimiter)
- **Character tokenization** (text is split into individual characters)
- **Subword tokenization** (text is split into partial words or character sets)

[Understand Tokens](https://learn.microsoft.com/en-us/dotnet/ai/conceptual/understanding-tokens)

### Function Calling
Function calling is the ability to reliably connect LLMs to external tools to enable
effective tool usage and interaction with external APIs.

LLMs like GPT-4 and GPT-3.5 have been fine-tuned to detect when a function needs to be called
and then output *JSON* containing arguments to call the function. The functions that are being
called by function calling will act as tools in your AI application and you can define more than
one in a single request.

Function calling is an important ability for building LLM-powered chatbots or agents that need
to retrieve context for an LLM or interact with external tools by converting natural language
into API calls.

- **For example**, the query `"What is the weather like in Belize?"` will be converted to a function call such as `get_current_weather(location: string, unit: 'celsius' | 'fahrenheit')`

[Function calling with LLMs](https://www.promptingguide.ai/applications/function_calling)

### Constrained Decoding
In the context of structured generation, constrained decoding is a technique that manipulates a
generative model's token generation process to constrain its next-token predictions to only
tokens that do not violate the required output structure.

State of the art constrained decoding skips the parts of the structured output that are
boilerplate scaffolding or tokens that can be uniquely determined based on preceding tokens and
the constraints of the desired output. Only the parts of the output that strictly require
generation are sampled from a restricted set of compatible tokens in the model's next-token
probability distribution.

Constrained decoding works by setting “constraints” — specific phrases or structures that the
model must include in its output within a certain number of tokens. This approach offers several
benefits:

- Structured Output: Ensures generated text follows a predetermined format.
- Guided Reasoning: Encourages step-by-step thought processes.
- Consistent Responses: Helps maintain uniformity across multiple generations.
- Error Reduction: Minimizes off-topic or irrelevant content.

Common applications include:

- Generating code with specific elements
- Creating responses in particular formats (e.g., JSON, XML)
- Producing step-by-step reasoning for problem-solving
- Answering multiple-choice questions with explanations

[Constrained Decoding](https://www.aidancooper.co.uk/constrained-decoding/)
[Implementing Constrained Decoding](https://medium.com/@albersj66/part-6-implementing-constrained-decoding-for-phi-3-vision-2c72a1be6a17)