#ifndef LIBMI_MEDIAINFO_H
#define LIBMI_MEDIAINFO_H

#include <ruby.h>
#include <MediaInfoDLL/MediaInfoDLL.h>

#define Q(x) #x
#define JOIN_(a,b) a##b
#define JOIN(a,b)  JOIN_(a,b)

#define GET_CLASS_STRUCT(type_t, mi, selfptr)       \
    type_t *mi;                                   \
    Data_Get_Struct(selfptr, type_t, mi);

#define MARK(x)                                 \
    if (x != Qnil) { rb_gc_mark(x); }


#endif /*GEM_MEDIAINFO_MEDIAINFO_H*/
