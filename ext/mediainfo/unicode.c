#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#include "common.h"
#include "unicode.h"

VALUE
mediainfo_chars_to_rstring(const MediaInfo_Char *str)
{
    VALUE rstring;
    char *rstr;

#ifdef MINFO_EXT_UNI
    size_t len = wcstombs(NULL, str, 0);
    rstr = alloca(len + 1);
    if ((size_t)-1 == wcstombs(rstr, str, len)) {
        rb_raise(rb_eEncodingError, "could not convert given wchar_t* string to char* as used by libmediainfo");
    }
#else
    rstr = str;
#endif

    rstring = rb_str_new2(rstr);

    return rstring;
}

MediaInfo_Char *
rstring_to_mediainfo_chars(VALUE rstring)
{
    MediaInfo_Char *mi_str;
    char *rstr = RSTRING_PTR(rstring);

#ifdef MINFO_EXT_UNI
    size_t len = mbstowcs(NULL, rstr, 0);
    mi_str = xmalloc(sizeof(MediaInfo_Char) * (len + 1));
    if ((size_t)-1 == mbstowcs(mi_str, rstr, len)) {
        rb_raise(rb_eEncodingError, "could not convert given char* string to wchar_t* as needed by libmediainfo");
    }
    mi_str[len] = L'\0';
#else
    mi_str = rstr;
#endif

    return mi_str;
}


VALUE
mediainfo_string_to_rb(const MediaInfo_Char *str)
{
    bool isnum = true;
    const MediaInfo_Char *p;
    for (p = str; *p; p++) {
        if (!MINFO_EXT_ISDIGIT(*p)) {
            isnum = false;
            break;
        }
    }

    if (isnum) {
        return LL2NUM(MINFO_EXT_STRTOLL(str, NULL, 10));
    } else {
#ifdef MINFO_EXT_UNI
        return mediainfo_chars_to_rstring(str);
#else
        return rb_str_new2(str);
#endif
    }
}
