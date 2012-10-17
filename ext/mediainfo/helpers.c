#include "common.h"

const char *stream_type_name(int type)
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


VALUE mi_stream_type_name(VALUE self, VALUE type)
{
    return rb_str_new2( stream_type_name(NUM2INT(type)) );
}


VALUE mi_count_streams(VALUE self, VALUE type)
{
    return mi_count_get(self, type, INT2NUM(-1));
}

VALUE mi_count_stream_fields(VALUE self, VALUE type, VALUE stream_id)
{
    return mi_count_get(self, type, stream_id);
}

VALUE mi_print_report_bang(VALUE self)
{
    char *str;
    MediaInfo_Char *str_wc;
    UNPACK_MI;
    str = mi_report_string(mi);
    str_wc = allocWC(str);
    wprintf(L"str = 0x%p (length: %zd)\n", str_wc, wcslen(str_wc));
    wprintf(L"%s", str_wc);
    freeWC(str_wc);
}

void init_mediainfo_helpers(void) {
    rb_define_method(cMediaInfo, "stream_type_name", mi_stream_type_name,    1);
    rb_define_method(cMediaInfo, "count_streams",    mi_count_streams,       1);
    rb_define_method(cMediaInfo, "count_fields",     mi_count_stream_fields, 2);
    rb_define_method(cMediaInfo, "print_report!",    mi_print_report_bang,   0);
}