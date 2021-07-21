# frozen_string_literal: true

require "bundler/gem_tasks"
task default: %i[]

require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new "Gaussian" do |ext|
    ext.lib_dir = "ext"
end