#pragma once

#include "common.h"
#include "ByteBuffer.h"
#include "JavaClass.h"

class JDA {
public:
	zip* jarFile;
public:
	std::vector<std::pair<std::string, JavaClass*>> classes;
	JDA() {};
	JDA(zip*);
public:
	JavaClass* getClass(std::string name);
	std::vector<JavaClass*> getClasses(std::string name);
};