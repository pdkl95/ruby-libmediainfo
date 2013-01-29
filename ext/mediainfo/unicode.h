#ifndef GEM_MEDIAINFO_UNICODE_H
#define GEM_MEDIAINFO_UNICODE_H

#if defined(UNICODE) || defined (_UNICODE)
#  define MINFO_EXT_UNI 1
#  define MINFO_EXT_FMT "%S"
#  define MINFO_EXT_LEN wcslen
#  define MINFO_EXT_ISDIGIT iswdigit
#  define MINFO_EXT_STRTOLL wcstoll
#  define RSTRING_TO_MINFO_FREE(x) xfree(x)
#else
#  undef MINFO_EXT_UNI
#  define MINFO_EXT_FMT "%s"
#  define MINFO_EXT_LEN strlen
#  define MINFO_EXT_ISDIGIT isdigit
#  define MINFO_EXT_STRTOLL strtoll
#  define RSTRING_TO_MINFO_FREE(x)
#endif

VALUE            mediainfo_chars_to_rstring(const MediaInfo_Char *str);
MediaInfo_Char * rstring_to_mediainfo_chars(VALUE rstring);
VALUE            mediainfo_string_to_rb(const MediaInfo_Char *str);

#endif /*GEM_MEDIAINFO_UNICODE_H*/
