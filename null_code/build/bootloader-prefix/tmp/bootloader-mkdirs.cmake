# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/jadenv/esp/esp-idf/components/bootloader/subproject"
  "/home/jadenv/Documents/NULL/null_code/build/bootloader"
  "/home/jadenv/Documents/NULL/null_code/build/bootloader-prefix"
  "/home/jadenv/Documents/NULL/null_code/build/bootloader-prefix/tmp"
  "/home/jadenv/Documents/NULL/null_code/build/bootloader-prefix/src/bootloader-stamp"
  "/home/jadenv/Documents/NULL/null_code/build/bootloader-prefix/src"
  "/home/jadenv/Documents/NULL/null_code/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/jadenv/Documents/NULL/null_code/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/jadenv/Documents/NULL/null_code/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
