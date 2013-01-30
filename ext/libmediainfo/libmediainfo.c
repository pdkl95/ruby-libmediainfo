#include "common.h"
#include "libmediainfo.h"
#include "api.h"
#include "constants.h"
#include "mediainfo.h"
#include "stream_type.h"
#include "track.h"

void
Init_libmediainfo(void)
{
    Init_mediainfo_api();
    Init_mediainfo_MediaInfo();
    Init_mediainfo_MediaInfo_StreamType();
    Init_mediainfo_MediaInfo_Track();
    Init_mediainfo_constants();
}
