#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// This tells the library this source file is where function definitions should be expanded into.
// Must be set in exactly one source file in your project.
#define DOSE_IMPLEMENTATION
// This enables internal log and more detailed error messages to be printed to stderr
#define DOSE_DEBUG
#include <dose.h>

int main(int argc, char** argv) {
    int sock = dose_dial("tcp!google.com!http");  // Equivalent to "tcp!google.com!80"
    if (sock < 0) {
        char* err_str = dose_geterrstr(sock);
        fprintf(stderr, "dose_dial() failed! Error: %s\n", err_str);
        dose_freeerrstr(err_str);
        exit(EXIT_FAILURE);
    }

    const char request[] = "GET / HTTP/1.0\r\n\r\n";
    if (dose_send(sock, request, strlen(request)) < 0) {
        char* err_str = dose_geterrstr(sock);
        fprintf(stderr, "dose_send() failed! Error: %s\n", err_str);
        dose_freeerrstr(err_str);
        exit(EXIT_FAILURE);
    }

    char* resp = calloc(4096, sizeof(char));
    int err = dose_recv(sock, resp, 4096 * sizeof(char));
    if (err != DOSE_ERR_OK) {
        char* err_str = dose_geterrstr(err);
        fprintf(stderr, "dose_recv() failed! Error: %s\n", err_str);
        dose_freeerrstr(err_str);
        exit(EXIT_FAILURE);
    }

    err = dose_close(sock);
    if (err != DOSE_ERR_OK) {
        char* err_str = dose_geterrstr(err);
        fprintf(stderr, "dose_close() failed! Error: %s\n", err_str);
        dose_freeerrstr(err_str);
        exit(EXIT_FAILURE);
    }

    printf("Response:\n%s\n", resp);
    free(resp);
}
