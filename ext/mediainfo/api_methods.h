#ifndef LIBMI_API_METHODS_H
#define LIBMI_API_METHODS_H

VALUE mi_open(int argc, VALUE *argv, VALUE self);
VALUE mi_close(VALUE self);
char *mi_report_string(mi_t *mi);
VALUE mi_report_str(VALUE self);
VALUE mi_get_i(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_id, VALUE request_type);

VALUE mi_get(VALUE self, VALUE stream_type, VALUE stream_id, VALUE field_name, VALUE request_type);
VALUE mi_option(VALUE self, VALUE name, VALUE value);
VALUE mi_state_get(VALUE self);
VALUE mi_count_get(VALUE self, VALUE stream_type, VALUE stream_id);

void init_mediainfo_api_methods(void);

#endif /*LIBMI_API_METHODS_H*/
