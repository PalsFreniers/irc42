#pragma once

#include <string>

typedef uint8_t u8;
typedef uint32_t u32;

void sha256(std::string msg, u32 hash[8]);
