#include "string_stream.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void ignore_whitespaces(struct string_stream *stream);
static int is_whitespace(char c);

struct string_stream {
	const char *string;
};

struct string_stream *string_stream_create(const char *raw_string) {
	struct string_stream *stream = (struct string_stream *)malloc(sizeof(struct string_stream));

	stream->string = raw_string;

	return stream;
}

int string_stream_read_int(struct string_stream *stream, int *num) {
	char buffer[10];
	int success = sscanf(stream->string, "%s", buffer);
	if (success != EOF) {
		stream->string += strlen(buffer);
		ignore_whitespaces(stream);
		*num = atoi(buffer);
	}

	return success;
}

int string_stream_read_string(struct string_stream *stream, char *string) {
	int success = sscanf(stream->string, "%s", string);
	if (success != EOF) {
		stream->string += strlen(string);
		ignore_whitespaces(stream);
	}
	return success;
}

void string_stream_close(struct string_stream *stream) {
	free(stream);
}

static void ignore_whitespaces(struct string_stream *stream) {
	while (is_whitespace(stream->string[0])) {
		++stream->string;
	}
}

static int is_whitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n';
}
