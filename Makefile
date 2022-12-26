.PHONY: amalgamate clean examples
.DEFAULT_GOAL: amalgamate

amalgamate: dose.h

clean:
	rm dose.h
	rm examples/http_client

examples: examples/http_client

examples/http_client: examples/http_client.c dose.h
	$(CC) -I. examples/http_client.c -o examples/http_client

dose.h: dose_toplevel.h io_impl.h dial_impl.h debug_impl.h
	quom dose_toplevel.h dose.h
