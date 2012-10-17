#include "common.h"

VALUE cMediaInfo;
VALUE cMediaInfo_mStream;
VALUE cMediaInfo_mInfo;
VALUE cMediaInfo_mInfoOpt;
VALUE cMediaInfo_mFileOpt;

static void mi_dealloc(void *mi)
{
    free(mi);
}

static VALUE mi_alloc(VALUE klass)
{
    mi_t *mi = malloc(sizeof(mi_t));
    mi->handle = MediaInfo_New();

    return Data_Wrap_Struct(klass, NULL, mi_dealloc, mi);
}

static VALUE mi_initialize(VALUE self, VALUE path)
{
    return mi_open(self, path);
}

void Init_mediainfo(void)
{
    MediaInfoDLL_Load();
    if (!MediaInfoDLL_IsLoaded()) {
        rb_raise(rb_eLoadError, "Mediainfo.so not loaded!\n");
    }

    cMediaInfo  = rb_define_class("MediaInfo", rb_cObject);
    rb_define_alloc_func(cMediaInfo, mi_alloc);
    rb_define_method(cMediaInfo, "initialize", mi_initialize, 1);

    init_mediainfo_constants();
//    init_mediainfo_api_methods();
//    init_mediainfo_helpers();
}
