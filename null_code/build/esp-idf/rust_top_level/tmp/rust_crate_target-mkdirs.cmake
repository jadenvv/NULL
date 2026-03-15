# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/jadenv/Documents/NULL/null_code/components/rust_top_level/rust_crate"
  "/home/jadenv/Documents/NULL/null_code/components/rust_top_level/rust_crate"
  "/home/jadenv/Documents/NULL/null_code/build/esp-idf/rust_top_level"
  "/home/jadenv/Documents/NULL/null_code/build/esp-idf/rust_top_level/tmp"
  "/home/jadenv/Documents/NULL/null_code/build/esp-idf/rust_top_level/stamp"
  "/home/jadenv/Documents/NULL/null_code/build/esp-idf/rust_top_level"
  "/home/jadenv/Documents/NULL/null_code/build/esp-idf/rust_top_level/stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/jadenv/Documents/NULL/null_code/build/esp-idf/rust_top_level/stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/jadenv/Documents/NULL/null_code/build/esp-idf/rust_top_level/stamp${cfgdir}") # cfgdir has leading slash
endif()
