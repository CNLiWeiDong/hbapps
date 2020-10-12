#include <hb/crypto/md5.hpp>
#include <hb/crypto/cryptopp.hpp>

namespace hb { namespace crypto {
    
    using namespace CryptoPP;

    std::string md5(const std::string &msg)
    {	
        std::string digest;

        MD5 hash;
        hash.Update((const byte*)msg.data(), msg.size());
        digest.resize(hash.DigestSize());
        hash.Final((byte*)&digest[0]);

        return digest;
    }
    
    bool md5_verify(const std::string &msg,const std::string &digest)
    {
        MD5 hash;
        return hash.VerifyDigest( (byte*)digest.c_str(), (const byte *)msg.c_str(), msg.size() );
    }

} } // namespace hb
