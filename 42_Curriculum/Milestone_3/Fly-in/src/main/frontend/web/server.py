# Built-in modules
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from functools import partial

# Local modules
from ..terminal import TerminalPrinter


class VisualizerServer:
    """Serve the static visualizer files over HTTP.

    The server binds to the first free port starting at `start_port`,
    prints how to open the visualizer, and blocks until the user stops it
    with Ctrl+C.

    Attributes:
        static_dir: Directory served as the web root.
        host: Host name or address the server binds to.
        start_port: First port to try.
        port: Port the server is bound to, or None before `run()` has
            found a free one.
    """
    STATIC_DIR = Path(__file__).resolve().parent / "static"
    HOST = "localhost"  # or "127.0.0.1"
    DEFAULT_PORT = 8000
    MAX_PORT = 65535

    INSTRUCTION = "Visualizer running at -> http://{host}:{port}"
    IN_VSCODE = "Open the link in your browser " \
                "(Cmd+click it in VS Code's terminal)."
    TURN_OFF = "Press Ctrl+C to stop the server."

    def __init__(self, static_dir: Path = STATIC_DIR, host: str = HOST,
                 start_port: int = DEFAULT_PORT) -> None:
        """Store the server configuration. Nothing is started yet.

        Args:
            static_dir: Directory to serve. Defaults to the `static`
                folder next to this file.
            host: Host name or address to bind to.
            start_port: First port to try.
        """
        self.static_dir = static_dir
        self.host = host
        self.start_port = start_port
        self.port: int | None = None

    def _bind(self) -> ThreadingHTTPServer:
        """Create the HTTP server on the first free port.

        Starting at `start_port`, try each port in turn and skip the ones
        that are already in use.

        Returns:
            A bound `ThreadingHTTPServer`. The caller must close it.

        Raises:
            OSError: If no port up to `MAX_PORT` is free.
        """
        handler = partial(SimpleHTTPRequestHandler,
                          directory=str(self.static_dir))

        for port in range(self.start_port, self.MAX_PORT + 1):
            try:
                httpd = ThreadingHTTPServer((self.host, port), handler)
            except OSError:
                TerminalPrinter.print_msg(msg=f"Port {port} is busy, "
                                          f"trying {port + 1}...\n")
                continue
            self.port = port
            return httpd
        raise OSError(f"No free port between {self.start_port} "
                      f"and {self.MAX_PORT}")

    def _print_instructions(self) -> None:
        """Print how to open the visualizer and how to stop the server."""
        TerminalPrinter.print_msg(msg="Server successfully started",
                                  color="green")
        TerminalPrinter.print_msg(msg="\n" + self.INSTRUCTION.format(
            host=self.host, port=self.port))
        TerminalPrinter.print_msg(msg=self.IN_VSCODE)
        TerminalPrinter.print_msg(msg=f"{self.TURN_OFF}\n")

    def run(self) -> None:
        """Serve the static directory until interrupted.

        Bind to a free port, print the access instructions, then block
        serving requests. The server never returns on its own: it only
        stops through `KeyboardInterrupt`, which is handled here and not
        propagated.

        Raises:
            OSError: If no free port could be found.
        """
        with self._bind() as httpd:
            self._print_instructions()
            try:
                httpd.serve_forever()
            except KeyboardInterrupt:
                TerminalPrinter.print_msg(msg="\nServer shutting down.",
                                          color="red")
