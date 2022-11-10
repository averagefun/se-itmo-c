#include "io_utils.h"

#define READ_ERROR_STRING(ERR) "Error while reading from file: " #ERR "!\n"
#define WRITE_ERROR_STRING(ERR) "Error while writing to file: " #ERR "!\n"

char* read_status_msgs_en[] = {
    [READ_OK] = "File readed succsesfully :)\n",
    [READ_INVALID_FILE] = READ_ERROR_STRING("invalid filename"),
    [READ_INVALID_SIGNATURE] = READ_ERROR_STRING("invalid file signature"),
    [READ_INVALID_BITS] = READ_ERROR_STRING("invalid bits"),
    [READ_INVALID_HEADER] = READ_ERROR_STRING("invalid header")};

char* write_status_msgs_en[] = {
    [WRITE_OK] = "File writed succsesfully :)\n",
    [WRITE_INVALID_FILE] = READ_ERROR_STRING("invalid filename"),
    [WRITE_HEADER_FAILED] = READ_ERROR_STRING("invalid image header"),
    [WRITE_DATA_FAILED] = READ_ERROR_STRING("invalid image bitmap data")};

enum read_status read_file(const char* input_path, from_format_reader* reader,
                           struct image* input_img) {
    FILE* input_file = fopen(input_path, "rb");
    if (!input_file) {
        return READ_INVALID_FILE;
    }
    enum read_status read_status = reader(input_file, input_img);
    fclose(input_file);
    return read_status;
}

enum write_status write_file(const char* output_path, to_format_writer* writer,
                             struct image output_img) {
    FILE* output_file = fopen(output_path, "wb");
    if (!output_file) {
        return WRITE_INVALID_FILE;
    }
    enum write_status write_status = writer(output_file, output_img);
    fclose(output_file);
    return write_status;
}
