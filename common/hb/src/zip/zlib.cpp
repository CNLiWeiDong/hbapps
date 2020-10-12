#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <hb/zip/zlib.hpp>

namespace hb { namespace zip {  

namespace bio = boost::iostreams;
hb::type::bytes zlib_compress(const hb::type::bytes &in) {
  hb::type::bytes out;
  bio::filtering_ostream comp;
  comp.push(bio::zlib_compressor(bio::zlib::default_compression));
  comp.push(bio::back_inserter(out));
  bio::write(comp, in.data(), in.size());
  bio::close(comp);
  return out;
}

hb::type::bytes zlib_decompress(const hb::type::bytes &in) {
  hb::type::bytes out;
  bio::filtering_ostream decomp;
  decomp.push(bio::zlib_decompressor());
  decomp.push(bio::back_inserter(out));
  bio::write(decomp, in.data(), in.size());
  bio::close(decomp);
  return out;
}
} } // namespace hb
