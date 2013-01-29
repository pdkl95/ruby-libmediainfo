#!/bin/env ruby
#
# This demo simply iterats through each field in
# each track that libmediainfo provided.

$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'mediainfo'

def show(path)
  puts ">>> \"#{path}\""
  info = MediaInfo.new(path)

  info.each_track do |track|
    track.each_field do|k,v|
      puts "#{track.stream_type.name}[#{track.id}] #{k}: #{v.inspect}"
    end
  end
end

if ARGV.length < 1
  puts "usage: #{File.basename($0)} <media_file> [<media_file> [...]]"
  exit 1
end

ARGV.each do |media_file|
  show media_file
end

