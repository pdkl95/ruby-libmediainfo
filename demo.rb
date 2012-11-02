#!/bin/env ruby

$LOAD_PATH.unshift File.expand_path('../lib', __FILE__)
require 'mediainfo'

file = if ARGV.length > 1
         ARGV.first
       else
         'test.avi'
       end

puts "*** beg const dump: MediaInfo::Stream"
MediaInfo::Stream.constants.each do |name|
  puts "  #{name.inspect}  \t= #{MediaInfo::Stream::const_get(name)}"
end
puts "*** end const dump: MediaInfo::Stream"
puts

puts "*** MediaInfo.new"
x = MediaInfo.new
puts "*** opening file: \"#{file}\""
x.open(file)
puts "*** open successful!"

#puts "***********"
#puts x.print_report!
#puts "***********"
