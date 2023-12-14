PROJ = Gluino.Core
PROJ_TEST = dev/TestApp
PROJ_TEST_SDK = net8.0

ifeq ($(OS),Windows_NT)
# Windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OS := Linux
	else ifeq ($(UNAME_S),Darwin)
		OS := Darwin
	else
		$(error Unsupported operating system)
	endif
endif

# x64, x86, arm64
ifeq ($(arch),)
	UNAME_M := $(shell uname -m)
	ifeq ($(UNAME_M),x86_64)
		arch := x64
	else ifeq ($(UNAME_M),i686)
		arch := x86
	endif
endif
ARCH ?= $(arch)

build: build

ifeq ($(OS),Windows_NT)
### Start Windows ###
PLATFORM=$(ARCH)
ifeq ($(ARCH),x86)
	PLATFORM=Win32
endif

build: install-deps
	msbuild $(PROJ).sln /p:Configuration=Release /p:Platform=$(ARCH)
dev: install-deps
	msbuild $(PROJ).sln /p:Configuration=Debug /p:Platform=$(ARCH)
	cp $(PROJ)/bin/Debug/$(PLATFORM)/$(PROJ).dll $(PROJ_TEST)/bin/Debug/$(PROJ_TEST_SDK)/
	cp $(PROJ)/bin/Debug/$(PLATFORM)/$(PROJ).pdb $(PROJ_TEST)/bin/Debug/$(PROJ_TEST_SDK)/
	cp $(PROJ)/bin/Debug/$(PLATFORM)/WebView2Loader.dll $(PROJ_TEST)/bin/Debug/$(PROJ_TEST_SDK)/
all:
	$(MAKE) build arch=x86
	$(MAKE) build arch=x64
	$(MAKE) build arch=arm64
setup: ; @echo "No build system generation for Windows"
setup-dev: ; @echo "No build system generation for Windows"
install-deps: ; [ ! -d "packages" ] && nuget restore || :
clean:
	rm -rf $(PROJ)/bin
	rm -rf $(PROJ)/obj
	rm -rf packages
### End Windows ###
else ifeq ($(OS),Darwin)
### Start macOS ###
PLATFORM=x86_64
ifeq ($(ARCH),x64)
	PLATFORM=x86_64
endif

SETUP_CMD = cmake \
	--no-warn-unused-cli \
	-DCMAKE_OSX_ARCHITECTURES="$(PLATFORM)" \
	-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
	-DCMAKE_BUILD_TYPE:STRING=$(CONFIG) \
	-S$(shell pwd) -B$(shell pwd)/$(BUILD) -G "Unix Makefiles"

build: setup
	cd build/$(ARCH) && make all
build-universal: setup-universal
	cd build/universal && make all
dev: setup-dev
	cd build && make all
	cp build/$(PROJ).dylib $(PROJ_TEST)/bin/Debug/$(PROJ_TEST_SDK)/$(PROJ).dylib
all:
	$(MAKE) build arch=arm64
	$(MAKE) build arch=x64
setup:
	$(eval CONFIG := Release)
	$(eval BUILD := build/$(ARCH))
	@$(SETUP_CMD)
setup-dev:
	$(eval CONFIG := Debug)
	$(eval BUILD := build)
	@$(SETUP_CMD)
setup-universal:
	$(eval CONFIG := Release)
	$(eval BUILD := build/universal)
	$(eval PLATFORM := x86_64;arm64)
	@$(SETUP_CMD)
install-deps: ; @echo "No dependencies to install for macOS"
clean: ; rm -rf build
### End macOS ###
else ifeq ($(OS),Linux)
### Start Linux ###
SETUP_CMD = cmake \
	--no-warn-unused-cli \
	-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
	-DCMAKE_BUILD_TYPE:STRING=$(CONFIG) \
	-S$(shell pwd) -B$(shell pwd)/build -G "Unix Makefiles"

build: install-deps setup
	cd build && make all
dev: setup-dev
	sleep 3
	cd build && make all
	cp build/$(PROJ).so $(PROJ_TEST)/bin/Debug/$(PROJ_TEST_SDK)/$(PROJ).so
setup:
	$(eval CONFIG := Release)
	@$(SETUP_CMD)
setup-dev:
	$(eval CONFIG := Debug)
	@$(SETUP_CMD)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libgtk-3-dev libwebkit2gtk-4.0-dev libnotify4 libnotify-dev
clean: ; rm -rf build
### End Linux ###
endif
