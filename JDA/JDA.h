#pragma once

#include "common.h"
#include "ByteBuffer.h"

class JDA {
public:
	zip* jarFile;
	std::unordered_map<std::string, ByteBuffer*> classBuffers;
public:
	JDA(zip*);
};