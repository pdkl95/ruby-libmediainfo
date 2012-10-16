#include "common.h"

char *stream_type_name(int type)
{
    switch(type) {
    case MediaInfo_Stream_General:  return "General";
    case MediaInfo_Stream_Video:    return "Video";
    case MediaInfo_Stream_Audio:    return "Audio";
    case MediaInfo_Stream_Text:     return "Text";
    case MediaInfo_Stream_Chapters: return "Chapters";
    case MediaInfo_Stream_Image:    return "Image";
    case MediaInfo_Stream_Menu:     return "Menu";
    default:                        return "Unknown!";
    }
}


static VALUE mi_stream_type_name(VALUE self, VALUE type)
{
    UNPACK_MI;
    int stream_type = NUM2INT(type);
    char *str = stream_type_name;
    return rb_str_new2(str);
}


static VALUE mi_count_streams(VALUE self, VALUE type)
{
    return mi_count_get(self, type, INT2NUM(-1));
}

static VALUE mi_count_stream_fields(VALUE self, VALUE type, VALUE stream_id)
{
    return mi_count_get(self, type, stream_id);
}

init_mediainfo_helpers() {
    rb_define_method(cMediaInfo, "stream_type_name", mi_stream_type_name,    1);
    rb_define_method(cMediaInfo, "count_streams",    mi_count_streams,       1);
    rb_define_method(cMediaInfo, "count_fields",     mi_count_stream_fields, 2);
}
