#include "common.h"

void init_mediainfo_constants()
{
    cMediaInfo_mStream  = rb_define_module_under(cMediaInfo, "Stream");
    cMediaInfo_mInfo    = rb_define_module_under(cMediaInfo, "Info");
    cMediaInfo_mInfoOpt = rb_define_module_under(cMediaInfo, "InfoOption");
    cMediaInfo_mFileOpt = rb_define_module_under(cMediaInfo, "FileOption");

    rb_define_const(cMediaInfo_mStream,  "General",         INT2NUM(MediaInfo_Stream_General));
    rb_define_const(cMediaInfo_mStream,  "Video",           INT2NUM(MediaInfo_Stream_Video));
    rb_define_const(cMediaInfo_mStream,  "Audio",           INT2NUM(MediaInfo_Stream_Audio));
    rb_define_const(cMediaInfo_mStream,  "Text",            INT2NUM(MediaInfo_Stream_Text));
    rb_define_const(cMediaInfo_mStream,  "Chapters",        INT2NUM(MediaInfo_Stream_Chapters));
    rb_define_const(cMediaInfo_mStream,  "Image",           INT2NUM(MediaInfo_Stream_Image));
    rb_define_const(cMediaInfo_mStream,  "Menu",            INT2NUM(MediaInfo_Stream_Menu));
    rb_define_const(cMediaInfo_mStream,  "Max",             INT2NUM(MediaInfo_Stream_Max));

    rb_define_const(cMediaInfo_mInfo,    "Name",            INT2NUM(MediaInfo_Info_Name));
    rb_define_const(cMediaInfo_mInfo,    "Text",            INT2NUM(MediaInfo_Info_Text));
    rb_define_const(cMediaInfo_mInfo,    "Measure",         INT2NUM(MediaInfo_Info_Measure));
    rb_define_const(cMediaInfo_mInfo,    "Options",         INT2NUM(MediaInfo_Info_Options));
    rb_define_const(cMediaInfo_mInfo,    "Name_Text",       INT2NUM(MediaInfo_Info_Name_Text));
    rb_define_const(cMediaInfo_mInfo,    "MeasureText",     INT2NUM(MediaInfo_Info_Measure_Text));
    rb_define_const(cMediaInfo_mInfo,    "Info",            INT2NUM(MediaInfo_Info_Info));
    rb_define_const(cMediaInfo_mInfo,    "HowTo",           INT2NUM(MediaInfo_Info_HowTo));
    rb_define_const(cMediaInfo_mInfo,    "Max",             INT2NUM(MediaInfo_Info_Max));

    rb_define_const(cMediaInfo_mInfoOpt, "ShowInInform",    INT2NUM(MediaInfo_InfoOption_ShowInInform));
    rb_define_const(cMediaInfo_mInfoOpt, "Reserved",        INT2NUM(MediaInfo_InfoOption_Reserved));
    rb_define_const(cMediaInfo_mInfoOpt, "ShowInSupported", INT2NUM(MediaInfo_InfoOption_ShowInSupported));
    rb_define_const(cMediaInfo_mInfoOpt, "TypeOfValue",     INT2NUM(MediaInfo_InfoOption_TypeOfValue));
    rb_define_const(cMediaInfo_mInfoOpt, "Max"        ,     INT2NUM(MediaInfo_InfoOption_Max));

    rb_define_const(cMediaInfo_mFileOpt, "Nothing",         INT2NUM(MediaInfo_FileOption_Nothing));
    rb_define_const(cMediaInfo_mFileOpt, "NoRecursive",     INT2NUM(MediaInfo_FileOption_NoRecursive));
    rb_define_const(cMediaInfo_mFileOpt, "CloseAll",        INT2NUM(MediaInfo_FileOption_CloseAll));
    rb_define_const(cMediaInfo_mFileOpt, "Max",             INT2NUM(MediaInfo_FileOption_Max));
}
