class CustomError(Exception):
    """Raised for invalid or unexpected input encountered during processing."""
    pass


class GenerationTimeoutError(Exception):
    """Raised when constrained decoding fails to converge within a
    step budget."""
    pass
