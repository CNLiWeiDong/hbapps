#include <hb/crypto/sha.hpp>
#include <hb/crypto/cryptopp.hpp>

namespace hb { namespace crypto {
    
    using namespace CryptoPP;

    std::string sha1(const std::string &msg)
    {	
        std::string digest;

        SHA1 hash;
        hash.Update((const byte*)msg.data(), msg.size());
        digest.resize(hash.DigestSize());
        hash.Final((byte*)&digest[0]);

        return digest;
    }
    
    bool sha1_verify(const std::string &msg,const std::string &digest)
    {
        SHA1 hash;
        return hash.VerifyDigest( (byte*)digest.c_str(), (const byte *)msg.c_str(), msg.size() );
    }

    std::string sha256(const std::string &msg)
    {	
        std::string digest;

        SHA256 hash;
        hash.Update((const byte*)msg.data(), msg.size());
        digest.resize(hash.DigestSize());
        hash.Final((byte*)&digest[0]);

        return digest;
    }
    
    bool sha256_verify(const std::string &msg,const std::string &digest)
    {
        SHA256 hash;
        return hash.VerifyDigest( (byte*)digest.c_str(), (const byte *)msg.c_str(), msg.size() );
    }

    std::string sha512(const std::string &msg)
    {	
        std::string digest;

        SHA512 hash;
        hash.Update((const byte*)msg.data(), msg.size());
        digest.resize(hash.DigestSize());
        hash.Final((byte*)&digest[0]);

        return digest;
    }
    
    bool sha512_verify(const std::string &msg,const std::string &digest)
    {
        SHA512 hash;
        return hash.VerifyDigest( (byte*)digest.c_str(), (const byte *)msg.c_str(), msg.size() );
    }

} } // namespace hb
