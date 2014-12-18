#include "../../../php_cryptopp.h"
#include "../../../exception/php_exception.h"
#include "../../../hash/php_hash_transformation_interface.h"
#include "../../../mac/php_mac_interface.h"
#include "../php_symmetric_cipher_interface.h"
#include "php_authenticated_symmetric_cipher.h"
#include "php_authenticated_symmetric_cipher_interface.h"
#include "php_authenticated_symmetric_cipher_abstract.h"
#include <zend_exceptions.h>
#include <string>

// TODO is decryptor necessary??

/* {{{ custom object create/free handler */
zend_object_handlers AuthenticatedSymmetricCipherAbstract_object_handlers;

void AuthenticatedSymmetricCipherAbstract_free_storage(void *object TSRMLS_DC) {
    AuthenticatedSymmetricCipherAbstractContainer *obj = static_cast<AuthenticatedSymmetricCipherAbstractContainer *>(object);
    delete obj->encryptor;
    delete obj->decryptor;
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value AuthenticatedSymmetricCipherAbstract_create_handler(zend_class_entry *type TSRMLS_DC) {
    zend_object_value retval;

    AuthenticatedSymmetricCipherAbstractContainer *obj = static_cast<AuthenticatedSymmetricCipherAbstractContainer *>(emalloc(sizeof(AuthenticatedSymmetricCipherAbstractContainer)));
    memset(obj, 0, sizeof(AuthenticatedSymmetricCipherAbstractContainer));

    zend_object_std_init(&obj->std, type TSRMLS_CC);

    #if PHP_VERSION_ID < 50399
        zend_hash_copy(obj->std.properties, &type->properties_info, (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));
    #else
        object_properties_init(static_cast<zend_object*>(&(obj->std)), type);
    #endif

    retval.handle   = zend_objects_store_put(obj, NULL, AuthenticatedSymmetricCipherAbstract_free_storage, NULL TSRMLS_CC);
    retval.handlers = &AuthenticatedSymmetricCipherAbstract_object_handlers;

    return retval;
}
/* }}} */

/* {{{ PHP abstract class declaration */
zend_class_entry *cryptopp_ce_AuthenticatedSymmetricCipherAbstract;

static zend_function_entry cryptopp_methods_AuthenticatedSymmetricCipherAbstract[] = {
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, __sleep, arginfo_AuthenticatedSymmetricCipherAbstract___sleep, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, __wakeup, arginfo_AuthenticatedSymmetricCipherAbstract___wakeup, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, getName, arginfo_SymmetricCipherInterface_getName, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, getBlockSize, arginfo_SymmetricCipherInterface_getBlockSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, getDigestSize, arginfo_HashTransformationInterface_getDigestSize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, isValidKeyLength, arginfo_SymmetricCipherInterface_isValidKeyLength, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, setKey, arginfo_SymmetricCipherInterface_setKey, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, setIv, arginfo_SymmetricTransformationInterface_setIv, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, encrypt, arginfo_SymmetricTransformationInterface_encrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, decrypt, arginfo_SymmetricTransformationInterface_decrypt, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, calculateDigest, arginfo_HashTransformationInterface_calculateDigest, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, update, arginfo_HashTransformationInterface_update, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, finalize, arginfo_HashTransformationInterface_finalize, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_ME(Cryptopp_AuthenticatedSymmetricCipherAbstract, restart, arginfo_SymmetricTransformationInterface_restart, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
    PHP_FE_END
};

void init_class_AuthenticatedSymmetricCipherAbstract(TSRMLS_D) {
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Cryptopp", "AuthenticatedSymmetricCipherAbstract", cryptopp_methods_AuthenticatedSymmetricCipherAbstract);
    cryptopp_ce_AuthenticatedSymmetricCipherAbstract            = zend_register_internal_class(&ce TSRMLS_CC);
    cryptopp_ce_AuthenticatedSymmetricCipherAbstract->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

    cryptopp_ce_AuthenticatedSymmetricCipherAbstract->create_object = AuthenticatedSymmetricCipherAbstract_create_handler;
    memcpy(&AuthenticatedSymmetricCipherAbstract_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    AuthenticatedSymmetricCipherAbstract_object_handlers.clone_obj = NULL;

    zend_class_implements(cryptopp_ce_AuthenticatedSymmetricCipherAbstract TSRMLS_CC, 1, cryptopp_ce_AuthenticatedSymmetricCipherInterface);

    zend_declare_property_string(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, "name", 4, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, "key", 3, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_string(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, "iv", 2, "",  ZEND_ACC_PRIVATE TSRMLS_CC);
}
/* }}} */

/* {{{ inits a child class */
void init_class_AuthenticatedSymmetricCipherAbstractChild(const char *algoName, const char* className, zend_class_entry **classEntryPtr, zend_function_entry *classMethods TSRMLS_DC) {
    std::string namespacedClassName("Cryptopp\\");
    namespacedClassName.append(className);

    zend_class_entry ce;
    INIT_CLASS_ENTRY_EX(ce, namespacedClassName.c_str(), namespacedClassName.length(), classMethods);
    *classEntryPtr = zend_register_internal_class_ex(&ce, cryptopp_ce_AuthenticatedSymmetricCipherAbstract, NULL TSRMLS_CC);

    addAuthenticatedSymmetricCipherAlgo(algoName, namespacedClassName.c_str());
}
/* }}} */

/* {{{ get the pointer to the native encryptor object of a php cipher class */
CryptoPP::AuthenticatedSymmetricCipher *getCryptoppAuthenticatedSymmetricCipherEncryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    encryptor = static_cast<AuthenticatedSymmetricCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor;

    if (NULL == encryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherAbstract cannot be extended by user classes");
    }

    return encryptor;
}
/* }}} */

/* {{{ get the pointer to the native decryptor object of a php cipher class */
CryptoPP::AuthenticatedSymmetricCipher *getCryptoppAuthenticatedSymmetricCipherDecryptorPtr(zval *this_ptr TSRMLS_DC) {
    CryptoPP::AuthenticatedSymmetricCipher *decryptor;
    decryptor = static_cast<AuthenticatedSymmetricCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor;

    if (NULL == decryptor) {
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"Cryptopp\\AuthenticatedSymmetricCipherAbstract cannot be extended by user classes");
    }

    return decryptor;
}
/* }}} */

/* {{{ set the pointer to the native encryptor object of a php cipher class */
void setCryptoppAuthenticatedSymmetricCipherEncryptorPtr(zval *this_ptr, CryptoPP::AuthenticatedSymmetricCipher *encryptorPtr TSRMLS_DC) {
    static_cast<AuthenticatedSymmetricCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->encryptor = encryptorPtr;
}
/* }}} */

/* {{{ set the pointer to the native decryptor object of a php cipher class */
void setCryptoppAuthenticatedSymmetricCipherDecryptorPtr(zval *this_ptr, CryptoPP::AuthenticatedSymmetricCipher *decryptorPtr TSRMLS_DC) {
    static_cast<AuthenticatedSymmetricCipherAbstractContainer *>(zend_object_store_get_object(this_ptr TSRMLS_CC))->decryptor = decryptorPtr;
}
/* }}} */

/* {{{ verify that a key size is valid for an AuthenticatedSymmetricCipherAbstract instance */
static bool isCryptoppAuthenticatedSymmetricCipherKeyValid(zval *object, CryptoPP::AuthenticatedSymmetricCipher *cipher, int keySize) {
    zend_class_entry *ce;
    ce = zend_get_class_entry(object TSRMLS_CC);

    if (!cipher->IsValidKeyLength(keySize)) {
        if (0 == keySize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : a key is required", ce->name, keySize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid key length", ce->name, keySize);
        }

        return false;
    }

    return true;
}

bool isCryptoppAuthenticatedSymmetricCipherKeyValid(zval *object, CryptoPP::AuthenticatedSymmetricCipher *cipher) {
    zval *key;
    key         = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, object, "key", 3, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(key);

    return isCryptoppAuthenticatedSymmetricCipherKeyValid(object, cipher, keySize);
}
/* }}} */

/* {{{ verify that an iv size is valid for an AuthenticatedSymmetricCipherAbstract instance */
static bool isCryptoppAuthenticatedSymmetricCipherIvValid(zval *object, CryptoPP::AuthenticatedSymmetricCipher *cipher, int ivSize) {
    zend_class_entry *ce;
    ce = zend_get_class_entry(object TSRMLS_CC);

    if (cipher->IsResynchronizable() && cipher->IVSize() != ivSize) {
        if (0 == ivSize) {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : an initialization vector is required", ce->name, ivSize);
        } else {
            zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s : %d is not a valid initialization vector length", ce->name, ivSize);
        }

        return false;
    }

    return true;
}

bool isCryptoppAuthenticatedSymmetricCipherIvValid(zval *object, CryptoPP::AuthenticatedSymmetricCipher *cipher) {
    zval *iv;
    iv          = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, object, "iv", 2, 1 TSRMLS_CC);
    int ivSize  = Z_STRLEN_P(iv);

    return isCryptoppAuthenticatedSymmetricCipherIvValid(object, cipher, ivSize);
}
/* }}} */

