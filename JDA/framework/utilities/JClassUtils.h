#pragma once

//#include "common.h"
//#include "JDA.h"

class JClassUtils {
public:
    static /*?*/ std::vector<JavaClass*> getInnerClasses(JDA* jda, JavaClass* javaClass) {
        std::vector<JavaClass*> result;
        InnerClassAV* innerClassAV = (InnerClassAV*)javaClass->getAttribute("InnerClasses");
        if (innerClassAV != nullptr) {
            for (const auto& innerClass : innerClassAV->innerClasses) {
                result.push_back(jda->getClass(innerClass->getClassName()));
            }
        }
        return result;
    }

public:
    JavaClass* klass;
    JClassUtils(JavaClass* klass)
    {
        this->klass = klass;
    }

    std::vector<JavaClass*> getInnerClasses() {
        std::vector<JavaClass*> result;
        InnerClassAV* innerClassAV = (InnerClassAV*)this->klass->getAttribute("InnerClasses");
        if (innerClassAV != nullptr) {
            for (const auto& innerClass : innerClassAV->innerClasses) {
                result.push_back(g_JDA->getClass(innerClass->getClassName()));
            }
        }
        return result;
    }
};