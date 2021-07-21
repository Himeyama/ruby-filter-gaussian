require 'mkmf'
require "rubygems"

narray_dir = File.dirname(Gem.find_files("narray.h").first)

$LIBS += " #{lib_numo_path}/narray.so"

create_makefile "Gaussian"