#include "../../../php_cryptopp.h"
#include "php_block_cipher.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

/* {{{ block cipher algo list, and corresponding PHP classes */
static vector<string> cipherAlgoList;
static vector<string> cipherClassList;

void addBlockCipherAlgo(const string algoName, const string cipherClassname) {
    cipherAlgoList.push_back(algoName);
    cipherClassList.push_back(cipherClassname);
}

vector<string> getBlockCipherAlgoList() {
    vector<string> _algos(cipherAlgoList);
    sort(_algos.begin(), _algos.end());
    return _algos;
}

string getBlockCipherAlgoClass(const string &algoName) {
    vector<string>::iterator iterator = find(cipherAlgoList.begin(), cipherAlgoList.end(), algoName);

    if (iterator == cipherAlgoList.end()) {
        // block cipher not found
        return "";
    } else {
        // block cipher found
        // return corresponding block cipher classname
        int pos = iterator - cipherAlgoList.begin();
        return cipherClassList[pos];
    }
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_BlockCipher_getAlgos, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_BlockCipher_getClassname, 0)
    ZEND_ARG_INFO(0, algoName)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP class déclaration */
zend_class_entry *cryptopp_ce_BlockCipher;

static zend_function_entry cryptopp_methods_BlockCipher[] = {
    PHP_ME(Cryptopp_BlockCipher, getAlgos, arginfo_BlockCipher_getAlgos, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Cryptopp_BlockCipher, getClassname, arginfo_BlockCipher_getClassname, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_FE_END
};

void init_class_BlockCipher(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "BlockCipher", cryptopp_methods_BlockCipher);
    cryptopp_ce_BlockCipher             = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_BlockCipher->ce_flags  |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
}
/* }}} */

/* {{{ proto array BlockCipher::getAlgos(void)
   Get the list of supported block ciphers */
PHP_METHOD(Cryptopp_BlockCipher, getAlgos) {
    array_init(return_value);
    vector<string> _algos = getBlockCipherAlgoList();

    for (vector<string>::iterator it = _algos.begin(); it != _algos.end(); ++it) {
        add_next_index_string(return_value, it->c_str(), it->length());
    }
}
/* }}} */

/* {{{ proto array BlockCipher::getClassname(string algoName)
   Get the the name of the class that implements a block cipher */
PHP_METHOD(Cryptopp_BlockCipher, getClassname) {
    char *algoName      = NULL;
    int algoNameSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &algoName, &algoNameSize)) {
        return;
    }

    string algoNameStr(algoName, algoNameSize);
    string classname = getBlockCipherAlgoClass(algoNameStr);

    if (classname.empty()) {
        // return NULL if algo not found
        RETURN_NULL()
    } else {
        // return classname
        RETURN_STRING(classname.c_str(), 1)
    }
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