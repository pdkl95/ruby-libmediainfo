#include "common.h"

VALUE cMediaInfo;

static void mi_free(mi_t *mi)
{
    xfree(mi);
}

static VALUE mi_new(int argc, VALUE *argv, VALUE class) {
    VALUE self;
    mi_t *p = ALLOC(mi_t);
    printf("mi_new(): calling MediaInfo_New()\n"); fflush(stdout);
    p->handle = MediaInfo_New();
    printf("mi_new(): back from MediaInfo_New()\n"); fflush(stdout);
    if (!p->handle) {
        rb_raise(rb_eRuntimeError, "MediaInfo_New() failed!");
    }
    printf("mi_new(): handle = 0x%p\n", p->handle); fflush(stdout);
    self = Data_Wrap_Struct(class, NULL, mi_free, p);
    printf("mi_new(): allocated self with Data_Wrap_Struct()\n"); fflush(stdout);
    printf("mi_new(): callinng rb_obj_call_init()\n"); fflush(stdout);
    rb_obj_call_init(self, argc, argv);
    printf("mi_new(): returning self\n"); fflush(stdout);
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
    MediaInfoDLL_Load();
    if (!MediaInfoDLL_IsLoaded()) {
        rb_raise(rb_eLoadError, "Mediainfo.so not loaded!\n");
    }

    cMediaInfo  = rb_define_class("MediaInfo", rb_cObject);

    rb_define_singleton_method(cMediaInfo, "new", mi_new, -1);
    rb_define_method(cMediaInfo, "initialize", mi_initialize, -1);

    init_mediainfo_constants();
    init_mediainfo_api_methods();
    init_mediainfo_helpers();
}
