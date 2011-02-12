# PyOpenSG is (C) Copyright 2005-2009 by Allen Bierbaum
#
# This file is part of PyOpenSG.
#
# PyOpenSG is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# PyOpenSG is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#
# Init file for osg package.
#

# Hack to bring in all symbols OpenSG is linked against and loads from plugins
# Search for : setdlopenflags and RTLD_GLOBAL on google to see why

original_dlopen_flags = None
try:
   import dl, sys
   original_dlopen_flags = sys.getdlopenflags()
   sys.setdlopenflags(original_dlopen_flags | dl.RTLD_GLOBAL)
except:
   pass

# Import everything from the opensg module
from OSGSystemPy import *

if original_dlopen_flags:
   sys.setdlopenflags(original_dlopen_flags)

