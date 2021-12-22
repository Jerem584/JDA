#pragma once

#include "common.h"
#include "ByteBuffer.h"

class JDA {
public:
	zip* jarFile;
public:
	std::unordered_map<std::string, ByteBuffer*> classBuffers;
	JDA(zip*);
};