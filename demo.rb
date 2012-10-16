#!/bin/env ruby

$LOAD_PATH.unshift File.expand_path('../lib', __FILE__)
require 'mediainfo'

x = MediaInfo.new('bitbuf.avi')
puts "***********"
puts x.simple_report
puts "***********"
