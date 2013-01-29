#include "common.h"
#include "mediainfo.h"
#include "stream_type.h"
#include "track.h"

VALUE cMediaInfo_cTrack;

static mediainfo_track_t *
mediainfo_track_t_alloc()
{
    mediainfo_track_t *track;
    track = ALLOC(mediainfo_track_t);
    track->mediainfo   = Qnil;
    track->fields      = Qnil;
    track->stream_type = Qnil;
    track->id          = 0;
    return track;
}

static void
mediainfo_track_free(mediainfo_track_t *track)
{
    xfree(track);
}

static void
mediainfo_track_mark(mediainfo_track_t *track)
{
    MARK(track->mediainfo);
    MARK(track->fields);
    MARK(track->stream_type);
}

static VALUE
mediainfo_track_allocate(VALUE klass) {
    mediainfo_track_t *track = mediainfo_track_t_alloc();
    track->fields = rb_hash_new();
    return Data_Wrap_Struct(klass,
                            mediainfo_track_mark,
                            mediainfo_track_free,
                            track);
}

static VALUE
mediainfo_track_compare(VALUE self, VALUE other)
{
    GET_MEDIAINFO_TRACK(a, self);
    GET_MEDIAINFO_TRACK(b, other);

    if (rb_obj_class(other) == cMediaInfo_cTrack) {
        if (a->id < b->id) { return -1; }
        if (a->id < b->id) { return  1; }
        return INT2NUM(0);
    } else {
        return rb_funcall(a->mediainfo, rb_intern("<=>"), 1, b->mediainfo);
    }
}

#define GETTER(name)                            \
static VALUE                                    \
JOIN(mediainfo_track_get_,name)(VALUE self)     \
{                                               \
    UNPACK_TRACK;                               \
    return track->name;                         \
}
GETTER(mediainfo);
GETTER(stream_type);
GETTER(fields);
GETTER(id);
#undef GETTER

static VALUE
mediainfo_track_field(VALUE self, VALUE field_name)
{
    VALUE val;
    UNPACK_TRACK;
    val = rb_hash_lookup(track->fields, field_name);
    if (!val) {
        rb_raise(rb_eKeyError,
                 "No such field: \"%s\"",
                 RSTRING_PTR(field_name));
    }
    return val;
}

static VALUE
mediainfo_track_load_fields(VALUE self)
{
    int i;
    VALUE rtype, rn, ri, name, text;
    UNPACK_TRACK;
    GET_MEDIAINFO(mi, track->mediainfo);
    GET_MEDIAINFO_STREAM_TYPE(st, track->stream_type);

    rtype = INT2NUM(st->type);
    rn = mediainfo_count_get(track->mediainfo, rtype, track->id);
    track->num_fields = NUM2INT(rn);

    for (i = 0; i < track->num_fields; i++) {
        ri = INT2NUM(i);
        name = mediainfo_get_i(track->mediainfo, rtype, track->id, ri, INT2NUM(MediaInfo_Info_Name));
        text = mediainfo_get_i(track->mediainfo, rtype, track->id, ri, INT2NUM(MediaInfo_Info_Text));
        if (text != Qnil) {
            rb_hash_aset(track->fields, name, text);
        }
    }
}

static VALUE
mediainfo_track_initialize(VALUE self, VALUE mediainfo, VALUE stream_type, VALUE track_id)
{
    UNPACK_TRACK;
    GET_MEDIAINFO_STREAM_TYPE(st, stream_type);

    track->mediainfo   = mediainfo;
    track->stream_type = stream_type;
    track->id          = track_id;
    track->id_int      = NUM2INT(track_id);
    return mediainfo_track_load_fields(self);
}

static VALUE
mediainfo_track_inspect(VALUE self)
{
    char *buf;
    UNPACK_TRACK;

    if (track->id == Qnil) {
        return rb_str_new2("#<MediaInfo::Track nil>");
    } else if (track->fields == Qnil) {
#define INSPECT_FMT "#<MediaInfo::Track id=%d>"
        buf = alloca(8 + sizeof(INSPECT_FMT));
        sprintf(buf, INSPECT_FMT, track->id_int);
#undef INSPECT_FMT
        return rb_str_new2(buf);
    } else {
#define INSPECT_FMT "#<MediaInfo::Track id=%d num_fields=%d>"
        buf = alloca(16 + sizeof(INSPECT_FMT));
        sprintf(buf, INSPECT_FMT, track->id_int, track->num_fields);
#undef INSPECT_FMT
        return rb_str_new2(buf);
    }
}

int do_each_pair(VALUE key, VALUE val, VALUE data)
{
    rb_yield_values(2, key, val);
    return ST_CONTINUE;
}

static VALUE
mediainfo_track_keys(VALUE self)
{
    UNPACK_TRACK;
    return rb_funcall(track->fields, rb_intern("keys"), 0);
}

static VALUE
mediainfo_track_each(VALUE self)
{
    UNPACK_TRACK;
    rb_need_block();
    rb_hash_foreach(track->fields, do_each_pair, Qnil);
    return self;
}

static VALUE
mediainfo_track_to_s(VALUE self)
{
    return mediainfo_track_inspect(self);
}

void Init_mediainfo_MediaInfo_Track(void)
{
    cMediaInfo_cTrack = rb_define_class_under(cMediaInfo, "Track", rb_cObject);
    rb_define_alloc_func(cMediaInfo_cTrack, mediainfo_track_allocate);

#define GET(name)                                     \
    rb_define_method(cMediaInfo_cTrack,               \
                     Q(name),                         \
                     JOIN(mediainfo_track_get_,name), \
                     0);
    GET(mediainfo);
    GET(stream_type);
    GET(fields);
    GET(id);
#undef GET

#define M(suffix, name, numargs)                    \
    rb_define_method(cMediaInfo_cTrack,             \
                     name,                          \
                     JOIN(mediainfo_track_,suffix), \
                     numargs);
    M(compare,       "<=>",         1);
    M(field,         "[]",          1);
    M(field,         "field",       1);
    M(get_mediainfo, "mediainfo",   0);
    M(get_fields,    "fields",      0);
    M(initialize,    "initialize",  3);
    M(to_s,          "to_s",        0);
    M(keys,          "keys",        0);
    M(each,          "each",        0);
    M(each,          "each_field",  0);
#undef M
}
