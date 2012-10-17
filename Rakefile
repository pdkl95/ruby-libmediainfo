# encoding: utf-8

require 'rubygems'

begin
  require 'bundler'
rescue LoadError => e
  warn e.message
  warn "Run `gem install bundler` to install Bundler."
  exit -1
end

begin
  Bundler.setup(:development)
rescue Bundler::BundlerError => e
  warn e.message
  warn "Run `bundle install` to install missing gems."
  exit e.status_code
end

require 'rake'
require 'rake/extensiontask'

spec = Gem::Specification.load('libmediainfo.gemspec')

Rake::ExtensionTask.new do |ext|
  ext.name = 'mediainfo'
  ext.ext_dir = 'ext/mediainfo'
  ext.lib_dir = 'lib'
  ext.tmp_dir = 'tmp'
  ext.source_pattern = '*.{c,h}'
  ext.gem_spec = spec
end

require 'rubygems/tasks'
Gem::Tasks.new checksum: true, pgp: true


require 'rspec/core/rake_task'
RSpec::Core::RakeTask.new

task :test => :spec
task :default => :spec


require 'yard'
YARD::Rake::YardocTask.new
task :doc => :yard



