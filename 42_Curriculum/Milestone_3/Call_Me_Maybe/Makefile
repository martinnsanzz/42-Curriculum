.ONESHELL:

.DEFAULT_GOAL := help

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

# ------------------ Paths  ------------------ #
FUNC_DEF			:= data/input/functions_definition.json
MOULINETTE_FUNC_DEF	:= moulinette/data/input/functions_definition.json

INPUT				:= data/input/function_calling_tests.json
MOULINETTE_INPUT	:= moulinette/data/input/function_calling_tests.json

OUTPUT_DIR			:= data/output
OUTPUT				:= data/output/function_calling_results.json
MOULINETTE_OUTPUT	:= ../data/output/function_calling_results.json

TEST_DIR			:= data/test
TEST				:= data/test/prompt_test.json


# ------------------ Tools  ------------------ #
VENV				:= .venv
UV					:= ${VENV}/bin/uv
RUN_COMM			:= ${UV} run python
DEBUGGER			:= pdb

MYPY_FLAGS			:= . --warn-return-any --warn-unused-ignores \
						--ignore-missing-imports --disallow-untyped-defs \
						--check-untyped-defs --explicit-package-bases

MYPY_STRICT_FLAGS	:= . --strict --explicit-package-bases

TEST_ARG			:= --functions_definition ${FUNC_DEF} \
					   --test ${TEST}
STD_ARGS			:= --functions_definition ${FUNC_DEF} \
					   --input ${INPUT} \
					   --output ${OUTPUT}
MOULINETTE_STD_ARGS	:= --functions_definition ${MOULINETTE_FUNC_DEF} \
					   --input ${MOULINETTE_INPUT} \
					   --output ${OUTPUT} \
					   --visual_mode True
VISUAL_ARGS			:= --functions_definition ${FUNC_DEF} \
					   --input ${INPUT} \
					   --output ${OUTPUT} \
					   --visual_mode True

$(VENV):
	@echo "${YELLOW}Creating virtual environment...${RESET}"
	@uv venv  --prompt Call_Me_Maybe > /dev/null 2>&1

	@echo "${CYAN}	RUN 'source ./env/bin/activate \
	('activate.fish' if using fish)${RESET}"

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)
	@touch $(OUTPUT)

$(TEST_DIR):
	@mkdir -p $(TEST_DIR)
	@touch $(TEST)

# ------------------ Commands ------------------ #
help:
	@echo "${BOLD}Available commands:${RESET}"
	@echo "${CYAN}  make install${RESET}     - Create venv and install dependencies with uv"
	@echo "${CYAN}  make run${RESET}        - Run the program"
	@echo "${CYAN}  make test${RESET}        - Run the program with test .json file path"
	@echo "${CYAN}  make debug${RESET}      - Run with debugger (pdb by default)"
	@echo "${CYAN}  make visual${RESET}      - Run the program showing each step"
	@echo "${CYAN}  make moulinette${RESET}      - Run the program with moulinette + visual"
	@echo "${CYAN}  make lint${RESET}       - Run flake8 and mypy"
	@echo "${CYAN}  make lint-strict${RESET} - Run flake8 and mypy --strict"
	@echo "${CYAN}  make clean${RESET}      - Remove cache folders"
	@echo "${CYAN}  make fclean${RESET}     - Remove venv and cache"

install: $(VENV)
	@echo "${YELLOW}Installing dependencies...${RESET}"
	@uv sync --link-mode=copy 2>/dev/null

	@echo "${GREEN}\nEverything installed ${RESET}"

run: install $(OUTPUT_DIR)
	@echo "${GREEN}\nRunning... !!${RESET}"
	@ ${RUN_COMM} -m src ${STD_ARGS}

debug: $(VENV) install
	@echo "${PURPLE}Running in debug mode with $(DEBUGGER)...${RESET}"
	@$(RUN_COMM) -m $(DEBUGGER) -m src $(TEST_ARG)

test: install $(TEST_DIR) $(OUTPUT_DIR)
	@echo "${GREEN}\nRunning with test file... !!${RESET}"
	@ ${RUN_COMM} -m src ${TEST_ARG} --visual_mode True

visual: install $(OUTPUT_DIR)
	@echo "${GREEN}\nRunning in visual mode... !!${RESET}"
	@ ${RUN_COMM} -m src ${VISUAL_ARGS}

moulinette: $(OUTPUT_DIR)
	@echo "${GREEN}\nCreating moulinette test files... !!${RESET}"
	@(cd moulinette && uv run python -m moulinette prepare_exercises --set private)

	@echo "${GREEN}\nRunning Call_Me_Maybe with moulinette test files... !!${RESET}"
	@$(RUN_COMM) -m src $(MOULINETTE_STD_ARGS)

	@echo "${GREEN}\nChecking answers with moulinette... !!${RESET}"
	@(cd moulinette && uv run python -m moulinette grade_student_answers \
		$(MOULINETTE_OUTPUT) --set private)

clean:
	@echo "${RED}Removing '.mypy_cache' folders.${RESET}"
	@find . -type d -name .mypy_cache -exec rm -rf {} +

	@echo "${RED}Removing '__pycache__' folders.${RESET}"
	@find . -type d -name __pycache__ -exec rm -rf {} +

	@echo "${RED}Removing '.pyc' files.${RESET}"
	@find . -type f -name "*.pyc" -exec rm -rf {} +

	@echo "${RED}Removing test and output folders.${RESET}"
	@rm -rf $(OUTPUT_DIR)
	@rm -rf $(TEST_DIR)

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

.PHONY: install wheel run test visual moulinette debug clean fclean lint lint-strict help