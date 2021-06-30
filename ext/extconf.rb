require 'mkmf'

$CPPFLAGS += " -I ~/.rbenv/versions/3.0.1/lib/ruby/gems/3.0.0/extensions/x86_64-linux/3.0.0/numo-narray-0.9.1.9/numo"
$LIBS += " ~/.rbenv/versions/3.0.1/lib/ruby/gems/3.0.0/extensions/x86_64-linux/3.0.0/numo-narray-0.9.1.9/numo/narray.so"

create_makefile "Gaussian"