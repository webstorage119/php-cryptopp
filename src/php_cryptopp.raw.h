#ifndef PHP_CRYPTOPP_H
#define PHP_CRYPTOPP_H

#define PHP_CRYPTOPP_EXTNAME "cryptopp"
#define PHP_CRYPTOPP_VERSION "%ext_version%"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptlib.h"

extern zend_module_entry cryptopp_module_entry;
#define phpext_cryptopp_ptr &cryptopp_module_entry;

//%configure_inclusion%

#endif