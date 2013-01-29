#!/bin/env ruby
#
# An example that demonstrates how to request
# specific data fields about individual tracks.

$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'mediainfo'

def show(path)
  puts "*** MediaInfo.new(\"#{path}\") ***"
  info = MediaInfo.new(path)
  puts info

  g = info.general[0]
  puts "Format:   #{g['Format']} - #{g['Format/Info']}"
  puts "Duration: #{g['Duration/String']}"

  info.video.each do |track|
    puts
    puts "#{track}"
    puts "VIDEO[#{track.id}] Codec:      #{track['Codec']} - #{track['Codec/Info']}"
    puts "VIDEO[#{track.id}] Resolution: #{track['Width']}x#{track['Height']}"
    puts "VIDEO[#{track.id}] FrameRate:  #{track['FrameRate/String']}"
    puts "VIDEO[#{track.id}] Bits/Pixel: #{track['Bits-(Pixel*Frame)']}"
    puts "VIDEO[#{track.id}] BitRate:    #{track['BitRate/String']}"
  end

  info.audio.each do |track|
    puts
    puts "#{track}"
    puts "AUDIO[#{track.id}] Codec:      #{track['Codec']} - #{track['Codec/Info']}"
    puts "AUDIO[#{track.id}] Channels:   #{track['Channel(s)']}"
    puts "AUDIO[#{track.id}] BitDepth:   #{track['BitDepth/String']}"
    puts "AUDIO[#{track.id}] Channels:   #{track['SamplingRate']} (#{track['SamplingRate/String']})"
  end
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
