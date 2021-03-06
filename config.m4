CFLAGS="$CFLAGS -Wall"

PHP_ARG_ENABLE(cryptopp,
    [whether to enable the "crypto++" extension],
    [ --enable-cryptopp Enable "crypto++" extension support])

PHP_ARG_WITH(libcryptopp-dir,
    [for libcryptopp support],
    [ --with-libcryptopp-dir[=DIR] Set the path to libcryptopp install prefix.], yes)

if test $PHP_CRYPTOPP != "no"; then
    dnl search for cryptopp headers
    if test "$PHP_LIBCRYPTOPP_DIR" != "no" && test "$PHP_LIBCRYPTOPP_DIR" != "yes"; then
        # user defined path
        test -f $PHP_LIBCRYPTOPP_DIR/aes.h && CRYPTOPP_DIR=$PHP_LIBCRYPTOPP_DIR
    else
        # try to find it automatically
        for i in /usr/local /usr; do
            test -f $i/include/cryptopp/aes.h && CRYPTOPP_DIR=$i/include/cryptopp && break
        done
    fi

    if test -z "$CRYPTOPP_DIR"; then
        AC_MSG_ERROR(libcryptopp not found.)
    fi

    dnl retrieve cpp file list
    SRC_FILE_LIST=$(/usr/bin/env python ./config.py $CRYPTOPP_DIR)

    if test ! $? -eq 0; then
        AC_MSG_ERROR($SRC_FILE_LIST)
    fi


    PHP_ADD_INCLUDE($CRYPTOPP_DIR)
    PHP_REQUIRE_CXX()
    PHP_SUBST(CRYPTOPP_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, CRYPTOPP_SHARED_LIBADD)
    PHP_ADD_LIBRARY(cryptopp, 1, CRYPTOPP_SHARED_LIBADD)
    PHP_NEW_EXTENSION(cryptopp, $SRC_FILE_LIST, $ext_shared)
fi
