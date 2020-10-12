#include <hb/crypto/hex.hpp>
#include <hb/crypto/cryptopp.hpp>

namespace hb { namespace crypto {
    
    using namespace CryptoPP;

    std::string hex(const std::string &str)
    {
        std::string encoded;
        StringSource ss(str, true, new HexEncoder(new StringSink(encoded)));	
        return encoded;
    }

    std::string hex_str(const std::string &hex_str) {
        std::string decoded;
        StringSource ssk(hex_str, true /*pump all*/,
            new HexDecoder(
                new StringSink(decoded)
            ) // HexDecoder
        );
        return decoded;
    }
} } // namespace hb