/* {{{ sets the key and the iv (if applicable) of the native cipher objects of a cipher php object */
static void setKeyWithIv(zval *object, CryptoPP::AuthenticatedSymmetricCipher *encryptor, CryptoPP::AuthenticatedSymmetricCipher *decryptor) {
    // get key and iv of the php object
    zval *zKey;
    zval *zIv;
    zKey        = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, object, "key", 3, 1 TSRMLS_CC);
    zIv         = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, object, "iv", 2, 1 TSRMLS_CC);
    int keySize = Z_STRLEN_P(zKey);
    int ivSize  = Z_STRLEN_P(zIv);

    // set the key and the iv (if applicable) of native cipher objects
    if (keySize > 0 && !encryptor->IsResynchronizable()) {
        // an iv is not required
        // set key
        byte *key;
        key = reinterpret_cast<byte*>(Z_STRVAL_P(zKey));
        encryptor->SetKey(key, keySize);
        decryptor->SetKey(key, keySize);
    } else if (keySize > 0 && ivSize > 0 && encryptor->IsResynchronizable()) {
        // set key and iv
        byte *key;
        byte *iv;
        key = reinterpret_cast<byte*>(Z_STRVAL_P(zKey));
        iv  = reinterpret_cast<byte*>(Z_STRVAL_P(zIv));
        encryptor->SetKeyWithIV(key, keySize, iv, ivSize);
        decryptor->SetKeyWithIV(key, keySize, iv, ivSize);
    }
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricCipherAbstract::__sleep(void)
   Prevents serialization of an AuthenticatedSymmetricCipherAbstract instance */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, __sleep) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\AuthenticatedSymmetricCipherAbstract instances");
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricCipherAbstract::__wakeup(void)
   Prevents use of an AuthenticatedSymmetricCipherAbstract instance that has been unserialized */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, __wakeup) {
    zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"You cannot serialize or unserialize Cryptopp\\AuthenticatedSymmetricCipherAbstract instances");
}
/* }}} */

