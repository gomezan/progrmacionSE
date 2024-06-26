# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/david/esp/v5.2/esp-idf/components/bootloader/subproject"
  "C:/Software/Proyecto4/proof03_uart_interrupt/proof_timer/build/bootloader"
  "C:/Software/Proyecto4/proof03_uart_interrupt/proof_timer/build/bootloader-prefix"
  "C:/Software/Proyecto4/proof03_uart_interrupt/proof_timer/build/bootloader-prefix/tmp"
  "C:/Software/Proyecto4/proof03_uart_interrupt/proof_timer/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Software/Proyecto4/proof03_uart_interrupt/proof_timer/build/bootloader-prefix/src"
  "C:/Software/Proyecto4/proof03_uart_interrupt/proof_timer/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Software/Proyecto4/proof03_uart_interrupt/proof_timer/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Software/Proyecto4/proof03_uart_interrupt/proof_timer/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
