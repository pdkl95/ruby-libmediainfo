#!/usr/bin/env ruby

require 'mkmf'

have_header('dlfcn.h') or fail "missing header: dlfcn.h"

have_library('dl', 'dlopen', 'dlfcn.h') or fail "missing library: dl"



create_makefile 'mediainfo'