/* {{{ proto string AuthenticatedSymmetricCipherAbstract::getName(void)
   Return algorithm name */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, getName) {
    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    zval *name;
    name = zend_read_property(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "name", 4, 0 TSRMLS_CC);
    RETURN_ZVAL(name, 1, 0);
}
/* }}} */

/* {{{ proto int AuthenticatedSymmetricCipherAbstract::getBlockSize(void)
   Returns the block size */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, getBlockSize) {
    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    RETURN_LONG(encryptor->MandatoryBlockSize())
}
/* }}} */

/* {{{ proto int AuthenticatedSymmetricCipherAbstract::getDigestSize(void)
   Returns the digest size */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, getDigestSize) {
    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor)

    unsigned int digestSize = encryptor->DigestSize();
    RETURN_LONG(digestSize);
}
/* }}} */

/* {{{ proto bool AuthenticatedSymmetricCipherAbstract::isValidKeyLength(void)
   Indicates if a key length is valid */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, isValidKeyLength) {
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &keySize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);

    if (encryptor->IsValidKeyLength(keySize)) {
        RETURN_TRUE
    } else {
        RETURN_FALSE
    }
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricCipherAbstract::setKey(string key)
   Sets the key */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, setKey) {
    char *key   = NULL;
    int keySize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &keySize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    CryptoPP::AuthenticatedSymmetricCipher *decryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!isCryptoppAuthenticatedSymmetricCipherKeyValid(getThis(), encryptor, keySize)) {
        RETURN_FALSE;
    }

    // set the key on both the php object and the native cryptopp object
    zend_update_property_stringl(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "key", 3, key, keySize TSRMLS_CC);
    setKeyWithIv(getThis(), encryptor, decryptor);
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricCipherAbstract::setIv(string iv)
   Sets the initialization vector */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, setIv) {
    char *iv    = NULL;
    int ivSize  = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &iv, &ivSize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    CryptoPP::AuthenticatedSymmetricCipher *decryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    if (!encryptor->IsResynchronizable()) {
        // an initialization vector is not required
        return;
    } else if (!isCryptoppAuthenticatedSymmetricCipherIvValid(getThis(), encryptor, ivSize)) {
        // invalid iv
        RETURN_FALSE;
    }

    // set the iv on both the php object and the native cryptopp object
    zend_update_property_stringl(cryptopp_ce_AuthenticatedSymmetricCipherAbstract, getThis(), "iv", 2, iv, ivSize TSRMLS_CC);
    setKeyWithIv(getThis(), encryptor, decryptor);
}
/* }}} */

