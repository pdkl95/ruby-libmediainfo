#include "common.h"

VALUE cMediaInfo;
VALUE cMediaInfo_mStream;
VALUE cMediaInfo_mInfo;
VALUE cMediaInfo_mInfoOpt;
VALUE cMediaInfo_mFileOpt;

wchar_t *mkWC(const char *c)
{
    const size_t sz = strlen(c) + 1;
    wchar_t *wc = malloc(sz * sizeof(wchar_t));
    mbstowcs(wc, c, sz);
    return wc;
}

void freeWC(wchar_t *wc)
{
    free(wc);
}


static void mi_free(mi_t *mi)
{
    xfree(mi);
}

static VALUE mi_new(int argc, VALUE *argv, VALUE class) {
    VALUE self;
    mi_t *p = ALLOC(mi_t);
    p->handle = MediaInfo_New();
    if (!p->handle) {
        rb_raise(rb_eRuntimeError, "MediaInfo_New() failed!");
    }

    self = Data_Wrap_Struct(class, NULL, mi_free, p);
    printf("handle = 0x%p\n", p->handle); fflush(stdout);
    rb_obj_call_init(self, argc, argv);
    return self;
}

static VALUE mi_initialize(int argc, VALUE *argv, VALUE self)
{
    if (argc > 0 ) {
        return mi_open(argc, argv, self);
    } else {
        return self;
    }
}

void Init_mediainfo(void)
{
    /*MediaInfoDLL_Load();
    if (!MediaInfoDLL_IsLoaded()) {
        rb_raise(rb_eLoadError, "Mediainfo.so not loaded!\n");
    }*/

    cMediaInfo  = rb_define_class("MediaInfo", rb_cObject);

    rb_define_singleton_method(cMediaInfo, "new", mi_new, -1);
    rb_define_method(cMediaInfo, "initialize", mi_initialize, -1);

    init_mediainfo_constants();
    init_mediainfo_api_methods();
    init_mediainfo_helpers();
}
