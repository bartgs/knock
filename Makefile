##
##  Copyright 2015-2018
##  Author(s): Bart Gysens <gysens.bart@gmx.com>
##
##  This file is part of Knock.
##
##  Knock is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  Knock is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with Knock.  If not, see <http://www.gnu.org/licenses/>.
##

OBJ_DIR = obj
PKG_DIR = pkg

all:	knock

knock:
	@cd src; \
	make -f Makefile; \
	cd ..;\
	cp $(OBJ_DIR)/knock pkg/usr/bin
	dpkg-deb --build $(PKG_DIR) $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)

rebuild: clean knock



