#include "common.h"

VALUE mi_open(int argc, VALUE *argv, VALUE self)
{
    VALUE file_path;
    const char *path;
    const MediaInfo_Char *path_wc;
    size_t retval;
    UNPACK_MI;

    if (argc < 1) {
        rb_raise(rb_eArgError, "Missing: the filename to open");
    } else {
        file_path = argv[0];

        path = RSTRING_PTR(StringValue(file_path));
        path_wc = makeWC(path);
        wprintf("OPEN: path = '%s'\n", path_wc); fflush(stdout);
        retval = MediaInfo_Open(&mi->handle, path_wc);
        freeWC(path_wc);

        if (0 == retval) {
            rb_raise(rb_eIOError, "MediaInfo_Open() failed");
        }
    }
    printf("OPEN: finished!\n"); fflush(stdout);
    return self;
}

VALUE mi_close(VALUE self)
{
    UNPACK_MI;
    MediaInfo_Close(mi->handle);
    return self;
}

char *mi_report_string(mi_t *mi)
{
    const MediaInfo_Char *str = MediaInfo_Inform(mi->handle, 0);
    printf(">>>  str = 0x%p (length: %zd)\n", str, strlen(str));

    return str;
}

VALUE mi_report_str(VALUE self)
{
    UNPACK_MI;
    return rb_str_new2(mi_report_string(mi));
}

VALUE mi_get_i(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_id, VALUE request_type)
{
    UNPACK_MI;
    return rb_str_new2( MediaInfo_GetI(mi->handle,
                                       NUM2INT(stream_type),
                                       NUM2INT(stream_id),
                                       NUM2INT(field_id),
                                       NUM2INT(request_type)) );
}

VALUE mi_get(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_name, VALUE request_type)
{
    UNPACK_MI;
    return rb_str_new2( MediaInfo_Get(mi->handle,
                                      NUM2INT(stream_type),
                                      NUM2INT(stream_id),
                                      RSTRING_PTR(field_name),
                                      NUM2INT(request_type),
                                      0));
}

VALUE mi_option(VALUE self, VALUE name, VALUE value)
{
    UNPACK_MI;
    return rb_str_new2( MediaInfo_Option(mi->handle,
                                         RSTRING_PTR(name),
                                         RSTRING_PTR(value)) );
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
