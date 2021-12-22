#pragma once

#include "common.h"
#include "ByteBuffer.h"
#include "JavaClass.h"

class JDA {
public:
	zip* jarFile;
public:
	std::vector<JavaClass*> classes;
	JDA(zip*);
public:
	JavaClass* getClass(std::string name);
};