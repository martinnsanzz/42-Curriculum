# Built-in modules
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from functools import partial
from typing import Optional

# Local modules
from ..terminal import TerminalPrinter

STATIC_DIR = Path("src/main/frontend/web/static")
HOST = "localhost" # or "127.0.0.1"
DEFAULT_PORT = 8000

SERVER_INSTRUCTION = "Visualizer running at -> http://localhost:"
SERVER_IN_VSCODE = "Open the link in your browser " \
                   "(Cmd+click it in VS Code's terminal)."
TURN_OFF_SERVER = "Press Ctrl+C to stop the server."


def run_http_server() -> None:
    handler = partial(SimpleHTTPRequestHandler, directory=str(STATIC_DIR))
    port = DEFAULT_PORT
    httpd: Optional[ThreadingHTTPServer] = None

    while httpd is None:
        try:
            httpd = ThreadingHTTPServer((HOST, port), handler)
        except OSError:
            TerminalPrinter.print_msg(msg=f"Port {port} is busy,"
                                      f"trying {port + 1}...\n")
            port += 1

    TerminalPrinter.print_msg(msg="Server successfully started",
                              color="green")

    TerminalPrinter.print_msg(msg=f"\n{SERVER_INSTRUCTION}{port}")
    TerminalPrinter.print_msg(msg=f"{SERVER_IN_VSCODE}{port}")
    TerminalPrinter.print_msg(msg=f"{TURN_OFF_SERVER}\n")

    with httpd:
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            TerminalPrinter.print_msg(msg="\nServer shutting down.", color="red")
