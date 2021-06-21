require 'mkmf'

$CPPFLAGS += " -I#{ENV["HOME"]}/.rbenv/versions/3.0.1/lib/ruby/gems/3.0.0/extensions/x86_64-linux/3.0.0/numo-narray-0.9.1.9/numo"


# p "#{ENV["HOME"]}/.rbenv/versions/3.0.1/lib/ruby/gems/3.0.0/extensions/x86_64-linux/3.0.0/numo-narray-0.9.1.9/numo/numo/narray.h"

create_makefile "Gaussian"