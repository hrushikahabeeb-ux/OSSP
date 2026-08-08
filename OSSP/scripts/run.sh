#!/usr/bin/env bash
# Convenience script: build then run my_shell
set -e
make -C "$(dirname "$0")/.."
"$(dirname "$0")/../bin/my_shell"
