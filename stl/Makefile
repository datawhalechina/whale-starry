SOURCES := $(shell find . -type f \( -name '*.cc' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \))

all: format

format:
	@for src in $(SOURCES) ; do \
		echo "Formatting $$src" ; \
		clang-format -i "$$src" ; \
	done
	@echo "Done"
