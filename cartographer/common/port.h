
/*
  common/port.h文件主要实现2大功能：
  1，使用std::lround对浮点数进行四舍五入取整运算
  2，利用boost的iostreams/filter/gzip对字符串压缩与解压缩
*/

#ifndef CARTOGRAPHER_COMMON_PORT_H_
#define CARTOGRAPHER_COMMON_PORT_H_

#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/gzip.hpp>//包含多种解压与压缩算法
#include <boost/iostreams/filtering_stream.hpp>//配合filter实现流过滤
#include <cinttypes>
#include <cmath>
#include <string>

namespace cartographer {

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

namespace common {
//四舍五入取整
inline int RoundToInt(const float x) { return std::lround(x); }

inline int RoundToInt(const double x) { return std::lround(x); }

inline int64 RoundToInt64(const float x) { return std::lround(x); }

inline int64 RoundToInt64(const double x) { return std::lround(x); }
//压缩字符串string
//利用gzip_compressor对string进行压缩，第一参数是未压缩string，第二参数是完成压缩string
inline void FastGzipString(const std::string& uncompressed,
                           std::string* compressed) {
  boost::iostreams::filtering_ostream out;//创建过滤流
  out.push(
      boost::iostreams::gzip_compressor(boost::iostreams::zlib::best_speed));//使用快速压缩算法
  out.push(boost::iostreams::back_inserter(*compressed));//对compressed 使用后插迭代器
  boost::iostreams::write(out,
                          reinterpret_cast<const char*>(uncompressed.data()),
                          uncompressed.size());//压缩 char *,插入compressed
}
/*
利用gzip_decompressor解压缩string，第一参数是待解压的string，第二参数是解压后的string
*/
inline void FastGunzipString(const std::string& compressed,
                             std::string* decompressed) {
  boost::iostreams::filtering_ostream out;//创建过滤流
  out.push(boost::iostreams::gzip_decompressor());//指定解压缩算法
  out.push(boost::iostreams::back_inserter(*decompressed));//对decompressed使用后插入迭代器
  boost::iostreams::write(out, reinterpret_cast<const char*>(compressed.data()),
                          compressed.size());//解压缩char*，插入decompressed
}

}  // namespace common
}  // namespace cartographer

#endif  // CARTOGRAPHER_COMMON_PORT_H_
