#!/usr/bin/env ruby

require 'mkmf'

[ 'stdlib.h', 'string.h', 'wchar.h'
].each do |hdr|
  find_header(hdr) or fail "missing required header: #{hdr}"
end

pkg_config 'libmediainfo'
create_makefile 'mediainfo'
