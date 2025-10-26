BUILD_DIR = build
NPROC = $(shell nproc 2>/dev/null || echo 4)

.PHONY: all configure build clean fclean re run

all: configure build

configure:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		echo "Creating build directory..."; \
		mkdir -p $(BUILD_DIR); \
	fi
	@echo "Configuring with CMake..."
	@cd $(BUILD_DIR) && cmake ..

build:
	@echo "Building project..."
	@$(MAKE) -C $(BUILD_DIR) -j$(NPROC)
	@echo "Build complete! Executable: ./sdl_project"

clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		echo "Cleaning build files..."; \
		$(MAKE) -C $(BUILD_DIR) clean; \
	fi

fclean: clean
	@echo "Removing build directory..."
	@rm -rf $(BUILD_DIR)
	@rm -f sdl_project

re: fclean all

run: all
	@echo "Running application..."
	@./sdl_project
