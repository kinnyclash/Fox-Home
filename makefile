# ----------------------------
# Fox Home - CE C/C++ toolchain makefile
# ----------------------------
NAME = FOXHOME
DESCRIPTION = "Fox Home v0.11.18"
COMPRESSED = YES
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------
include $(shell cedev-config --makefile)
