#!/bin/bash
# Copyright 2015 (c) Michael Thomas (malinka) <malinka@entropy-development.com>
# Distributed under the terms of the GNU Affero General Public License v3

mkdir -p build-aux
libtoolize
aclocal -I m4
autoconf
autoheader
automake --add-missing
