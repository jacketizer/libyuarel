SRC_FILES = yuarel.c
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

CC = gcc
CFLAGS = -c -fPIC -g -Wall
LDFLAGS =-s -shared -fvisibility=hidden -Wl,--exclude-libs=ALL,--no-as-needed,-soname,libyuarel.so
PREFIX ?= /usr

.PHONY: all
all: yuarel

.PHONY: yuarel
yuarel: $(SRC_FILES) $(OBJ_FILES)
	@echo "Building libyuarel.so..."
	$(CC) $(LDFLAGS) $(OBJ_FILES) -o libyuarel.so

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: install
install: all
	install --directory ${PREFIX}/lib ${PREFIX}/include
	install libyuarel.so ${PREFIX}/lib/
	install yuarel.h ${PREFIX}/include/
	ldconfig -n ${PREFIX}/lib

.PHONY: examples
examples: examples/simple.c
	$(CC) examples/simple.c -lyuarel -o simple

.PHONY: check
check:
	@mkdir -p build
	@PREFIX=build make install
	LIBRARY_PATH="build/lib" \
	LD_LIBRARY_PATH="build/lib" \
	C_INCLUDE_PATH="build/include" \
	$(CC) tests/test_lib.c -lyuarel -o test_lib && ldd test_lib && ./test_lib

.PHONY: clean
clean:
	rm -f *.o
	rm -fr build
	rm -f simple test_lib

.PHONY: dist-clean
dist-clean: clean
	rm -f libyuarel.so
