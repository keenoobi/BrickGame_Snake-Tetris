CC=gcc
CPP=g++
CFLAGS=-Wall -Wextra -Werror -std=c11
CPPFLAGS=-Wall -Wextra -Werror -std=c++17
CURSES=-lncurses
DIR=
DIST_DIR=archive
TETRIS_PROJECT=$(shell find brick_game/tetris backend_tests -type f -name '*.c') # list of all the C source files in the project
SNAKE_PROJECT=$(shell find brick_game gui backend_tests -type f -name '*.cc') # list of all the C++ source files in the project
TETRIS_SOURCES=$(filter-out backend_tests/%.c, $(TETRIS_PROJECT))
SNAKE_SOURCES=$(filter-out gui/desktop/%.cc backend_tests/%, $(SNAKE_PROJECT))
TETRIS_TEST_SOURCES=$(shell find brick_game/tetris backend_tests -type f -name '*.c')
SNAKE_TEST_SOURCES=$(filter-out gui/% brick_game/tetris/%.cc brick_game/controller/%.cc, $(SNAKE_PROJECT))
TETRIS=tetris.game
TETRIS_TEST=backend_tests/tetris_tests/test
SNAKE_TEST=backend_tests/snake_tests/test
TETRIS_OBJECTS=$(TETRIS_SOURCES:.c=.o)
SNAKE_OBJECTS=$(SNAKE_SOURCES:.cc=.o)
TETRIS_TEST_OBJECTS=$(TETRIS_TEST_SOURCES:.c=.o)
SNAKE_TEST_OBJECTS=$(SNAKE_TEST_SOURCES:.cc=.o)
RESULT=libtetris_game.a

.PHONY: all clean install uninstall test gcov_report clang dvi dist rungame rungamecli git
OS := $(shell uname)
ifeq ($(OS),Linux)
	OPEN_COMMAND= xdg-open
	OPEN= $(OPEN_COMMAND) report/index.html
	CHECK_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURSE
	CHECK_LIBCPP = -lgtest -lsubunit -lm -lrt -lpthread -D_GNU_SOURSE
endif

ifeq ($(OS),Darwin)
	OPEN_COMMAND= open
	OPEN= $(OPEN_COMMAND) report/index.html
	CHECK_LIB = -lcheck
	CHECK_LIBCPP = -lgtest
endif

all: rungame

show:
	@echo $(SNAKE_PROJECT)


install:
	cd gui/desktop && qmake && make
	mv ./gui/desktop/DesktopView.app/Contents/MacOS/DesktopView ./gui/desktop/DesktopView.app/Contents/MacOS/snake.game

uninstall: clean
	@rm -f ./gui/desktop/DesktopView.app/Contents/MacOS/snake.game
	@echo "Game Is Unistalled Successfully"

installcli: $(RESULT) $(SNAKE_OBJECTS)
	@$(CPP) $(CPPFLAGS) $^ $(CURSES) -o $(DIR)$(TETRIS)
	@echo "Game Installed Successfully"

uninstallcli: clean
	@rm -f $(DIR)$(TETRIS)
	@echo "Game Is Unistalled Successfully"

test_snake: $(SNAKE_TEST_OBJECTS) 
	$(CPP) $(CPPFLAGS) $^ -o $(SNAKE_TEST) $(CHECK_LIBCPP)
	@./$(SNAKE_TEST)

test_tetris: $(TETRIS_TEST_OBJECTS) 
	@$(CC) $(CFLAGS) $^ -o $(TETRIS_TEST) $(CHECK_LIB)
	@./$(TETRIS_TEST)

$(RESULT): $(TETRIS_OBJECTS)
	@ar rc $(RESULT) $^
	echo "Static Library Created"

dvi:
	$(OPEN_COMMAND) ../dvi/README.html

dist: clean_dist
	cd ../ && mkdir -p $(DIST_DIR)
	cd ../ && cp -rf src/brick_game $(DIST_DIR)/brick_game
	cd ../ && cp -rf src/gui $(DIST_DIR)/gui
	cd ../ && cp -rf src/Makefile $(DIST_DIR)/
	cd ../ && tar -czvf archive.tar.gz $(DIST_DIR)
	cd ../ && rm -rf $(DIST_DIR)

dist_unpack:
	cd ../ && tar -xzvf archive.tar.gz


.c.o:
	@$(CC) -c $(CFLAGS) $< -o $@

.cc.o:
	$(CPP) -c $(CPPFLAGS) $< -o $@

gcov_report: $(SNAKE_TEST_SOURCES) 
	$(CPP) $(CPPFLAGS) --coverage $^ $(CHECK_LIBCPP) -o $@
	chmod +x *
	./$@ 
	lcov -t  "$@" --ignore-errors inconsistent -o $@.info --no-external -c -d .
	genhtml -o report/ $@.info
	$(OPEN)

gcov_report_tetris: $(TETRIS_TEST_SOURCES) 
	$(CC) $(CFLAGS) --coverage $^ $(CHECK_LIB) -o $@
	chmod +x *
	./$@
	lcov -t "$@" -o $@.info --no-external -c -d .
	genhtml -o report/ $@.info
	$(OPEN)

clang:
	clang-format -style=Google -n $(TETRIS_PROJECT) $(SNAKE_PROJECT)

valgrind:
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --read-var-info=yes $(TEST_RESULT)

rungame: install
	@./gui/desktop/DesktopView.app/Contents/MacOS/snake.game

rungamecli: installcli
	@./$(DIR)$(TETRIS)

git:
	git add .
	git commit
	git push origin develop

clean: clean_dist remove_save
	@rm -rf $(RESULT) $(TETRIS_OBJECTS) $(SNAKE_OBJECTS) $(TETRIS_TEST_OBJECTS) $(SNAKE_TEST_OBJECTS) $(TETRIS_TEST) $(SNAKE_TEST) gcov_report* *.gc* report/ *.o $(DIR)$(TETRIS)
	@make clean_desktop
	@echo "Everything is cleaned"

clean_desktop:
	@cd gui/desktop && make distclean && rm .qmake.stash

clean_dist:
	@cd ../ && rm -rf archive
	@cd ../ && rm -rf archive.tar.gz

remove_save:
	@rm -rf $(shell find . -type f -name '*.record') $(DIR)tetris.record $(DIR)snake.record