CC=gcc
CPP=g++
CFLAGS=-Wall -Wextra -Werror -std=c11
CPPFLAGS=-Wall -Wextra -Werror -std=c++17
CURSES=-lncurses
DIR=
QT_VIEW_DIR=gui/desktop/DesktopView.app/Contents/MacOS
TESTS_DIR=backend_tests
DIST_DIR=archive

MAIN_PROJECT=$(shell find brick_game gui backend_tests -type f -name '*.c' -o -name '*.cc') # list of all the C source files in the project

TETRIS_SOURCES=$(filter-out $(TESTS_DIR)/% %.cc, $(MAIN_PROJECT))
SNAKE_SOURCES=$(filter-out brick_game/tetris/%.c $(TESTS_DIR)/% gui/desktop/%, $(MAIN_PROJECT))
TETRIS_OBJECTS=$(TETRIS_SOURCES:.c=.o)
SNAKE_OBJECTS=$(SNAKE_SOURCES:.cc=.o)
TETRIS=tetris.game
SNAKE=snake.game

TETRIS_TEST_SOURCES=$(filter-out $(TESTS_DIR)/snake/% %.cc gui/% %/fsm_matrix.c, $(MAIN_PROJECT))
SNAKE_TEST_SOURCES=$(filter-out $(TESTS_DIR)/tetris/% %.c gui/% %/tetris_model.cc %/controller.cc, $(MAIN_PROJECT))
TETRIS_TEST_OBJECTS=$(TETRIS_TEST_SOURCES:.c=.o)
SNAKE_TEST_OBJECTS=$(SNAKE_TEST_SOURCES:.cc=.o)
TETRIS_TEST=$(TESTS_DIR)/tetris_tests/test
SNAKE_TEST=$(TESTS_DIR)/snake_tests/test

STATIC_LIB=libtetris_game.a

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

install:
	@cd gui/desktop && qmake && make
	@mv ./$(QT_VIEW_DIR)/DesktopView ./$(QT_VIEW_DIR)/$(SNAKE) 
	@echo "Game Installed Successfully"

uninstall: clean
	@rm -f ./$(QT_VIEW_DIR)/$(SNAKE) 
	@echo "Game Is Unistalled Successfully"

installcli: $(STATIC_LIB) $(SNAKE_OBJECTS)
	@$(CPP) $(CPPFLAGS) $^ $(CURSES) -o $(DIR)$(TETRIS)
	@echo "Game Installed Successfully"

uninstallcli: clean
	@rm -f $(DIR)$(TETRIS)
	@echo "Game Is Unistalled Successfully"

tests: test_snake test_tetris

test_snake: $(SNAKE_TEST_OBJECTS) 
	@$(CPP) $(CPPFLAGS) $^ -o $(SNAKE_TEST) $(CHECK_LIBCPP)
	@./$(SNAKE_TEST)

test_tetris: $(TETRIS_TEST_OBJECTS) 
	@$(CC) $(CFLAGS) $^ -o $(TETRIS_TEST) $(CHECK_LIB)
	@./$(TETRIS_TEST)

$(STATIC_LIB): $(TETRIS_OBJECTS)
	@ar rc $(STATIC_LIB) $^
	@echo "Static Library Created"

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
	@$(CPP) -c $(CPPFLAGS) $< -o $@

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
	clang-format -style=Google -n $(MAIN_PROJECT)

valgrind: tests
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --read-var-info=yes $(SNAKE_TEST)
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --read-var-info=yes $(TETRIS_TEST)

rungame: install
	@./$(QT_VIEW_DIR)/snake.game

rungamecli: installcli
	@./$(DIR)$(TETRIS)

git:
	git add .
	git commit
	git push origin develop

clean: clean_dist remove_save
	@rm -rf $(STATIC_LIB) $(TETRIS_OBJECTS) $(SNAKE_OBJECTS) $(TETRIS_TEST_OBJECTS) $(SNAKE_TEST_OBJECTS) $(TETRIS_TEST) $(SNAKE_TEST) gcov_report* *.gc* report/ *.o $(DIR)$(TETRIS)
	@make clean_desktop
	@echo "Everything is cleaned"

clean_desktop:
	@cd gui/desktop && make distclean && rm .qmake.stash

clean_dist:
	@cd ../ && rm -rf archive
	@cd ../ && rm -rf archive.tar.gz

remove_save:
	@rm -rf $(shell find . -type f -name '*.record') $(DIR)tetris.record $(DIR)snake.record