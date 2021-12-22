#include "JClassUtils.h"

std::vector<JavaClass*> JClassUtils::getInnerClasses(JDA* jda, JavaClass* javaClass) {
    std::vector<JavaClass*> result;
    InnerClassAV* innerClassAV = (InnerClassAV*)javaClass->getAttribute("InnerClasses");
    if (innerClassAV != nullptr) {
        for (const auto& innerClass : innerClassAV->innerClasses) {
            result.push_back(jda->getClass(innerClass->getClassName()));
        }
    }
    return result;
}
