<?php

namespace Cryptopp;

// TODO comment
abstract class StreamCipherAbstract implements StreamCipherInterface
{
    /**
     * {@inheritdoc}
     */
    final public function getName() {}

    /**
     * {@inheritdoc}
     */
    final public function getBlockSize() {}

    /**
     * {@inheritdoc}
     */
    final public function isValidKeyLength($keyLength) {}

    /**
     * {@inheritdoc}
     */
    final public function setKey($key) {}

    /**
     * {@inheritdoc}
     */
    final public function setIv($iv) {}

    /**
     * {@inheritdoc}
     */
    final public function encrypt($data) {}

    /**
     * {@inheritdoc}
     */
    final public function decrypt($data) {}

    /**
     * {@inheritdoc}
     */
    final public function restart() {}
}