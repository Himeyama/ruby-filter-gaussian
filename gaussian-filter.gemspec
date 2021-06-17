# frozen_string_literal: true

# require_relative "lib/gaussian/filter/version"

Gem::Specification.new do |spec|
  spec.name          = "gaussian-filter"
  # spec.version       = Gaussian::Filter::VERSION
  spec.version       = "0.0.1"
  spec.authors       = ["Murata Mitsuharu"]
  spec.email         = ["hikari.photon+mygit@gmail.com"]

  spec.summary       = ""
  spec.description   = ""
  spec.homepage      = "https://himeyama.github.io"
  spec.required_ruby_version = ">= 2.4.0"

  spec.metadata["allowed_push_host"] = ""

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "#{spec.homepage}"
  spec.metadata["changelog_uri"] = "#{spec.homepage}"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{\A(?:test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["ext"]

  # spec.extensions << "ext/extconf.rb"
end
