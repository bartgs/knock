##
##  Copyright 2017-2018
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

OBJ_DIR = ../obj
CCX_SRC = $(patsubst ./%.c, %.c, $(shell find . -type f -name '*.c'))
CCX_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.oo, $(CCX_SRC))
CCX_DEP = $(patsubst %.c, $(OBJ_DIR)/%.dd, $(CCX_SRC))

SED = sed
GCC = g++
GCC_CFL+=-Wall -g -c -I./
GCC_LIB =
GCC_INC = $(filter-out %git $(OBJ_DIR),$(subst �, ,$(filter �%,$(subst ./,�, $(shell find . -type d)))))
$(shell mkdir -p $(OBJ_DIR) 2>/dev/null)
$(foreach subdir,$(GCC_INC),$(shell mkdir -p $(OBJ_DIR)/$(subdir) 2>/dev/null))

TARGET = $(OBJ_DIR)/knock

$(TARGET) : $(CCX_OBJ)
	$(GCC) $(CCX_OBJ) $(GCC_LIB) -o $(TARGET)

-include $(CCX_DEP)

$(OBJ_DIR)/%.dd: %.c
	@set -e; \
	$(RM) $@; \
	$(GCC) -M $(GCC_CFL) $< > $@.temp; \
	$(SED) 's,\($(*F)\)\.o[ :]*,$(patsubst %.c,%.oo,$(OBJ_DIR)/$<) $@: ,g' < $@.temp > $@; \
	$(RM) -f $@.temp

$(OBJ_DIR)/%.oo: %.c
	$(GCC) $(GCC_CFL) $< -o $@

rebuild:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory

clean:
	@$(RM) -rf $(OBJ_DIR)
	@$(RM) -f $(TARGET)

