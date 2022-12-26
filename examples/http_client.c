#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DOSE_IMPLEMENTATION
// This enables internal log and more detailed error messages to be printed to stderr
#define DOSE_DEBUG
#include "../dose.h"

int main(int argc, char** argv) {
    int sock = dose_dial("tcp!google.com!http");  // Equivalent to "tcp!google.com!80"
    if (sock == -1) {
        fprintf(stderr, "dose_dial() failed!\n");
        exit(EXIT_FAILURE);
    }

    const char request[] = "GET / HTTP/1.0\r\n\r\n";
    if (dose_send(sock, request, strlen(request)) < 0) {
        fprintf(stderr, "dose_send() failed!\n");
        exit(EXIT_FAILURE);
    }

    char* resp = calloc(4096, sizeof(char));
    if (dose_recv(sock, resp, 4096 * sizeof(char)) < 0) {
        fprintf(stderr, "dose_recv() failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Response:\n%s\n", resp);
    free(resp);
}
