default:
    just --list

# Install Static and Dynamic Library Into /usr
install:
    sudo make install

# Run Tests check
check:
    make check

# Run Examples check
examples:
    make examples

# Update Readme Content
update_readme:
    make update_readme

# Generate Documentation
doxygen:
    make doxygen
