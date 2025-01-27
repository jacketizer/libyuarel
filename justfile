default:
    just --list

# Install Static and Dynamic Library Into /usr
install:
    sudo make install

# Uninstall Static and Dynamic Library from /usr
uninstall:
    sudo make uninstall

# Run Tests check
check:
    make check

# Run Examples Program (Requires yuarel.h to be installed into system)
examples:
    make examples

# Update Readme Content
update_readme:
    make update_readme

# Generate Documentation
doxygen:
    make doxygen

# Format Code To Repo Standard
format:
    make format
