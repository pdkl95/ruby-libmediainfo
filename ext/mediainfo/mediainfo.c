#include "common.h"
#include "mediainfo.h"
#include "unicode.h"
#include "api.h"
#include "constants.h"

VALUE cMediaInfo;

/***************************************************************************
 * struct mediainfo
 */

static mediainfo_t *
mediainfo_alloc()
{
    mediainfo_t *mi;
    mi = ALLOC(mediainfo_t);
    mi->handle = cMediaInfoAPI.new();
    mi->path = Qnil;
    return mi;
}

static void
mediainfo_free(mediainfo_t *mi)
{
    cMediaInfoAPI.delete(mi->handle);
    xfree(mi);
}

static void
mediainfo_mark(mediainfo_t *mi)
{
    if (mi->path != Qnil) {
        rb_gc_mark(mi->path);
    }
}

static VALUE
mediainfo_allocate(VALUE klass) {
    mediainfo_t *mi = mediainfo_alloc();
    if (!mi->handle) {
        rb_raise(rb_eRuntimeError, "MediaInfo_New() failed!");
    }
    return Data_Wrap_Struct(klass, mediainfo_mark, mediainfo_free, mi);
}

static VALUE
mediainfo_compare(VALUE self, VALUE other)
{
    GET_MI(a, self);
    GET_MI(b, other);
    return rb_funcall(a->path, rb_intern("<=>"), 1, b->path);
}

static VALUE
mediainfo_path_get(VALUE self)
{
    UNPACK_MI;
    return mi->path;
}

static VALUE
mediainfo_path_set(VALUE self, VALUE val)
{
    UNPACK_MI;
    mi->path = val;
    return val;
}

static VALUE
mediainfo_inspect(VALUE self)
{
    char *buf;
    UNPACK_MI;

    if (mi->path == Qnil) {
        return rb_str_new2("#<MediaInfo nil>");
    } else {
#define INSPECT_FMT "#<MediaInfo \"%s\">"
        buf = alloca(RSTRING_LEN(mi->path) + sizeof(INSPECT_FMT));
        sprintf(buf, INSPECT_FMT, RSTRING_PTR(mi->path));
        return rb_str_new2(buf);
    }
}

static VALUE
mediainfo_to_s(VALUE self)
{
    UNPACK_MI;
    return mediainfo_inspect(self);
}

static VALUE
mediainfo_open(int argc, VALUE *argv, VALUE self)
{
    MediaInfo_Char *path;
    size_t retval;
    UNPACK_MI;

    rb_scan_args(argc, argv, "10", &mi->path);
    mi->path = StringValue(mi->path);
    path = rstring_to_mediainfo_chars(mi->path);

    if (!cMediaInfoAPI.open(mi->handle, path)) {
        rb_raise(rb_eIOError, "MediaInfo_Open() failed");
    }
    RSTRING_TO_MINFO_FREE(path);
    return self;
}

static VALUE
mediainfo_initialize(int argc, VALUE *argv, VALUE self)
{
    if (argc > 0 ) {
        return mediainfo_open(argc, argv, self);
    } else {
        return self;
    }
}

static VALUE
mediainfo_close(VALUE self)
{
    UNPACK_MI;
    cMediaInfoAPI.close(mi->handle);
    return self;
}

static const MediaInfo_Char *
mediainfo_report_string(mediainfo_t *mi)
{
    const MediaInfo_Char *str = cMediaInfoAPI.inform(mi->handle, 0);
    return str;
}

static VALUE
mediainfo_inform(VALUE self)
{
    VALUE rstr;
    const MediaInfo_Char *str;
    UNPACK_MI;

    str = mediainfo_report_string(mi);
    rstr = mediainfo_chars_to_rstring(str);
    return rstr;
}

static VALUE
mediainfo_get_i(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_id, VALUE request_type)
{
    const MediaInfo_Char *mi_str;
    UNPACK_MI;

    mi_str = cMediaInfoAPI.geti(mi->handle,
                             NUM2INT(stream_type),
                             NUM2INT(stream_id),
                             NUM2INT(field_id),
                             NUM2INT(request_type));

    return mediainfo_string_to_rb(mi_str);
}

