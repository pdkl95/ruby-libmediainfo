#!/bin/env ruby

$LOAD_PATH.unshift File.expand_path('../lib', __FILE__)
require 'mediainfo'

def show(path)
  puts "*** MediaInfo.new(\"#{path}\") ***"
  mi = MediaInfo.new
  mi.open(path)
  puts mi
  puts mi.f_general(0, 'Format').inspect
  puts mi.f_general(0, 'Format/Info').inspect
  puts mi.f_general(0, 'Duration').inspect
  puts mi.f_video(0, 'Format').inspect
  puts mi.f_audio(0, 'Format').inspect
  #puts mi.get_field(MediaInfo::Stream::General, 0, 'Format/Info').inspect
  #puts mi.get(MediaInfo::Stream::General, 0, 'Format/Info', MediaInfo::Info::Text).inspect
end

if ARGV.length < 1
  puts "usage: #{File.basename($0)} <media_file> [<media_file> [...]]"
  exit 1
end

ARGV.each do |media_file|
  show media_file
end

#puts "***********"
#puts x.print_report!
#puts "***********"
