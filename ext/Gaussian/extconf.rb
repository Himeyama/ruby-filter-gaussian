require 'mkmf'

lib_path = `gem env gempath`.split(":")[1] + "/gems/numo-*"
numo_path = Dir.glob(lib_path)[0]
lib_numo_path = "#{numo_path}/lib/numo"

$LIBS += " #{lib_numo_path}/narray.so"

create_makefile "Gaussian"