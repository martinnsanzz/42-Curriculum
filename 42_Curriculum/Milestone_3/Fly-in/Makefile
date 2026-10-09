.ONESHELL:

.DEFAULT_GOAL		:= run

# ------------------ Colors ------------------ #
GREEN				:= \033[1;92m
RED					:= \033[1;91m
YELLOW				:= \033[1;93m
RESET				:= \033[0m
PURPLE				:= \033[95m
BLUE				:= \033[94m
CYAN				:= \033[96m
BOLD				:= \033[1m
UNDERLINE			:= \033[4m

# ------------------ Tools  ------------------ #
VENV				:= .venv
UV					:= uv
RUN_COMM			:= ${UV} run python
DEBUGGER			:= pdb

# ------------------ Paths  ------------------ #
MAP_PATH			:= maps/easy/01_linear_path.txt

# ------------------ FLAGS  ------------------ #
MAP_FLAG			:= --map ${MAP_PATH}

# ------------------ Commands ------------------ #
help:
	@echo "${BOLD}Available commands:${RESET}"

check_uv:
	@command -v ${UV} >/dev/null 2>&1 || { \
		echo "${RED}uv not found. Install it: https://docs.astral.sh/uv/${RESET}"; \
		exit 1; \
	}

install: check_uv
	@if ! ${UV} sync --check >/dev/null 2>&1; then \
		echo "${YELLOW}Installing dependencies...${RESET}"; \
		${UV} sync >/dev/null 2>&1 || exit 1; \
	fi
	@echo "${GREEN}Everything installed${RESET}"

run: install
	@echo "${GREEN}\nRunning program... !!\n${RESET}"
	@${RUN_COMM} -m fly_in ${MAP_FLAG}

debug:

clean:
	@echo "${RED}Removing '.mypy_cache' folders.${RESET}"
	@find . -type d -name .mypy_cache -exec rm -rf {} +

	@echo "${RED}Removing '__pycache__' folders.${RESET}"
	@find . -type d -name __pycache__ -exec rm -rf {} +

	@echo "${RED}Removing '.pytest_cache' folders.${RESET}"
	@find . -type d -name .pytest_cache -exec rm -rf {} +

	@echo "${RED}Removing '.pyc' files.${RESET}"
	@find . -type f -name "*.pyc" -exec rm -rf {} +

fclean: clean
	@echo "${RED}Removing '.venv'.${RESET}"
	@rm -rf $(VENV)


	@echo "${YELLOW}\nMAKE SURE YOU EXIT VENV 'deactivate'.${RESET}"

lint:
	@echo "${YELLOW}Running flake8...${RESET}"
	-@flake8 . && echo "${GREEN}✓ flake8 passed${RESET}" || true

	@echo "${YELLOW}\nRunning mypy...${RESET}"
	-@mypy ${MYPY_FLAGS} && echo "${GREEN}✓ mypy passed${RESET}" || true

lint-strict:
	@echo "${YELLOW}Running flake8...${RESET}"
	@flake8 . && echo "${GREEN}✓ flake8 passed${RESET}" || true

	@echo "${YELLOW}Running mypy --strict...${RESET}"
	@mypy ${MYPY_STRICT_FLAGS} && echo "${GREEN}✓ mypy --strict passed${RESET}"|| true

.PHONY: help, check_uv, install, run, debug, clean, fclean, lint, lint-strict