# frozen_string_literal: true

# require_relative "lib/gaussian/filter/version"

Gem::Specification.new do |spec|
  spec.name          = "gaussian-filter"
  spec.version       = "0.0.3"
  spec.authors       = ["Murata Mitsuharu"]
  spec.email         = ["hikari.photon+dev@gmail.com"]
  spec.summary       = "Gaussian Filter"
  spec.homepage      = "https://himeyama.github.io"
  spec.required_ruby_version = ">= 2.4.0"
  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{\A(?:test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["ext"]
  spec.extensions = %w[ext/Gaussian/extconf.rb]

  spec.add_dependency "numo-narray"
end
