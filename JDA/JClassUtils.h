#pragma once

#include "common.h"
#include "JDA.h"

class JClassUtils {
	static std::vector<JavaClass*> getInnerClasses(JDA* jda, JavaClass* javaClass);
};