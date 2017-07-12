/*
 * Util.cpp
 *
 *  Created on: 2017年6月23日
 *      Author: wong
 */

#include "Util.h"
#include <stdio.h>

void Util::HexDump(const char* buf, int len) {
  printf("len=%d, ", len);
  for (int i = 0; i < len; ++i) {
    printf("%02x ", (unsigned char) *(buf + i));
  }
  printf("\n");
}


