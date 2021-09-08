require 'mkmf'

narray_dir = Gem.find_files("numo").map{|e| e.include?("narray") ? e : false}.select{|e| e}.to_a[0]
$INCFLAGS += " -I#{narray_dir}"
$libs += " #{narray_dir}/narray.so"

create_makefile "Gaussian"
