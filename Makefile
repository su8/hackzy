#   05/24/2022
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.

#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.

#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#   MA 02110-1301, USA.

CFLAGS+=-Wall -Wextra -O2 -std=c++17 -pipe -pedantic -Wundef -Wshadow -W -Wwrite-strings -Wcast-align -Wstrict-overflow=5 -Wconversion -Wpointer-arith -Wformat=2 -Wsign-compare -Wendif-labels -Wredundant-decls -Winit-self
SRCS=main.cpp
PROGS=hackzy
BIN_DIR=/usr/bin

all:
	$(CXX) $(CFLAGS) -o $(PROGS) $(SRCS)

install: 
	install -D -s -m 755 hackzy $(BIN_DIR)/hackzy

uninstall:
	rm -f $(BIN_DIR)/hackzy

clean:
	rm -f hackzy

.PHONY: all install clean uninstall