static VALUE
mediainfo_get(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_name, VALUE request_type)
{
    MediaInfo_Char *mi_field_name;
    const MediaInfo_Char *mi_str;
    UNPACK_MI;

    mi_field_name = rstring_to_mediainfo_chars(field_name);

    mi_str = cMediaInfoAPI.get(mi->handle,
                            NUM2INT(stream_type),
                            NUM2INT(stream_id),
                            mi_field_name,
                            NUM2INT(request_type),
                            0);

    RSTRING_TO_MINFO_FREE(mi_field_name);

    return mediainfo_string_to_rb(mi_str);
}

static VALUE
mediainfo_option(VALUE self, VALUE name, VALUE value)
{
    VALUE rstr;
    MediaInfo_Char *mi_name;
    MediaInfo_Char *mi_value;
    const MediaInfo_Char *mi_str;
    UNPACK_MI;

    mi_name  = rstring_to_mediainfo_chars(name);
    mi_value = rstring_to_mediainfo_chars(value);

    mi_str = cMediaInfoAPI.option(mi->handle, mi_name, mi_value);

    RSTRING_TO_MINFO_FREE(mi_value);
    RSTRING_TO_MINFO_FREE(mi_name);

    rstr = mediainfo_chars_to_rstring(mi_str);
    return rstr;
}

static VALUE
mediainfo_state_get(VALUE self)
{
    UNPACK_MI;
    return INT2NUM(cMediaInfoAPI.state_get(mi->handle));
}

static VALUE
mediainfo_count_get(VALUE self, VALUE stream_type, VALUE stream_id)
{
    UNPACK_MI;
    return INT2NUM(cMediaInfoAPI.count_get(mi->handle,
                                        NUM2INT(stream_type),
                                        NUM2INT(stream_id)));
}

static VALUE
mediainfo_stream_type(int argc, VALUE *argv, VALUE self)
{
    VALUE type_number;
    rb_scan_args(argc, argv, "10", &type_number);
    return rb_intern( mediainfo_stream_type_name(NUM2INT(type_number)) );
}


static VALUE
mediainfo_num_streams(VALUE self, VALUE type)
{
    return mediainfo_count_get(self, type, INT2NUM(-1));
}

static VALUE
mediainfo_num_fields(VALUE self, VALUE type, VALUE stream_id)
{
    return mediainfo_count_get(self, type, stream_id);
}

static VALUE
mediainfo_get_field(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_name)
{
    return mediainfo_get(self, stream_type, stream_id, field_name,
                  INT2NUM(MediaInfo_Info_Text));
}

#define FIELD_GET(t,T)                                                  \
    static VALUE                                                        \
    JOIN(mediainfo_f_,t)(VALUE self, VALUE stream_id, VALUE field_name) \
    {                                                                   \
        return mediainfo_get_field(self, INT2NUM(JOIN(MediaInfo_Stream_,T)), \
                                   stream_id, field_name);              \
    }
FIELD_GET(general,General);
FIELD_GET(video,Video);
FIELD_GET(audio,Audio);
FIELD_GET(text,Text);
FIELD_GET(chapters,Chapters);
FIELD_GET(image,Image);
FIELD_GET(menu,Menu);
#undef FIELD_GET


void
Init_mediainfo_MediaInfo(void)
{
    cMediaInfo = rb_define_class("MediaInfo", rb_cObject);
    rb_define_alloc_func(cMediaInfo, mediainfo_allocate);

    rb_define_method(cMediaInfo, "<=>",   mediainfo_compare,  1);
    rb_define_method(cMediaInfo, "path",  mediainfo_path_get, 0);
    rb_define_method(cMediaInfo, "path=", mediainfo_path_set, 1);

#define M(name, numargs)                                            \
    rb_define_method(cMediaInfo, Q(name), JOIN(mediainfo_,name), numargs);
    M(initialize,  -1);
    M(inspect,      0);
    M(to_s,         0);
    M(open,        -1);
    M(close,        0);
    M(inform,       0);
    M(get_i,        4);
    M(get,          4);
    M(option,       2);
    M(state_get,    0);
    M(count_get,    2);
    M(stream_type, -1);
    M(num_streams,  1);
    M(num_fields,   2);
    M(get_field,    3);
    M(f_general,    2);
    M(f_video,      2);
    M(f_audio,      2);
    M(f_text,       2);
    M(f_chapters,   2);
    M(f_image,      2);
    M(f_menu,       2);
#undef M
}
