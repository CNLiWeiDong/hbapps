#include <hb/crypto/aes.hpp>
#include <hb/crypto/cryptopp.hpp>

namespace hb { namespace crypto {

    using namespace CryptoPP;

    std::string cfb_aes_encrypt(const std::string &sKey, const std::string &plainText) 
    {
        std::string outstr;
    
        //填key
        SecByteBlock key(AES::DEFAULT_KEYLENGTH);
        memset(key,0x30,key.size() );
        sKey.size()<=AES::DEFAULT_KEYLENGTH?memcpy(key,sKey.c_str(),sKey.size()):memcpy(key,sKey.c_str(),AES::DEFAULT_KEYLENGTH);
        
        //填iv
        byte iv[AES::BLOCKSIZE];
        memset(iv,0x30,AES::BLOCKSIZE);
    
        AES::Encryption aesEncryption((byte *)key, AES::DEFAULT_KEYLENGTH);
    
        CFB_Mode_ExternalCipher::Encryption cfbEncryption(aesEncryption, iv);
    
        StreamTransformationFilter cfbEncryptor(cfbEncryption, new HexEncoder(new StringSink(outstr)));
        cfbEncryptor.Put((byte *)plainText.data(), plainText.size());
        cfbEncryptor.MessageEnd();
    
        return outstr;
    }

    std::string cfb_aes_encrypt(const std::string &plainText) 
    {
        return cfb_aes_encrypt("", plainText);
    }

    std::string cfb_aes_decrypt(const std::string &sKey, const std::string &cipherText)
    {
        std::string outstr;
    
        //填key
        SecByteBlock key(AES::DEFAULT_KEYLENGTH);
        memset(key,0x30,key.size() );
        sKey.size()<=AES::DEFAULT_KEYLENGTH?memcpy(key,sKey.c_str(),sKey.size()):memcpy(key,sKey.c_str(),AES::DEFAULT_KEYLENGTH);
        
        //填iv
        byte iv[AES::BLOCKSIZE];
        memset(iv,0x30,AES::BLOCKSIZE);
    
        CFB_Mode<AES >::Decryption cfbDecryption((byte *)key, AES::DEFAULT_KEYLENGTH, iv);
        
        HexDecoder decryptor(new StreamTransformationFilter(cfbDecryption, new StringSink(outstr)));
        decryptor.Put((byte *)cipherText.data(), cipherText.size());
        decryptor.MessageEnd();
    
        return outstr;
    }
    std::string cfb_aes_decrypt(const std::string &cipherText)
    {
        return cfb_aes_decrypt("", cipherText);
    }

} } // namespace hb
