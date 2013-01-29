#include "common.h"
#include "mediainfo.h"
#include "track.h"

VALUE cMediaInfo_cTrack;

void Init_mediainfo_MediaInfo_Track(void)
{
    cMediaInfo_cTrack = rb_define_class_under(cMediaInfo, "Track", rb_cObject);
}