/* {{{ proto string AuthenticatedSymmetricCipherAbstract::encrypt(string data)
   Encrypts data */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, encrypt) {
    // TODO
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);

    // check key and iv
    if (!isCryptoppAuthenticatedSymmetricCipherKeyValid(getThis(), encryptor) || !isCryptoppAuthenticatedSymmetricCipherIvValid(getThis(), encryptor)) {
        RETURN_FALSE
    }

    // check dataSize against block size
    int blockSize = static_cast<int>(encryptor->MandatoryBlockSize());

    if (0 != dataSize % blockSize) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not a multiple of block size (%d)", ce->name, dataSize, blockSize);
        RETURN_FALSE
    }

    // encrypt
    byte output[dataSize];
    encryptor->ProcessData(output, reinterpret_cast<byte*>(data), dataSize);

    RETURN_STRINGL(reinterpret_cast<char*>(output), dataSize, 1)
}
/* }}} */

/* {{{ proto string AuthenticatedSymmetricCipherAbstract::decrypt(string data)
   Decrypts data */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, decrypt) {
    // TODO
    char *data      = NULL;
    int dataSize    = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &dataSize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *decryptor;
    decryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);

    // check key and iv
    if (!isCryptoppAuthenticatedSymmetricCipherKeyValid(getThis(), decryptor) || !isCryptoppAuthenticatedSymmetricCipherIvValid(getThis(), decryptor)) {
        RETURN_FALSE
    }

    // check dataSize against block size
    int blockSize = static_cast<int>(decryptor->MandatoryBlockSize());

    if (0 != dataSize % blockSize) {
        zend_class_entry *ce;
        ce  = zend_get_class_entry(getThis() TSRMLS_CC);
        zend_throw_exception_ex(getCryptoppException(), 0 TSRMLS_CC, (char*)"%s: data size (%d) is not a multiple of block size (%d)", ce->name, dataSize, blockSize);
        RETURN_FALSE
    }

    // encrypt
    byte output[dataSize];
    decryptor->ProcessData(output, reinterpret_cast<byte*>(data), dataSize);

    RETURN_STRINGL(reinterpret_cast<char*>(output), dataSize, 1)
}
/* }}} */

/* {{{ proto string AuthenticatedSymmetricCipherAbstract::calculateDigest(string data)
   Calculate the MAC */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, calculateDigest) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(encryptor)

    if (!isCryptoppAuthenticatedSymmetricCipherKeyValid(getThis(), encryptor)) {
        RETURN_FALSE;
    }

    byte digest[encryptor->DigestSize()];

    try {
        encryptor->CalculateDigest(digest, reinterpret_cast<byte*>(msg), msgSize);
    } catch (bool e) {
        RETURN_FALSE;
    }

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), encryptor->DigestSize(), 1);
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricCipherAbstract::update(string data)
   Adds data to the current incremental MAC */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, update) {
    char *msg   = NULL;
    int msgSize = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &msg, &msgSize)) {
        return;
    }

    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(encryptor)

    if (!isCryptoppMacKeyValid(getThis(), encryptor)) {
        RETURN_FALSE;
    }

    encryptor->Update(reinterpret_cast<byte*>(msg), msgSize);
}
/* }}} */

/* {{{ proto string AuthenticatedSymmetricCipherAbstract::finalize(void)
   Finalize an incremental MAC and return resulting digest */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, finalize) {
    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(encryptor)

    if (!isCryptoppMacKeyValid(getThis(), encryptor)) {
        RETURN_FALSE;
    }

    byte digest[encryptor->DigestSize()];

    try {
        encryptor->Final(digest);
    } catch (bool e) {
        RETURN_FALSE;
    }

    RETVAL_STRINGL(reinterpret_cast<char*>(digest), encryptor->DigestSize(), 1);
}
/* }}} */

/* {{{ proto void AuthenticatedSymmetricCipherAbstract::restart()
   Reset the initialization vector to its initial state (the one passed in setIv()) */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, restart) {
    // TODO
    CryptoPP::AuthenticatedSymmetricCipher *encryptor;
    CryptoPP::AuthenticatedSymmetricCipher *decryptor;
    encryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_ENCRYPTOR_PTR(encryptor);
    decryptor = CRYPTOPP_AUTHENTICATED_SYMMETRIC_CIPHER_ABSTRACT_GET_DECRYPTOR_PTR(decryptor);
    setKeyWithIv(getThis(), encryptor, decryptor);
}
/* }}} */

/* TODO todel */
PHP_METHOD(Cryptopp_AuthenticatedSymmetricCipherAbstract, verify) {

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