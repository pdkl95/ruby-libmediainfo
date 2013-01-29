#include "common.h"
#include "mediainfo.h"
#include "stream_type.h"
#include "track.h"

VALUE cMediaInfo_cStreamType;

const char *
mediainfo_stream_type_to_name(int stream_type)
{
    switch(stream_type) {
    case MediaInfo_Stream_General:  return "general";
    case MediaInfo_Stream_Video:    return "video";
    case MediaInfo_Stream_Audio:    return "audio";
    case MediaInfo_Stream_Text:     return "text";
    case MediaInfo_Stream_Chapters: return "chapters";
    case MediaInfo_Stream_Image:    return "image";
    case MediaInfo_Stream_Menu:     return "menu";
    default:                        return "unknown";
    }
}

static mediainfo_stream_type_t *
mediainfo_stream_type_t_alloc()
{
    mediainfo_stream_type_t *st;
    st = ALLOC(mediainfo_stream_type_t);
    st->mediainfo  = Qnil;
    st->tracks     = Qnil;
    st->num_tracks = 0;
    return st;
}

static void
mediainfo_stream_type_free(mediainfo_stream_type_t *st)
{
    xfree(st);
}

static void
mediainfo_stream_type_mark(mediainfo_stream_type_t *st)
{
    MARK(st->mediainfo);
    MARK(st->name);
    MARK(st->tracks);
}


static VALUE
mediainfo_stream_type_allocate(VALUE klass)
{
    mediainfo_stream_type_t *st = mediainfo_stream_type_t_alloc();
    st->tracks = rb_ary_new();
    return Data_Wrap_Struct(klass,
                            mediainfo_stream_type_mark,
                            mediainfo_stream_type_free,
                            st);
}

#define GETTER(name)                                \
static VALUE                                        \
JOIN(mediainfo_stream_type_get_,name)(VALUE self)   \
{                                                   \
    UNPACK_STREAM_TYPE;                             \
    return st->name;                                \
}
GETTER(mediainfo);
GETTER(tracks);
GETTER(name);
#undef GETTER

static VALUE
mediainfo_stream_type_load_tracks(VALUE self)
{
    int i;
    size_t n;
    VALUE rn;
    VALUE track;
    VALUE argv[3];
    UNPACK_STREAM_TYPE;
    GET_MEDIAINFO(mi, st->mediainfo);

    argv[0] = st->mediainfo;
    argv[1] = self;
    rn = mediainfo_count_get(st->mediainfo, INT2NUM(st->type), INT2NUM(-1));
    st->num_tracks = NUM2INT(rn);

    for (i = 0; i < st->num_tracks; i++) {
        argv[2] = INT2NUM(i);
        track = rb_class_new_instance(3, argv, cMediaInfo_cTrack);
        rb_ary_push(st->tracks, track);
        rb_ary_push(mi->tracks, track);
    }
    return self;
}

static VALUE
mediainfo_stream_type_initialize(VALUE self, VALUE mediainfo, VALUE stream_type)
{
    UNPACK_STREAM_TYPE;
    st->mediainfo = mediainfo;
    st->type      = NUM2INT(stream_type);
    st->cname     = mediainfo_stream_type_to_name(st->type);
    st->name      = rb_str_new2(st->cname);
    return mediainfo_stream_type_load_tracks(self);
}

static VALUE
mediainfo_stream_type_inspect(VALUE self)
{
    char *buf;
    UNPACK_STREAM_TYPE;

#define INSPECT_FMT "#<MediaInfo::StreamType num_tracks=%zd>"
    buf = alloca(8 + sizeof(INSPECT_FMT));
    sprintf(buf, INSPECT_FMT, st->num_tracks);
#undef INSPECT_FMT
    return rb_str_new2(buf);
}

static VALUE
mediainfo_stream_type_to_s(VALUE self)
{
    return mediainfo_stream_type_inspect(self);
}

static VALUE
mediainfo_stream_type_track(VALUE self, VALUE idx)
{
    UNPACK_STREAM_TYPE;
    return rb_ary_entry(st->tracks, NUM2INT(idx));
}

static VALUE
mediainfo_stream_type_each(VALUE self)
{
    UNPACK_STREAM_TYPE;
    return rb_ary_each(st->tracks);
}

void
Init_mediainfo_MediaInfo_StreamType(void)
{
    cMediaInfo_cStreamType = rb_define_class_under(cMediaInfo,
                                                   "StreamType",
                                                   rb_cObject);
    rb_define_alloc_func(cMediaInfo_cStreamType,
                         mediainfo_stream_type_allocate);

#define GET(name)                                           \
    rb_define_method(cMediaInfo_cStreamType,                \
                     Q(name),                               \
                     JOIN(mediainfo_stream_type_get_,name), \
                     0);
    GET(mediainfo);
    GET(tracks);
    GET(name);
#undef GET

#define M(suffix, name, numargs)                          \
    rb_define_method(cMediaInfo_cStreamType,              \
                     name,                                \
                     JOIN(mediainfo_stream_type_,suffix), \
                     numargs);
    M(initialize, "initialize", 2);
    M(to_s,       "to_s",       0);
    M(track,      "track",      1);
    M(track,      "[]",         1);
    M(each,       "each",       0);
#undef M
}
