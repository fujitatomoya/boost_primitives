#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstddef>
#include <cstdlib>

namespace bip = boost::interprocess;
namespace fs = boost::filesystem;
using intAlloc = bip::allocator<int, bip::managed_mapped_file::segment_manager>;
using intVec = std::vector<int, intAlloc>;

int main(int argc, char *argv[])
{
  // Define file names
  const char *fileName  = "managed_mapped_file.bin";
  const std::size_t fileSize = 10000;
  bip::file_mapping::remove(fileName);

  // Remove on exit
  struct file_remove
  {
    file_remove(const char *fileName)
      : fileName_(fileName) {}
    ~file_remove() { bip::file_mapping::remove(fileName_); }
    const char *fileName_;
  } remover(fileName);

  // Create managed_mapped_file and push some contents.
  bip::managed_mapped_file file_vec(bip::create_only, fileName, fileSize);
  auto *vecObj = file_vec.construct<intVec>("myVecName")(file_vec.get_allocator<int>());
  for (size_t i = 0; i < 10; i++) {
    vecObj->push_back(i + 100);
  }
  for (auto it = vecObj->begin(); it != vecObj->end(); it++) {
    std::cout << ' ' << *it;
  }
  std::cout << '\n';

  // View mapped region.
  std::cout << "1: address " << vecObj << ", size " << fs::file_size(fileName) << '\n';

  // Shrink file size. (should be beyond page size at least)
  bip::managed_mapped_file::shrink_to_fit(fileName);

  // View mapped region.
  std::cout << "2: address " << vecObj << ", size " << fs::file_size(fileName) << " (after shrink)\n";

  // Shrink file size. (should be beyond page size at least)
  bip::managed_mapped_file::grow(fileName, (fileSize * 4));

  // View mapped region.
  std::cout << "3: address " << vecObj << ", size " << fs::file_size(fileName) << " (after grow)\n";

  return EXIT_SUCCESS;
}
