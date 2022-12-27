.PHONY: amalgamate clean examples
.DEFAULT_GOAL: amalgamate

amalgamate: dose.h

clean:
	rm -f dose.h
	rm -f examples/http_client
	rm -f examples/*.exe

examples: examples/http_client

examples/http_client: examples/http_client.c dose.h
	$(CC) -I. examples/http_client.c -Wall -Wpedantic -o examples/http_client $(CFLAGS)

dose.h: dose_toplevel.h io_impl.h dial_impl.h debug_impl.h
	quom dose_toplevel.h dose.h
