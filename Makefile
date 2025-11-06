BUILD_DIR = build

CMAKE = cmake
CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

all: build compile_commands.json

compile_commands.json:
	ln -s $(BUILD_DIR)/$@ $@

build:
	$(CMAKE) $(CMAKE_FLAGS) -B $(BUILD_DIR)
	$(CMAKE) --build $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) compile_commands.json

.DEFAULT_GOAL := all
