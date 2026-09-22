#!/usr/bin/env python3


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


C().msg("W", "LOADING STATUS: Loading programs...")


TOTAL_ERRORS = 0

import sys  # noqa
from typing import Any  # noqa


np: Any = None
pd: Any = None
plt: Any = None
requests: Any = None


try:
    import matplotlib as plt  # type: ignore
except (ModuleNotFoundError, ImportError) as error:
    TOTAL_ERRORS += 1
    C().msg("F", f"     -{str(error)}")
    plt = None

try:
    import pandas as pd  # type: ignore
except (ModuleNotFoundError, ImportError) as error:
    TOTAL_ERRORS += 1
    C().msg("F", f"     -{str(error)}")

try:
    import numpy as np  # type: ignore
except (ModuleNotFoundError, ImportError) as error:
    TOTAL_ERRORS += 1
    C().msg("F", f"     -{str(error)}")

try:
    import requests  # type: ignore
except (ModuleNotFoundError, ImportError) as error:
    TOTAL_ERRORS += 1
    C().msg("F", f"     -{str(error)}")


def install_instructions() -> None:
    C().msg("Bo", "\nInstallation instructions: ")

    print("With pip -> 'pip -m install <package_name>'")
    print("With poetry -> 'poetry add <package_name>@latest'\n")


def dependencies_info() -> None:
    C().msg("H", "\nChecking dependencies: ")
    if pd:
        print(f" [OK] pandas ({pd.__version__}): Data \
manipulation ready")
    if np:
        print(f" [OK] numpy ({np.__version__}): Numerical \
computation ready")
    if requests:
        print(f" [OK] request ({requests.__version__}): Network \
access ready")
    if plt:
        print(f" [OK] matplotlib ({plt.__version__}): Visualization\
ready")


def get_data() -> dict[str, list]:
    url = "https://api.coingecko.com/api/v3/coins/bitcoin/market_chart"
    params = {"vs_currency": "usd",
              "days": "30"}
    response = requests.get(url, params=params)
    data = response.json()
    return data['prices']


if __name__ == "__main__":
    if TOTAL_ERRORS != 0:
        install_instructions()
    if TOTAL_ERRORS != 4:
        dependencies_info()

    if TOTAL_ERRORS == 0:
        C().msg("H", "\n=== Testing dependencies ===")
        C().msg("Bo", "This uses numpy to generate an Matrix array")
        array = np.random.randint(1, 100, size=[5, 5])

        print(f"Array: \n{array}")
        print(f"Shape of array: {array.shape}")
        print(f"Data type of array: {array.dtype}")
        print(f"Size of array (n-elements): {array.size}")
        print(f"Number of dimensions: {array.ndim}")

        try:
            C().msg("Bo", "\nThis uses panda to manipulate data")
            df = pd.read_csv('la_liga.csv')
            C().msg("Bo", "\nData frame: ")
            print(df)
            C().msg("Bo", "\nData frame head (first 3): ")
            print(df.head(3))
            C().msg("Bo", "\nData frame tail (last 3): ")
            print(df.tail(3))

        except FileNotFoundError:
            C().msg("F", "File 'la_liga.csv' does not exist")

        C().msg("Bo", "\nUsing requests to get data of crypto prices \
    over the last 30 days")
        C().msg("Bo", "\nCreating crypto_analysis.png")
        prices = get_data()
        df = pd.DataFrame(prices, columns=['Timestamp', 'Price'])

        df['moving_avg'] = df['Price'].rolling(window=7).mean()

        import matplotlib.pyplot as plt  # type: ignore # noqa
        plt.plot(df['Price'], label='BTC Price')
        plt.plot(df['moving_avg'], label='7-day MA')

        plt.xlabel('Data Points (30 days, ~hourly)')
        plt.ylabel('Price (USD)')
        plt.legend()
        plt.savefig('crypto_analysis.png')
