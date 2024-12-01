#ifndef STRING_STREAM_H_454354
#define STRING_STREAM_H_454354

struct string_stream;

struct string_stream *string_stream_create(const char *raw_string);
int string_stream_read_int(struct string_stream *stream, int *num);
int string_stream_read_string(struct string_stream *stream, char *string);
void string_stream_close(struct string_stream *stream);

#endif
