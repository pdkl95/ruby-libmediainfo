#include "ruby.h"
#include <ruby/encoding.h>

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <MediaInfoDLL/MediaInfoDLL.h>

#define Q(x) #x
#define NOOP1(a)
#define NOOP2(a,b)

#define JOIN_(a,b) a##b
#define JOIN(a,b)  JOIN_(a,b)

#if SIZEOF_MEDIAINFO_CHAR > 1
#  define MEDIAINFO_USE_WCHAR 1
#else
#  undef MEDIAINFO_USE_WCHAR
#endif

#if   SIZEOF_MEDIAINFO_CHAR == 1
#  define MEDIAINFO_CHAR_ENCODING "UTF-8"
#elif SIZEOF_MEDIAINFO_CHAR == 2
#  define MEDIAINFO_CHAR_ENCODING "UTF-16LE"
#elif SIZEOF_MEDIAINFO_CHAR == 4
#  define MEDIAINFO_CHAR_ENCODING "UTF-32LE"
#else
#  error "Unusual size for the type \"MediaInfo_Char\" - giving up"
#endif


#if MEDIAINFO_USE_WCHAR
#  define miS(x) ((wchar *)(JOIN(L,x)))
#  define mic_strlen(x) wcslen((wchar_t *)x)
#  define mic_printf(x, ...) wprintf(JOIN(L,x), __VA_ARGS__)
#else
#  define miS(x) ((char *)x)
#  define mic_strlen(x) strlen((char *)x)
#  define mic_printf(x, ...) printf(x, __VA_ARGS__)
#endif

#include "mediainfo.h"
#include "constants.h"
#include "api_methods.h"
#include "helpers.h"
