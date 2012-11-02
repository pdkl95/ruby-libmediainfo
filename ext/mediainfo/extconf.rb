#!/usr/bin/env ruby

require 'mkmf'

[ 'stdlib.h', 'string.h', 'wchar.h'
].each do |hdr|
  find_header(hdr) or fail "missing required header: #{hdr}"
end

pkg_config 'libmediainfo' or fail "pkg_config failed for libmediainfo"

mi_hdr = 'MediaInfoDLL/MediaInfoDLL.h'
have_header(mi_hdr) or
  fail "missing libmediainfo header \"#{mi_hdr}\""

mi_char = 'MediaInfo_Char'
have_type(mi_char, mi_hdr) or
  fail "expected heaader file \"#{mi_hdr}\" to define the type \"#{mi_char}\""

mi_char_size = check_sizeof(mi_char, mi_hdr)
case mi_char_size
when 1, 2, 4
  #puts "sizeof(MediaInfo_Char) == #{mi_char_size}"
else
  fail "Unsupported size of MediaInfo_Char (#{mi_char_size})"
end

create_header
create_makefile 'mediainfo'
