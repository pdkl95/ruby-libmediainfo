#!/usr/bin/env ruby

require 'mkmf'

[ 'stdlib.h', 'string.h', 'wchar.h'
].each do |hdr|
  find_header(hdr) or fail "missing required header: #{hdr}"
end

pkg_config 'libmediainfo' or fail "pkg_config failed for libmediainfo"

mi_hdr = 'MediaInfoDLL/MediaInfoDLL.h'
have_header(mi_hdr) or fail "missing libmediainfo header \"#{mi_hdr}\""

#create_header
create_makefile 'libmediainfo'

if ENV['V'] == '1'
  system("sed -i -e 's/^V = 0/V = 1/' Makefile")
end
