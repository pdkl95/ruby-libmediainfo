#include "common.h"

VALUE mediainfo_chars_to_rstring(const MediaInfo_Char *str)
{
    VALUE rstring;
    char *rstr;

#ifdef MINFO_EXT_UNI
    size_t len = wcstombs(NULL, str, 0);
    rstr = malloc(len + 1);
    if ((size_t)-1 == wcstombs(rstr, str, len)) {
        rb_raise(rb_eEncodingError, "could not convert given wchar_t* string to char* as used by libmediainfo");
    }
#else
    rstr = str;
#endif

    rstring = rb_str_new2(rstr);

#ifdef MINFO_EXT_UNI
    free(rstr);
#endif

    return rstring;
}

MediaInfo_Char *rstring_to_mediainfo_chars(VALUE rstring)
{
    MediaInfo_Char *mi_str;
    char *rstr = RSTRING_PTR(rstring);

#ifdef MINFO_EXT_UNI
    size_t len = mbstowcs(NULL, rstr, 0);
    mi_str = malloc(len + 1);
    if ((size_t)-1 == mbstowcs(mi_str, rstr, len)) {
        rb_raise(rb_eEncodingError, "could not convert given char* string to wchar_t* as needed by libmediainfo");
    }
    mi_str[len] = '\0';
#else
    mi_str = rstr;
#endif

    return mi_str;
}

#ifdef MINFO_EXT_UNI
#  define RSTRING_TO_MINFO_FREE(x) free(x)
#else
#  define RSTRING_TO_MINFO_FREE(x)
#endif


VALUE mi_open(int argc, VALUE *argv, VALUE self)
{
    VALUE file_path;
    MediaInfo_Char *path;
    size_t retval;
    UNPACK_MI;

    printf("mi_open(): unpacked mi->handle\n"); fflush(stdout);
    printf("mi_open(): scanning args\n"); fflush(stdout);
    rb_scan_args(argc, argv, "10", &file_path);
    printf("mi_open(): converting arg to (MediaInfo_Char *)\n"); fflush(stdout);
    path = rstring_to_mediainfo_chars(file_path);

    printf("mi_open(): path = '" MINFO_EXT_FMT "'\n", path);
    fflush(stdout);
    void *m = MediaInfo_New();
    retval = MediaInfo_Open(m, L"test.avi");
    //retval = MediaInfo_Open(mi->handle, path);
    printf("mi_open(): MediaInfo_Open() returned %zd; calling free()\n", retval); fflush(stdout);
    RSTRING_TO_MINFO_FREE(path);
    printf("mi_open(): free()ed the converted-string buffer\n"); fflush(stdout);
    if (0 == retval) {
        rb_raise(rb_eIOError, "MediaInfo_Open() failed");
    }

    printf("mi_open(): finished!\n"); fflush(stdout);
    return self;
}

VALUE mi_close(VALUE self)
{
    UNPACK_MI;
    MediaInfo_Close(mi->handle);
    return self;
}

const MediaInfo_Char *mi_report_string(mi_t *mi)
{
    const MediaInfo_Char *str = MediaInfo_Inform(mi->handle, 0);
    return str;
}

VALUE mi_report_str(VALUE self)
{
    VALUE rstr;
    const MediaInfo_Char *str;
    UNPACK_MI;

    str = mi_report_string(mi);
    rstr = mediainfo_chars_to_rstring(str);
    return rstr;
}

VALUE mi_get_i(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_id, VALUE request_type)
{
    const MediaInfo_Char *mi_str;
    VALUE rstr;
    UNPACK_MI;

    mi_str = MediaInfo_GetI(mi->handle,
                            NUM2INT(stream_type),
                            NUM2INT(stream_id),
                            NUM2INT(field_id),
                            NUM2INT(request_type));

    rstr = mediainfo_chars_to_rstring(mi_str);
    return rstr;
}

VALUE mi_get(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_name, VALUE request_type)
{
    VALUE rstr;
    MediaInfo_Char *mi_field_name;
    const MediaInfo_Char *mi_str;
    UNPACK_MI;

    mi_field_name = rstring_to_mediainfo_chars(field_name);

    mi_str = MediaInfo_Get(mi->handle,
                           NUM2INT(stream_type),
                           NUM2INT(stream_id),
                           mi_field_name,
                           NUM2INT(request_type),
                           0);

    RSTRING_TO_MINFO_FREE(mi_field_name);

    rstr = mediainfo_chars_to_rstring(mi_str);
    return rstr;
}

VALUE mi_option(VALUE self, VALUE name, VALUE value)
{
    VALUE rstr;
    MediaInfo_Char *mi_name;
    MediaInfo_Char *mi_value;
    const MediaInfo_Char *mi_str;
    UNPACK_MI;

    mi_name  = rstring_to_mediainfo_chars(name);
    mi_value = rstring_to_mediainfo_chars(value);

    mi_str = MediaInfo_Option(mi->handle, mi_name, mi_value);

    RSTRING_TO_MINFO_FREE(mi_value);
    RSTRING_TO_MINFO_FREE(mi_name);

    rstr = mediainfo_chars_to_rstring(mi_str);
    return rstr;
}

VALUE mi_state_get(VALUE self)
{
    UNPACK_MI;
    return INT2NUM(MediaInfo_State_Get(mi->handle));
}

VALUE mi_count_get(VALUE self, VALUE stream_type, VALUE stream_id)
{
    UNPACK_MI;
    return INT2NUM( MediaInfo_Count_Get(mi->handle,
                                        NUM2INT(stream_type),
                                        NUM2INT(stream_id)) );
}

void init_mediainfo_api_methods(void) {
    rb_define_method(cMediaInfo, "open",        mi_open,       -1);
    rb_define_method(cMediaInfo, "close",       mi_close,      0);
    rb_define_method(cMediaInfo, "report_str",  mi_report_str, 0);
    rb_define_method(cMediaInfo, "get_by_id",   mi_get_i,      4);
    rb_define_method(cMediaInfo, "get_by_name", mi_get,        4);
    rb_define_method(cMediaInfo, "option",      mi_option,     2);
    rb_define_method(cMediaInfo, "status_get",  mi_state_get,  0);
    rb_define_method(cMediaInfo, "count_get",   mi_count_get,  2);
}
