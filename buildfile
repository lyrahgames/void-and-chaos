cxx.std = latest
using cxx

hxx{*}: extension = hpp
cxx{*}: extension = cpp

import libs = sfml-graphics%lib{sfml-graphics}

exe{main}: {hxx cxx}{**} $libs

cxx.poptions =+ "-I$src_root"