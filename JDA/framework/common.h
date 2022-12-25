#pragma once

#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include "vendor/zip.h"
#include <fstream>
#include <cassert>

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;
using zip = miniz_cpp::zip_file;
#include "utilities/JvmOpcodes.h" // this only declare


#include "utilities/ByteBuffer.h"
#include "main_class/JavaClass.h"
#include "main_class/JDA.h"
JDA* g_JDA;

#include "utilities/CodeUtils.h"
#include "utilities/JClassUtils.h"

