require 'mkmf'

narray_dir = Gem.find_files("numo")[0]
have_library("narray")

create_makefile "Gaussian"