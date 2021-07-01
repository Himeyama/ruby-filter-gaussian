require 'mkmf'

$CPPFLAGS += " -I/usr/lib/x86_64-linux-gnu/ruby/vendor_ruby/2.7.0"
$LIBS += " /usr/lib/x86_64-linux-gnu/ruby/vendor_ruby/2.7.0/narray.so"
create_makefile "Gaussian"