CC = gcc
CLANG_FORMAT = clang-format

PROJECTS = $(shell find . -mindepth 2 -name Makefile -printf '%h\n' | sort)

.PHONY: format clean clean-projects clean-data

format:
	@find . -type d \( -name .git -o -name bin -o -name obj \) -prune -o \
		-type f \( -name '*.c' -o -name '*.h' \) -exec $(CLANG_FORMAT) --style=file -i {} +

clean: clean-projects clean-data

clean-projects:
	@set -e; for project in $(PROJECTS); do \
		echo "Cleaning $$project"; \
		$(MAKE) -C "$$project" clean; \
	done

clean-data:
	@find . -type f -name '*.dat' -print -delete
