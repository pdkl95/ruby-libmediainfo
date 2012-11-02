#include "common.h"

static const char *stream_type_name(int type)
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


static VALUE mi_stream_type_name(int argc, VALUE *argv, VALUE self)
{
    VALUE type_number;
    rb_scan_args(argc, argv, "10", &type_number);
    return rb_str_new2( stream_type_name(NUM2INT(type_number)) );
}


static VALUE mi_count_streams(VALUE self, VALUE type)
{
    return mi_count_get(self, type, INT2NUM(-1));
}

static VALUE mi_count_stream_fields(VALUE self, VALUE type, VALUE stream_id)
{
    return mi_count_get(self, type, stream_id);
}

static VALUE mi_print_report(VALUE self)
{
    UNPACK_MI;
    mic_printf("%s", mi_report_string(mi));
    return self;
}

void init_mediainfo_helpers(void) {
#define M(name, numargs)                                            \
    rb_define_method(cMediaInfo, Q(name), JOIN(mi_,name), numargs);

    M(stream_type_name,     1);
    M(count_streams,        1);
    M(count_stream_fields,  2);
    M(print_report,        -1);

#undef M
}
