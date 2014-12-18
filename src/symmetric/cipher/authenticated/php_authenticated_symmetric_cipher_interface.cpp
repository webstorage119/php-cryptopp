#include "../../../php_cryptopp.h"
#include "../../../mac/php_mac_interface.h"
#include "../php_symmetric_transformation_interface.h"
#include "php_authenticated_symmetric_cipher_interface.h"

/* {{{ PHP interface declaration */
zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherInterface;

static zend_function_entry cryptopp_methods_AuthenticatedSymmetricCipherInterface[] = {
    PHP_FE_END
};

void init_interface_AuthenticatedSymmetricCipherInterface(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "AuthenticatedSymmetricCipherInterface", cryptopp_methods_AuthenticatedSymmetricCipherInterface);
    cryptopp_ce_AuthenticatedSymmetricCipherInterface = zend_register_internal_interface(&ce TSRMLS_CC);

    zend_class_implements(cryptopp_ce_AuthenticatedSymmetricCipherInterface TSRMLS_CC, 1, cryptopp_ce_SymmetricTransformationInterface);
    zend_class_implements(cryptopp_ce_AuthenticatedSymmetricCipherInterface TSRMLS_CC, 1, cryptopp_ce_MacInterface);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 expandtab fdm=marker
 * vim<600: sw=4 ts=4 expandtab
 */