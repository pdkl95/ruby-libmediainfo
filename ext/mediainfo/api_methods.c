#include "common.h"

VALUE mediainfo_chars_to_rstring(const MediaInfo_Char *str)
{
    rb_encoding *enc = rb_enc_find(MEDIAINFO_CHAR_ENCODING);
    VALUE str_encoded = rb_external_str_new_with_enc((char *)str, wcslen(str), enc);
    return rb_str_conv_enc(str_encoded, enc, rb_utf8_encoding());
}

MediaInfo_Char *rstring_to_mediainfo_chars(VALUE rstring)
{
    rb_encoding *enc;
    VALUE rstring_encoded;
    MediaInfo_Char *str;

    //printf("[rstring -> " MEDIAINFO_CHAR_ENCODING "] \n"); fflush(stdout);
    enc = rb_enc_find(MEDIAINFO_CHAR_ENCODING);
    if (!enc) {
        rb_raise(rb_eRuntimeError, "no encoding return from: rb_enc_find(\"" MEDIAINFO_CHAR_ENCODING "\");\n");
    }

    printf("[rstring -> " MEDIAINFO_CHAR_ENCODING "] calling rb_str_export_to_enc()\n"); fflush(stdout);
    rstring_encoded = rb_str_export_to_enc(rstring, enc);
    if NIL_P(rstring_encoded) {
        rb_raise(rb_eRuntimeError, "failure in rb_str_export_to_enc()\n");
    }
    printf("[rstring -> " MEDIAINFO_CHAR_ENCODING "] converting VALUE to c-string\n"); fflush(stdout);

    str = (MediaInfo_Char *)RSTRING_PTR(rstring_encoded);
    printf("[rstring -> " MEDIAINFO_CHAR_ENCODING "] returning new string\n"); fflush(stdout);
    return str;
}


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

    mic_printf("mi_open(): [printf, char *] path = '%s'\n", path);
    fflush(stdout);

    retval = MediaInfo_Open(&mi->handle, path);
    printf("mi_open(): \n"); fflush(stdout);
    free(path);
    printf("mi_open(): \n"); fflush(stdout);
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
    mic_printf(">>>  str = 0x%p (length: %zd)\n", str, mic_strlen(str));
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
    const MediaInfo_Char *mi_str;
    UNPACK_MI;

    mi_str = MediaInfo_Get(mi->handle,
                           NUM2INT(stream_type),
                           NUM2INT(stream_id),
                           rstring_to_mediainfo_chars(field_name),
                           NUM2INT(request_type),
                           0);
    rstr = mediainfo_chars_to_rstring(mi_str);
    return rstr;
}

VALUE mi_option(VALUE self, VALUE name, VALUE value)
{
    VALUE rstr;
    const MediaInfo_Char *mi_str;
    UNPACK_MI;

    mi_str = MediaInfo_Option(mi->handle,
                              rstring_to_mediainfo_chars(name),
                              rstring_to_mediainfo_chars(value));
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
