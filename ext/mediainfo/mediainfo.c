#include "ruby.h"

VALUE mMediaInfo;


void Init_mediainfo()
{
    mMediaInfo = rb_define_module("MediaInfo");
}
