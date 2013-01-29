#include "common.h"
#include "api.h"

mediainfo_api_t cMediaInfoAPI;

static void
check_binding(void *func, char *name)
{
    if (!func) {
        rb_raise(rb_eLoadError,
                 "Failed to bind MediaInfo_%s() from libmediainfo.so",
                 name);
    }
}

void
Init_mediainfo_api(void)
{
    MediaInfoDLL_Load();
    if (!MediaInfoDLL_IsLoaded()) {
        rb_raise(rb_eLoadError, "Mediainfo.so not loaded!\n");
    }

#define MI_API(field, name)                                             \
    cMediaInfoAPI.field = JOIN(MediaInfo_,name);                        \
    check_binding(cMediaInfoAPI.field, Q(name));

    MI_API(new,New);
    MI_API(delete,Delete);
    MI_API(open,Open);
    MI_API(close,Close);
    MI_API(inform,Inform);
    MI_API(geti,GetI);
    MI_API(get,Get);
    MI_API(option,Option);
    MI_API(state_get,State_Get);
    MI_API(count_get,Count_Get);
#undef MI_API
}
