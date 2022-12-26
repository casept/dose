#!/usr/bin/env bash
set -eo pipefail

# For now, just very crudely test whether examples build and run
make examples
./examples/http_client
