# -*- encoding: utf-8 -*-

$LOAD_PATH.unshift File.expand_path('../lib', __FILE__)
require 'mediainfo/version'

Gem::Specification.new do |gem|
  gem.name = "libmediainfo"
  gem.version = MediaInfo::VERSION::STRING
  gem.summary = "Ruby bindings for libmediainfo.so"
  gem.description = "Ruby bindings for MediaInfo using libmediainfo.so"
  gem.license = "GPL-3"
  gem.authors = ["Brent Sanders"]
  gem.email = "pdkl95@thoughtnoise.net"
  gem.homepage = "https://github.com/pdkl95/ruby-libmediainfo#readme"

  gem.files = `git ls-files`.split($/)
  gem.executables = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files = gem.files.grep(%r{^(test|spec|features)/})
  gem.require_paths = ['lib']

  gem.extensions << "ext/libmediainfo/extconf.rb"

  gem.add_development_dependency 'bundler', '~> 1.2'
  gem.add_development_dependency 'rake', '~> 10.0'
  gem.add_development_dependency 'redcarpet', '~> 2.2'
  gem.add_development_dependency 'rspec', '~> 2.11'
  gem.add_development_dependency 'rubygems-tasks', '~> 0.2'
  gem.add_development_dependency 'yard', '~> 0.8'
  gem.add_development_dependency 'rake-compiler', '~> 0.8.1'
end
