CUR_DIR := $(shell pwd)
SRC_FILES := yuarel.c
OBJ_FILES := $(patsubst %.c, %.o, $(SRC_FILES))

VERSION_MAJOR := 1
VERSION_MINOR := 0
VERSION_PATCH := 0
VERSION := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
LIBNAME := yuarel
PKG_NAME := lib$(LIBNAME)-$(VERSION)

CC := gcc
AR := ar
CFLAGS := -c -fPIC -g -Wall
LDFLAGS := -s -shared -fvisibility=hidden -Wl,--exclude-libs=ALL,--no-as-needed,-soname,lib$(LIBNAME).so.$(VERSION_MAJOR)
PREFIX ?= /usr

.PHONY: all
all: yuarel

# Dev Note: $ is used by both make and AWK. Must escape $ for use in AWK within makefile.
.PHONY: readme_update
readme_update:
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

.PHONY: examples
examples: examples/simple.c
	$(CC) examples/simple.c -l$(LIBNAME) -o simple

.PHONY: check
check:
	@mkdir -p build
	PREFIX=$(CUR_DIR)/build make install
	$(CC) tests/test_lib.c -Ibuild/include -Lbuild/lib -l$(LIBNAME) -o test_lib
	LD_LIBRARY_PATH="build/lib" \
	./test_lib

.PHONY: dist
dist:
	install -d $(PKG_NAME)
	install *.c $(PKG_NAME)/
	install *.h $(PKG_NAME)/
	install Makefile LICENSE README.md $(PKG_NAME)/
	cp -r tests $(PKG_NAME)/
	cp -r examples $(PKG_NAME)/
	tar -pczf $(PKG_NAME).tar.gz $(PKG_NAME)

.PHONY: clean
clean:
	rm -f *.o
	rm -fr build
	rm -f simple test_lib

.PHONY: dist-clean
dist-clean: clean
	rm -f libyuarel.so.*
	rm -rf $(PKG_NAME)
	rm -f $(PKG_NAME).tar.gz
