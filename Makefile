PREFIX := $(HOME)/.local
BINARY := build/dlib

.PHONY: all build configure install uninstall clean

all: build

configure:
	cmake -B build

build: configure
	cmake --build build

install: build
	@echo "Installing dlib..."
	install -Dm755 $(BINARY) $(PREFIX)/bin/dlib
	@echo "Done. Restart your shell or run: source ~/.bashrc"

uninstall:
	@echo "Uninstalling dlib..."
	rm -f $(PREFIX)/bin/dlib
	@echo "Done."

clean:
	rm -rf build