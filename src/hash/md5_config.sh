#!/bin/bash

test ! -f "$CRYPTOPP_DIR/md5.h" && echo "DISABLED" && exit 0

echo "src/hash/md5.cpp"
echo "hash/php_md5.h"
echo "PHP_FE(cryptopp_md5, NULL)"

