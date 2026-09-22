.ONESHELL:

.DEFAULT_GOAL := install

#COLORS
GREEN = \033[1;92m
RED = \033[1;91m
YELLOW = \033[1;93m
RESET = \033[0m
PURPLE = \033[95m
BLUE = \033[94m
CYAN = \033[96m
BOLD = \033[1m
UNDERLINE = \033[4m

PROG = a_maze_ing.py
CONFIG = config.txt

VENV = env
PYTHON = $(VENV)/bin/python
PIP = $(VENV)/bin/pip

DEBUGGER = pdb

$(VENV):
	@echo "${YELLOW}Creating virtual environment...${RESET}"
	@python3 -m venv $(VENV)

	@echo "${CYAN}	RUN 'source ./env/bin/activate \
	('activate.fish' if using fish)${RESET}"

install: $(VENV)
	@echo "${YELLOW}Installing dependencies...${RESET}"
	@$(PIP) install -q -r requirements.txt > /dev/null 2>&1

	@echo "${GREEN}Everything installed ${RESET}"

wheel:
	@echo "${YELLOW}Creating package wheel !!${RESET}"
	@$(PYTHON) -m build > /dev/null 2>&1
	@cp dist/*.whl .
	@rm -rf dist && rm -rf src/mazegen.egg-info
	@echo "${GREEN}\nSucesfully build mazegen .whl file !!${RESET}"

run: install
	@echo "${GREEN}\nRunning $(PROG) !!${RESET}"
	@$(PYTHON) $(PROG) $(CONFIG)

debug: $(VENV) install
	@echo "${PURPLE}Running $(PROG) in debug mode with $(DEBUGGER)...${RESET}"
	@$(PYTHON) -m $(DEBUGGER) $(PROG) $(CONFIG) 2>/dev/null

clean:
	@echo "${RED}Removing '.mypy_cache' folders.${RESET}"
	@find . -type d -name .mypy_cache -exec rm -rf {} +

	@echo "${RED}Removing '__pycache__' folders.${RESET}"
	@find . -type d -name __pycache__ -exec rm -rf {} +

	@echo "${RED}Removing '.pyc' files.${RESET}"
	@find . -type d -name *.pyc -exec rm -rf {} +

	@echo "${RED}Removing 'output_maze.txt' files.${RESET}"
	@find . -type f -name output_maze.txt -exec rm -rf {} +

fclean: clean
	@echo "${RED}Removing 'env/' from root.${RESET}"
	@rm -rf $(VENV)


	@echo "${YELLOW}\nMAKE SURE YOU EXIT VENV 'deactivate'.${RESET}"

lint:
	@echo "${YELLOW}Running flake8...${RESET}"
	-@flake8 . --exclude=env && echo "${GREEN}✓ flake8 passed${RESET}" || true

	@echo "${YELLOW}\nRunning mypy...${RESET}"
	-@mypy . --warn-return-any --warn-unused-ignores \
	--ignore-missing-imports --disallow-untyped-defs \
	--check-untyped-defs --explicit-package-bases && \
	echo "${GREEN}✓ mypy passed${RESET}" || true

lint-strict:
	@echo "${YELLOW}Running flake8...${RESET}"
	@flake8 . --exclude=env && echo "${GREEN}✓ flake8 passed${RESET}" || true

	@echo "${YELLOW}Running mypy --strict...${RESET}"
	@mypy . --strict --explicit-package-bases && \
	echo "${GREEN}✓ mypy --strict passed${RESET}"|| true

help:
	@echo "${BOLD}Available targets:${RESET}"
	@echo "${CYAN}  make install${RESET}     - Create venv and install dependencies"
	@echo "${CYAN}  make run${RESET}        - Run the program"
	@echo "${CYAN}  make debug${RESET}      - Run with debugger (pdb by default)"
	@echo "${CYAN}  make lint${RESET}       - Run flake8 and mypy"
	@echo "${CYAN}  make lint-strict${RESET} - Run flake8 and mypy --strict"
	@echo "${CYAN}  make clean${RESET}      - Remove cache folders"
	@echo "${CYAN}  make fclean${RESET}     - Remove venv and cache"
	@echo "${CYAN}  make wheel${RESET}     - Builds .whl from src/mazegen package"

.PHONY: install wheel run debug clean wheel fclean lint lint-strict help