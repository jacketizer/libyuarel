CUR_DIR := $(shell pwd)
SRC_FILES := yuarel.c
OBJ_FILES := $(patsubst %.c, %.o, $(SRC_FILES))

# Semantic Versioning 2.0.0 https://semver.org/
# MAJOR version when you make incompatible API changes
VERSION_MAJOR := 1
# MINOR version when you add functionality in a backward compatible manner
VERSION_MINOR := 2
# PATCH version when you make backward compatible bug fixes
VERSION_PATCH := 0

VERSION := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
LIBNAME := yuarel
PKG_NAME := lib$(LIBNAME)-$(VERSION)

CC := gcc
AR := ar
CFLAGS := -c -fPIC -g -Wall -Werror -std=c99 -pedantic -Wunused -Wlogical-op -Wno-uninitialized -Wextra -Wformat-security -Wno-init-self -Wwrite-strings -Wshift-count-overflow -Wdeclaration-after-statement -Wno-undef -Wno-unknown-pragmas -Wno-stringop-truncation
LDFLAGS := -s -shared -fvisibility=hidden -Wl,--exclude-libs=ALL,--no-as-needed,-soname,lib$(LIBNAME).so.$(VERSION_MAJOR)
PREFIX ?= /usr

CHECK_CFLAGS := -Wall -Werror -std=c99 -pedantic
EXAMPLES_CFLAGS := -Wall -Werror -std=c99 -pedantic

DOXYFILE ?= Doxyfile
DOXYGEN_OUTPUT_DIR ?= doc

.PHONY: all
all: yuarel

.PHONY: update_readme
update_readme:
	# Library Version (From clib package metadata)
	echo "${VERSION}" | xargs -I{} sed -i 's|<version>.*</version>|<version>{}</version>|' README.md
	echo "${VERSION}" | xargs -I{} sed -i 's|<versionBadge>.*</versionBadge>|<versionBadge>![Version {}](https://img.shields.io/badge/version-{}-blue.svg)</versionBadge>|' README.md
	jq --arg version "${VERSION}" '.version = $$version' clib.json > clib.json.tmp && mv clib.json.tmp clib.json

.PHONY: yuarel
yuarel: $(SRC_FILES) $(OBJ_FILES)
	@echo "Building $(PKG_NAME)..."
	$(CC) $(LDFLAGS) $(OBJ_FILES) -o lib$(LIBNAME).so.$(VERSION_MAJOR)
	$(AR) rcs lib$(LIBNAME).a $(OBJ_FILES)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: install
install: all
	install --directory $(PREFIX)/lib $(PREFIX)/include
	install lib$(LIBNAME).so.$(VERSION_MAJOR) lib$(LIBNAME).a $(PREFIX)/lib/
	ln -fs $(PREFIX)/lib/lib$(LIBNAME).so.$(VERSION_MAJOR) $(PREFIX)/lib/lib$(LIBNAME).so
	install yuarel.h $(PREFIX)/include/
	ldconfig -n $(PREFIX)/lib

.PHONY: uninstall
uninstall:
	# Remove libraries
	rm -f $(PREFIX)/lib/lib$(LIBNAME).so.$(VERSION_MAJOR) $(PREFIX)/lib/lib$(LIBNAME).a
	# Remove the symbolic link
	rm -f $(PREFIX)/lib/lib$(LIBNAME).so
	# Remove the header file
	rm -f $(PREFIX)/include/yuarel.h
	# Re-run ldconfig to clear the cache
	ldconfig

.PHONY: examples
examples: examples/simple.c
	$(CC) $(EXAMPLES_CFLAGS) examples/simple.c -l$(LIBNAME) -o simple
	./simple

.PHONY: check
check:
	@mkdir -p build
	PREFIX=$(CUR_DIR)/build make install
	$(CC) $(CHECK_CFLAGS) tests/test_lib.c -Ibuild/include -Lbuild/lib -l$(LIBNAME) -o test_lib
	$(CC) $(EXAMPLES_CFLAGS) examples/simple.c -Ibuild/include -Lbuild/lib -l$(LIBNAME) -o simple

	# Run Main Tests
	LD_LIBRARY_PATH="build/lib" \
	./test_lib

	# Run Examples
	LD_LIBRARY_PATH="build/lib" \
	./simple

.PHONY: dist
dist:
	install -d $(PKG_NAME)
	install *.c $(PKG_NAME)/
	install *.h $(PKG_NAME)/
	install Makefile LICENSE README.md $(PKG_NAME)/
	cp -r tests $(PKG_NAME)/
	cp -r examples $(PKG_NAME)/
	tar -pczf $(PKG_NAME).tar.gz $(PKG_NAME)

.PHONY: format
format:
	# pip install clang-format
	clang-format -i *.c
	clang-format -i *.h
	clang-format -i tests/*.c
	clang-format -i tests/*.h
	clang-format -i examples/*.c

.PHONY: clean
clean:
	@echo "Cleaning up generated documentation..."
	rm -f *.o
	rm -f *.a
	rm -f *.so.*
	rm -fr build
	rm -f simple test_lib
	rm -rf $(DOXYGEN_OUTPUT_DIR)

.PHONY: dist-clean
dist-clean: clean
	rm -f libyuarel.so.*
	rm -rf $(PKG_NAME)
	rm -f $(PKG_NAME).tar.gz

# Run doxygen to generate documentation
.PHONY: doxygen
doxygen:
	@echo "Generating Doxygen documentation..."
	doxygen $(DOXYFILE)
