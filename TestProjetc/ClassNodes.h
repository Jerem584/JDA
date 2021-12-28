#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <JDA.h>


class ClassNodes
{
public:
	std::pair<std::string, std::string> ClassName; // <obf, unobf>
	std::unordered_map<std::string, std::string> Fields; // <obf, unobf>
	std::unordered_map<std::string, std::string> Methods; // <obf<descriptor>, unobf<descriptor>>
};
std::unordered_map<std::string, ClassNodes*> DeObfuscatedClasses;

class DifferenceSpotting
{
protected:
	JavaClass *obf_klass, *unobfuscated_klass;
public:
	void InvokeVirtualDiff()
	{
		std::vector<std::pair<std::string, std::string>> invoke_unobf, invoke_obf;
		for (const auto& method : unobfuscated_klass->methods)
		{
			auto opcodes = CodeUtils::getOpcodesFromVector(method->getCode());
			for (const auto& opcode : opcodes)
			{
				if (opcode.first == JvmOpcode::INVOKEVIRTUAL)
				{
					u2 methodRefIdx = CodeUtils::getInvokeIndex(opcode.second);
					CP_MethodRef* methodRef = unobfuscated_klass->constantPool->getMethodRef(methodRefIdx);
					std::string methodClass = unobfuscated_klass->constantPool->getClassAt(methodRef->classIndex);
					std::string name = unobfuscated_klass->constantPool->getNameAndTypeName(methodRef->nameAndTypeIndex);
					if (methodClass.find("net/m") != std::string::npos) // won't change
						invoke_unobf.push_back(std::make_pair(method->getName(), (methodClass).append("->").append(name)));
				}
			}
		}
		for (const auto& method : obf_klass->methods)
		{
			auto opcodes = CodeUtils::getOpcodesFromVector(method->getCode());
			for (const auto& opcode : opcodes)
			{
				if (opcode.first == JvmOpcode::INVOKEVIRTUAL)
				{
					u2 methodRefIdx = CodeUtils::getInvokeIndex(opcode.second);
					CP_MethodRef* methodRef = obf_klass->constantPool->getMethodRef(methodRefIdx);
					std::string methodClass = obf_klass->constantPool->getClassAt(methodRef->classIndex);
					std::string name = obf_klass->constantPool->getNameAndTypeName(methodRef->nameAndTypeIndex);
					if (methodClass.find("/") == std::string::npos) // will change often
						invoke_obf.push_back(std::make_pair(method->getName(), (methodClass).append("->").append(name)));
				}
			}
		}

		std::cout << "UnObf -> \n";
		for (auto& b : invoke_unobf)
		{
			std::cout << "\t" << b.first << " calling -> " << b.second << std::endl;
		}

		std::cout << "Obf -> \n";
		for (auto& b : invoke_obf)
		{
			std::cout << "\t" << b.first << " calling -> " << b.second << std::endl;
		}

	}
};

class ClassUtils : DifferenceSpotting
{
private:
	JDA *obfuscated, *non_obfuscated;
private:
	auto InitClass(JavaClass* klass) -> bool
	{
		if (!klass->is_readed)
			klass->read();
		return true;
	}
	auto GetMainClass(bool obf)
	{
		return obf ? obfuscated->getClass("net/minecraft/client/main/Main") : non_obfuscated->getClass("net/minecraft/client/main/Main");
	}
public:
	ClassUtils(JDA* obf, JDA* unobf)
	{
		this->obfuscated = obf;
		this->non_obfuscated = unobf;
	}

	auto SpotDifference() -> void
	{
		if (InitClass(this->obf_klass = GetMainClass(true)) && InitClass(this->unobfuscated_klass = GetMainClass(false)))
		{
			InvokeVirtualDiff();
		}
	}
};