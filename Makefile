BUILD_DIR = build

CMAKE = cmake
CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

all: build compile_commands.json unittests

compile_commands.json:
	ln -s $(BUILD_DIR)/$@ $@

build:
	$(CMAKE) $(CMAKE_FLAGS) -B $(BUILD_DIR)
	$(CMAKE) --build $(BUILD_DIR)

build-tests:
	$(CMAKE) $(CMAKE_FLAGS) -DBUILD_TESTS=ON -B $(BUILD_DIR)
	$(CMAKE) --build $(BUILD_DIR)

unittests: build-tests
	cd $(BUILD_DIR) && ctest --output-on-failure

clean:
	rm -rf $(BUILD_DIR) compile_commands.json

.PHONY: all build clean build-tests unittests
.DEFAULT_GOAL := all
