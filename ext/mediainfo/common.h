#include "ruby.h"

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <MediaInfoDLL/MediaInfoDLL.h>

#define Q(x) #x
#define NOOP1(a)
#define NOOP2(a,b)

#define JOIN_(a,b) a##b
#define JOIN(a,b)  JOIN_(a,b)

#if defined(UNICODE) || defined (_UNICODE)
#  define MINFO_EXT_UNI 1
#  define MINFO_EXT_FMT "%S"
#  define MINFO_EXT_LEN wcslen
#else
#  undef MINFO_EXT_UNI
#  define MINFO_EXT_FMT "%s"
#  define MINFO_EXT_LEN strlen
#endif

#include "mediainfo.h"
#include "constants.h"
#include "api_methods.h"
#include "helpers.h"
