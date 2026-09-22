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
    U = '\033[4m'

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
    name = "Numeric Processor"

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
    name = "Text Processor"

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
    name = "Log Processor"

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


class DataStream():
    def __init__(self) -> None:
        self.proc_lst: list = []

    def register_processor(self, proc: DataProcessor) -> None:
        self.proc_lst.append(proc)

    def process_stream(self, stream: list[typing.Any]) -> None:
        for data in stream:
            data_processed = False
            for proc in self.proc_lst:
                if proc.validate(data):
                    data_processed = True
                    proc.ingest(data)
                    break
            if not data_processed:
                print(C.F + f"DataStream error - Can't process \
element in stream: {data}" + C.E)

    def print_processors_stats(self) -> None:
        print(C.C + "\n== DataStream statistics ==" + C.E)
        if not self.proc_lst:
            print(C.W + "No processor found, no data" + C.E)
        else:
            for proc in self.proc_lst:
                total_data = len(proc.data) + proc.output_count
                print(C.G + f"{proc.name}", end="" + C.E)
                print(f": total {total_data} items processed, ",
                      end="")
                print(f"remaining {len(proc.data)} on processor")

    def output_pipeline(self, nb: int, plugin: 'ExportPlugin') -> None:
        for proc in self.proc_lst:
            data_lst: list = []
            for x in range(nb):
                try:
                    data_lst.append(proc.output())
                except Exception:
                    pass
            if data_lst:
                plugin.process_output(data_lst)


class ExportPlugin(typing.Protocol):
    def process_output(self, data: list[tuple[int, str]]) -> None:
        pass


class CSV():
    def __init__(self) -> None:
        self.data_holder: list = []

    def process_output(self, data: list[tuple[int, str]]) -> None:
        result: str = ",".join(tup[1] for tup in data)
        print(C.C + "CSV output:" + C.E)
        print(result)


class JSON():
    def process_output(self, data: list[tuple[int, str]]) -> None:
        tmp_lst: list = []
        for x in data:
            tmp_lst.append((f'"item_{x[0]}": "{x[1]}"'))
        result = ", ".join(tmp_lst)
        print(C.C + "JSON output:" + C.E)
        print(f"{{{result}}}")


if __name__ == "__main__":
    num_proc: NumericProcessor = NumericProcessor()
    text_proc: TextProcessor = TextProcessor()
    log_proc: LogProcessor = LogProcessor()
    csv_plugin: ExportPlugin = CSV()
    json_plugin: ExportPlugin = JSON()

    print(C.H + "=== Code Nexus - Data Stream ===" + C.E)
    print("Initialize Data Stream...")
    data_stream: DataStream = DataStream()
    data_stream.print_processors_stats()

    print(C.U + "\nRegistering Processors:" + C.E)
    data_stream.register_processor(num_proc)
    data_stream.register_processor(text_proc)
    data_stream.register_processor(log_proc)

    print("Send first batch of data on stream: ['Hello world', \
[3.14, -1, 2.71], [{'log_level': 'WARNING', \
'log_message': 'Telnet access! Use ssh instead'}, \
{'log_level': 'INFO', 'log_message': 'User wil is \
connected'}], 42, ['Hi', 'five']]\n")
    data_stream.process_stream(['Hello world',
                                [3.14, -1, 2.71],
                                [{'log_level': 'WARNING',
                                  'log_message': 'Telnet access! \
Use ssh instead'},
                                    {'log_level': 'INFO', 'log_message':
                                     'User wil is connected'}],
                                42,
                                ['Hi', 'five']])

    data_stream.print_processors_stats()
    print(C.Bo + "\nSend 3 processed data from each \
processor to a CSV plugin:" + C.E)
    data_stream.output_pipeline(3, csv_plugin)
    data_stream.print_processors_stats()

    print("\nSend another batch of data: [21, ['I love AI', \
'LLMs are wonderful', 'Stay healthy'], [{'log_level': \
'ERROR', 'log_message': '500 server crash'}, {'log_level': \
'NOTICE', 'log_message': 'Certificate expires in 10 days'}], \
[32, 42, 64, 84, 128, 168], 'World hello'")
    data_stream.process_stream([21, ['I love AI',
                                     'LLMs are wonderful',
                                     'Stay healthy'],
                                [{'log_level':
                                  'ERROR', 'log_message':
                                  '500 server crash'},
                               {'log_level':
                                 'NOTICE', 'log_message':
                                 'Certificate expires \
in 10 days'}],
                               [32, 42, 64, 84, 128, 168],
                               'World hello'])
    data_stream.print_processors_stats()
    print(C.Bo + "\nSend 5 processed data from each \
processor to a JSON plugin:" + C.E)
    data_stream.output_pipeline(5, json_plugin)
    data_stream.print_processors_stats()
