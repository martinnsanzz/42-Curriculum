#!/usr/bin/env python3


import typing
import abc


class C:
    H = '\033[95m'  # Header
    B = '\033[94m'  # Blue
    C = '\033[96m'  # Cyan
    G = '\033[92m'  # Green
    W = '\033[93m'  # Warning
    F = '\033[91m'  # Fail
    E = '\033[0m'  # End
    Bo = '\033[1m'  # Bold

    def msg(self, color: str, msg: str):
        print(getattr(self, color) + msg + C.E)


class DataProcessor(abc.ABC):
    def __init__(self) -> None:
        self.data: list = []
        self.output_count = 0

    def output(self) -> tuple[int, str]:
        if not self.data:
            raise Exception("Data is empty")
        try:
            return (self.output_count, self.data[0])
        finally:
            self.output_count += 1
            self.data.pop(0)

    @abc.abstractmethod
    def validate(self, data: typing.Any) -> bool:
        pass

    @abc.abstractmethod
    def ingest(self, data: typing.Any) -> None:
        pass


class NumericProcessor(DataProcessor):
    def validate(self, data: typing.Any) -> bool:
        if isinstance(data, list):
            return all(isinstance(x, (int, float)) for x in data)
        return isinstance(data, (int, float))

    def ingest(self, data: int | float | list[int | float]) -> None:
        if not self.validate(data):
            raise TypeError("Improper numeric data")
        if not isinstance(data, list):
            self.data.append(str(data))
        else:
            self.data.extend(str(x) for x in data)


class TextProcessor(DataProcessor):
    def validate(self, data: typing.Any) -> bool:
        if isinstance(data, list):
            return all(isinstance(x, str) for x in data)
        return isinstance(data, str)

    def ingest(self, data: str | list[str]) -> None:
        if not self.validate(data):
            raise TypeError("Improper str data")
        if not isinstance(data, list):
            self.data.append(data)
        else:
            self.data.extend(x for x in data)


class LogProcessor(DataProcessor):
    def validate(self, data: typing.Any) -> bool:
        if isinstance(data, list):
            return all(
                isinstance(x, dict) and
                all(isinstance(k, str) and
                    isinstance(v, str) for k, v in x.items())
                for x in data
            )
        if isinstance(data, dict):
            return all((isinstance(k, str) and
                        isinstance(v, str)) for k, v in data.items())
        else:
            return False

    def ingest(self, data: dict[str, str] | list[dict[str, str]]) -> None:
        if not self.validate(data):
            raise TypeError("Improper str data")
        if not isinstance(data, list):
            self.data.append(': '.join(data.values()))
        else:
            for x in data:
                self.data.append(': '.join(x.values()))


def num_proc_tester() -> None:
    num_proc: NumericProcessor = NumericProcessor()

    C().msg("C", "\nTesting Numeric Processor...")
    print("Trying to validate input '42': ", end="")
    print(C.G + str(num_proc.validate(42)) + C.E)

    print("Trying to validate input 'Hello': ", end="")
    print(C.F + str(num_proc.validate("Hello")) + C.E)

    print("Test invalid ingestion of string 'foo' "
          "without prior validation:")
    try:
        num_proc.ingest("foo")
    except Exception as error:
        print(C.F + f"Got exception: {error}" + C.E)
    num_proc.ingest([1, 2, 3, 4, 5])
    print(f"Processing data: {num_proc.data}")
    print(num_proc.data)

    print(C.Bo + "Extracting 3 values..." + C.E)
    for _ in range(3):
        try:
            output = num_proc.output()
            print(f"Numeric value {output[0]}: {output[1]}")
        except Exception as error:
            print(error)


def text_proc_test() -> None:
    text_proc: TextProcessor = TextProcessor()

    C().msg("C", "\nTesting Numeric Processor...")
    print("Trying to validate input '42': ", end="")
    print(C.F + str(text_proc.validate(42)) + C.E)

    text_proc.ingest(["Hello", "Nexus", "World"])
    print(f"Processing data: {text_proc.data}")

    print(C.Bo + "Extracting 1 value..." + C.E)
    try:
        output = text_proc.output()
        print(f"Numeric value {output[0]}: {output[1]}")
    except Exception as error:
        print(error)


def log_proc_test() -> None:
    log_proc: LogProcessor = LogProcessor()

    C().msg("C", "\nTesting Numeric Processor...")
    print("Trying to validate input 'Hello': ", end="")
    print(C.F + str(log_proc.validate("Hello")) + C.E)

    print("Processing data: [{'log_level': 'NOTICE', \
'log_message': 'Connection to server'}, \
{'log_level': 'ERROR', \
'log_message': 'Unauthorized access!!'}]")
    log_proc.ingest([{'log_level': 'NOTICE',
                      'log_message': 'Connection to server'},
                    {'log_level': 'ERROR',
                     'log_message': 'Unauthorized access!!'}])
    print(C.Bo + "Extracting 2 values..." + C.E)
    for _ in range(2):
        try:
            output = log_proc.output()
            print(f"Numeric value {output[0]}: {output[1]}")
        except Exception as error:
            print(error)


if __name__ == "__main__":
    C().msg("H", "=== Code Nexus - Data Processor ===")
    num_proc_tester()
    text_proc_test()
    log_proc_test()
