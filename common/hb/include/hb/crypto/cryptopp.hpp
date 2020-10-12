#pragma once

#include <cryptopp/aes.h>
#include <cryptopp/files.h>    // FileSource, FileSink
#include <cryptopp/osrng.h>    // AutoSeededRandomPool
#include <cryptopp/modes.h>    // CFB_Mode
#include <cryptopp/hex.h>      // HexEncoder
#include <cryptopp/base64.h>   // Base64Encoder
#include <cryptopp/base32.h>   // Base64Encoder
#include <cryptopp/gcm.h>      // GCM模式支持
#include <cryptopp/sha.h>      
#include <cryptopp/md5.h>      
#include <cryptopp/rsa.h>      // RSAES_OAEP_SHA_Decryptor
#include <cryptopp/pem.h>
#include <cryptopp/pem_common.h>