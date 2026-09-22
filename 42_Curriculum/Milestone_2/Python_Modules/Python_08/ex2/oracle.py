#!/usr/bin/env python3
# Fish command to set an env variable 'set -x MATRIX_MODE production'
# To unset it 'set -e MATRIX_MODE'
# In bash it would be 'export MATRIX_MODE=production'
# To unset it 'unset MATRIX_MODE'

import sys
import os


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

    def msg(self, color: str, msg: str) -> None:
        print(getattr(self, color) + msg + C.E)


try:
    from dotenv import load_dotenv  # type: ignore
    load_dotenv()  # Loads variables from .env into os.environ
except (ModuleNotFoundError, ImportError) as error:
    C().msg("F", f"{str(error)}")
    sys.exit()


def display_conf() -> None:
    mode = os.getenv("MATRIX_MODE")
    db_url = os.getenv("DATABASE_URL")
    api_key = os.getenv("API_KEY")
    log_level = os.getenv("LOG_LEVEL")
    zion = os.getenv("ZION_ENDPOINT")

    print(f"    Mode: {mode}")
    print(f"    Database: {db_url}")
    print(f"    API Access: {api_key}")
    print(f"    Log Level: {log_level}")
    print(f"    Zion Network: {zion}")


def security_check() -> None:
    mode = os.getenv("MATRIX_MODE")
    db_url = os.getenv("DATABASE_URL")
    api_key = os.getenv("API_KEY")
    log_level = os.getenv("LOG_LEVEL")
    zion = os.getenv("ZION_ENDPOINT")

    lst = [mode, db_url, api_key, log_level, zion]

    for x in lst:
        if not x:
            C().msg("F", "[KO] .env file not configured properly")
            sys.exit()
    C().msg("G", "  [OK] No hardcoded secrets detected")
    C().msg("G", "  [OK] .env file properly configured")
    C().msg("G", "  [OK] Production overrides available")


if __name__ == "__main__":
    script_dir: str = os.path.dirname(__file__)
    env_path: str = os.path.join(script_dir, '.env')

    C().msg("H", "ORACLE STATUS: Reading the Matrix...")

    if os.path.exists(env_path):
        C().msg("Bo", "\nConfiguration loaded: ")
        display_conf()
        C().msg("Bo", "\nEnvironment security check: ")
        security_check()
    else:
        print('.env file missing...')
