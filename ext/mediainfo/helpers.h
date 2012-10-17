#ifndef LIBMI_HELPERS_H
#define LIBMI_HELPERS_H

const char *stream_type_name(int type);

VALUE mi_stream_type_name(VALUE self, VALUE type);
VALUE mi_count_stream_fields(VALUE self, VALUE type, VALUE stream_id);
VALUE mi_count_streams(VALUE self, VALUE type);
VALUE mi_print_report_bang(VALUE self);

void init_mediainfo_helpers(void);

#endif /*LIBMI_HELPERS_H*/