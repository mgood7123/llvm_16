#ifndef __TEST_ARCHIVE_H__
#define __TEST_ARCHIVE_H__

#include <stdint.h>

#define ARCHIVE_COUNT 3
#define FILE_COUNT 2

static const unsigned char _archive_buffers[ARCHIVE_COUNT][648] =
{
    {
        0x5a, 0x50, 0x4b, 0x15, 0x01, 0x00, 0x5a, 0x50, 0x4b, 0x14, 0x49, 0x20, 0x61, 0x6d, 0x20, 0x61,
        0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x21, 0x0a, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x0a, 0x49, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x6e, 0x65, 0x76, 0x65, 0x72, 0x20,
        0x62, 0x65, 0x20, 0x61, 0x6e, 0x79, 0x74, 0x68, 0x69, 0x6e, 0x67, 0x20, 0x6d, 0x6f, 0x72, 0x65,
        0x20, 0x74, 0x68, 0x61, 0x6e, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x66, 0x69, 0x6c,
        0x65, 0x2e, 0x0a, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x0a,
        0x5a, 0x50, 0x4b, 0x13, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x66, 0x69, 0x6c, 0x65, 0x31, 0x2e, 0x74, 0x78, 0x74, 0x0a,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7d, 0xb0, 0x02, 0x7d, 0xa4, 0xcb, 0x74, 0x78, 0x00,
        0x09, 0x00, 0x66, 0x69, 0x6c, 0x65, 0x32, 0x2e, 0x74, 0x78, 0x74, 0xb3, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x5d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5d, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x52, 0x8e, 0xdd, 0x24, 0x0f, 0x5c, 0xf2, 0x15, 0x00, 0x5a, 0x50, 0x4b, 0x12,
        0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    },
    {
        0x5a, 0x50, 0x4b, 0x15, 0x01, 0x00, 0x5a, 0x50, 0x4b, 0x14, 0x28, 0xb5, 0x2f, 0xfd, 0x00, 0x58,
        0xf5, 0x00, 0x00, 0xb0, 0x49, 0x20, 0x61, 0x6d, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20,
        0x66, 0x69, 0x6c, 0x65, 0x21, 0x0a, 0x30, 0x31, 0x32, 0x0a, 0x03, 0x14, 0x00, 0x25, 0x8a, 0xb7,
        0x61, 0x28, 0xb5, 0x2f, 0xfd, 0x00, 0x58, 0xfd, 0x01, 0x00, 0x44, 0x03, 0x49, 0x20, 0x77, 0x69,
        0x6c, 0x6c, 0x20, 0x6e, 0x65, 0x76, 0x65, 0x72, 0x20, 0x62, 0x65, 0x20, 0x61, 0x6e, 0x79, 0x74,
        0x68, 0x69, 0x6e, 0x67, 0x20, 0x6d, 0x6f, 0x72, 0x65, 0x20, 0x74, 0x68, 0x61, 0x6e, 0x20, 0x61,
        0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x2e, 0x0a, 0x30, 0x31, 0x32, 0x0a,
        0x03, 0x14, 0x00, 0x2a, 0x40, 0xc0, 0x0d, 0x00, 0x05, 0x5a, 0x50, 0x4b, 0x13, 0x02, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x66,
        0x69, 0x6c, 0x65, 0x31, 0x2e, 0x74, 0x78, 0x74, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x7d, 0xb0, 0x02, 0x7d, 0xa4, 0xcb, 0x74, 0x78, 0x01, 0x09, 0x00, 0x66, 0x69, 0x6c, 0x65, 0x32,
        0x2e, 0x74, 0x78, 0x74, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x5d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x8e, 0xdd, 0x24,
        0x0f, 0x5c, 0xf2, 0x15, 0x01, 0x5a, 0x50, 0x4b, 0x12, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00
    },
    {
        0x5a, 0x50, 0x4b, 0x15, 0x01, 0x00, 0x5a, 0x50, 0x4b, 0x14, 0x04, 0x22, 0x4d, 0x18, 0x40, 0x40,
        0xc0, 0x28, 0x00, 0x00, 0x00, 0xff, 0x04, 0x49, 0x20, 0x61, 0x6d, 0x20, 0x61, 0x20, 0x74, 0x65,
        0x73, 0x74, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x21, 0x0a, 0x30, 0x01, 0x00, 0x1e, 0x1f, 0x31, 0x01,
        0x00, 0x1e, 0x1f, 0x32, 0x01, 0x00, 0x1a, 0x50, 0x32, 0x32, 0x32, 0x32, 0x0a, 0x00, 0x00, 0x00,
        0x00, 0x04, 0x22, 0x4d, 0x18, 0x40, 0x40, 0xc0, 0x46, 0x00, 0x00, 0x00, 0xff, 0x22, 0x49, 0x20,
        0x77, 0x69, 0x6c, 0x6c, 0x20, 0x6e, 0x65, 0x76, 0x65, 0x72, 0x20, 0x62, 0x65, 0x20, 0x61, 0x6e,
        0x79, 0x74, 0x68, 0x69, 0x6e, 0x67, 0x20, 0x6d, 0x6f, 0x72, 0x65, 0x20, 0x74, 0x68, 0x61, 0x6e,
        0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x2e, 0x0a, 0x30, 0x01,
        0x00, 0x50, 0x1f, 0x31, 0x01, 0x00, 0x50, 0x1f, 0x32, 0x01, 0x00, 0x4c, 0x50, 0x32, 0x32, 0x32,
        0x32, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x5a, 0x50, 0x4b, 0x13, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x66, 0x69, 0x6c, 0x65,
        0x31, 0x2e, 0x74, 0x78, 0x74, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7d, 0xb0, 0x02,
        0x7d, 0xa4, 0xcb, 0x74, 0x78, 0x02, 0x09, 0x00, 0x66, 0x69, 0x6c, 0x65, 0x32, 0x2e, 0x74, 0x78,
        0x74, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x5d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x8e, 0xdd, 0x24, 0x0f, 0x5c, 0xf2,
        0x15, 0x02, 0x5a, 0x50, 0x4b, 0x12, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
};

static const char* _archive_names[ARCHIVE_COUNT] = {
    "archive_none.zpk",
    "archive_zstd.zpk",
    "archive_lz4.zpk"
};

static const int _archive_sizes[ARCHIVE_COUNT] = {
    648, 241, 270
};

static char* _filenames[FILE_COUNT] = {
    "file1.txt",
    "file2.txt"
};

static const int _uncomp_sizes[FILE_COUNT] = {
    169, 349
};

static const uint64_t _hashes[FILE_COUNT] = {
    0x7874cba47d02b07dL,
    0x15f25c0f24dd8e52L
};


static const unsigned char _files[FILE_COUNT][350] = {
    {
        0x49, 0x20, 0x61, 0x6d, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x66, 0x69, 0x6c,
        0x65, 0x21, 0x0a, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x0a
    },
    {
        0x49, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x6e, 0x65, 0x76, 0x65, 0x72, 0x20, 0x62, 0x65,
        0x20, 0x61, 0x6e, 0x79, 0x74, 0x68, 0x69, 0x6e, 0x67, 0x20, 0x6d, 0x6f, 0x72, 0x65, 0x20,
        0x74, 0x68, 0x61, 0x6e, 0x20, 0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x66, 0x69, 0x6c,
        0x65, 0x2e, 0x0a, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
        0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
        0x32, 0x32, 0x32, 0x0a
    }
};

#endif // __TEST_ARCHIVE_H__