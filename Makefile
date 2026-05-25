UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(UNAME_S),Linux)
    OS := linux
    RM := rm -f
    MKDIR := mkdir -p
    INSTALL := install -Dm755
    BINARY_PATH := build/dlib
    BINARY_NAME := dlib
else ifeq ($(UNAME_S),Windows_NT)
    OS := windows
    RM := del /Q
    MKDIR := mkdir
    INSTALL := copy
    BINARY_PATH := build/Release/dlib.exe
    BINARY_NAME := dlib.exe
else
    $(error OS not supported)
endif

PREFIX := $(HOME)/.local
BINARY := $(BINARY_PATH)

.PHONY: all build configure install uninstall clean

all: build

configure:
	cmake -B build -DCMAKE_BUILD_TYPE=Release

build: configure
	cmake --build build --config Release

install: build
ifeq ($(OS),linux)
	@echo "Installing dlib..."
	$(INSTALL) $(BINARY) $(PREFIX)/bin/$(BINARY_NAME)
	@echo "Done. Restart your shell or run: source ~/.bashrc"
else ifeq ($(OS),windows)
	@echo "Copying dlib.exe to $(PREFIX)\\bin\\"
	if not exist "$(PREFIX)\\bin" $(MKDIR) "$(PREFIX)\\bin"
	copy $(BINARY) "$(PREFIX)\\bin\\$(BINARY_NAME)"
	@echo "Done. Add $(PREFIX)\\bin to your PATH if needed."
endif

uninstall:
ifeq ($(OS),linux)
	$(RM) $(PREFIX)/bin/$(BINARY_NAME)
else ifeq ($(OS),windows)
	$(RM) "$(PREFIX)\\bin\\$(BINARY_NAME)"
endif
	@echo "Uninstalled."

clean:
	rm -rf build

