#pragma once

#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#pragma warning(push)
#pragma warning(disable:6297)
#pragma warning(disable:6319)
#include "vendor/zip.h"
#pragma warning(pop)
#include <fstream>
#include <cassert>

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;
using zip = miniz_cpp::zip_file;
#include "utilities/JvmOpcodes.h" // this only declare
inline int g_ReadCount;


#include "utilities/ByteBuffer.h"
#include "main_class/JavaClass.h"
#include "main_class/JDA.h"
JDA* g_JDA;

#include "utilities/CodeUtils.h"
#include "utilities/JClassUtils.h"

