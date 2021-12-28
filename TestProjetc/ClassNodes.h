#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <JDA.h>

struct MethodCallerInfo
{
	std::string caller_class;
	std::string klass;
	std::string method_name;
	std::string method_descriptor;

};
class ClassNodes
{
public:
	std::pair<std::string, std::string> ClassName; // <obf, unobf>
	std::unordered_map<std::string, std::string> Fields; // <obf, unobf>
	std::unordered_map<std::string, std::string> Methods; // <obf<descriptor>, unobf<descriptor>>
};
std::unordered_map<std::string, ClassNodes*> DeObfuscatedClasses; // <name, classnode>

class DifferenceSpotting
{
protected:
	JavaClass *obf_klass, *unobfuscated_klass;
public:
	std::pair<std::vector<MethodCallerInfo>, std::vector< MethodCallerInfo>> GetInvokeVirtual() // return two list of methodcallerinfo
	{
		auto p = DeObfuscatedClasses[obf_klass->getClassName()];
		if (p)
		{
			auto unobfuscated_name = p->ClassName.second;
			for (auto method : obf_klass->methods)
			{
				std::string fullname = method->getName().append(method->getDescriptors());
				bool good = false;
				 
				for (auto b : p->Methods)
				{
					if (b.first == fullname)
					{
						fullname = b.second;
						good = true;
					}
				}
				if (!good)
					continue;

				for (auto method_ : unobfuscated_klass->methods)
				{

				}
			}
		}
		else
			printf("Failed to gather %s from DeObfuscatedClasses!\n", obf_klass->getClassName().c_str());

		return std::pair< std::vector<MethodCallerInfo>, std::vector<MethodCallerInfo>>();
	}
	// Main -> une autre classe -> une autre classe -> une autre classe -> une autre classe -> une autre classe -> une autre classe -> une autre classe
	void DebutHierarchisation()
	{
		std::unordered_map<int, MethodCallerInfo> invoke_unobf, invoke_obf;
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
					std::string descriptor = unobfuscated_klass->constantPool->getNameAndTypeDescriptor(methodRef->nameAndTypeIndex);

					if (methodClass.find("net/m") != std::string::npos) // won't change
						invoke_unobf.insert(std::make_pair(invoke_unobf.size(), MethodCallerInfo(method->getName(), (methodClass), (name), (descriptor))));
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
					std::string descriptor = obf_klass->constantPool->getNameAndTypeDescriptor(methodRef->nameAndTypeIndex);
					if (methodClass.find("/") == std::string::npos) // will change often
						invoke_obf.insert(std::make_pair(invoke_obf.size(), MethodCallerInfo(method->getName(), (methodClass), (name), (descriptor))));
				}
			}
		}

		/* shit start here */
		for (auto& unobf_method : invoke_unobf)
		{
			auto unobf_methodcallerinfo = unobf_method.second;
			auto obf_method = invoke_obf[unobf_method.first];
			if (obf_method.method_descriptor == unobf_methodcallerinfo.method_descriptor)
			{
				DeObfuscatedClasses.insert(std::make_pair(obf_method.klass, new ClassNodes())); // since we are 100% sure
				DeObfuscatedClasses[obf_method.klass]->ClassName = std::make_pair(obf_method.klass, unobf_methodcallerinfo.klass);
				DeObfuscatedClasses[obf_method.klass]->Methods.insert(std::make_pair(obf_method.method_name.append(obf_method.method_descriptor), unobf_methodcallerinfo.method_name.append(unobf_methodcallerinfo.method_descriptor)));
			}
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
			DebutHierarchisation();
			DeObfuscatedClasses.insert(std::make_pair(obf_klass->getClassName(), new ClassNodes()));
			DeObfuscatedClasses[obf_klass->getClassName()]->ClassName = std::make_pair(obf_klass->getClassName(), unobfuscated_klass->getClassName());
			DeObfuscatedClasses[obf_klass->getClassName()]->Methods.insert(std::make_pair("main", "main"));

			for (auto clazz : DeObfuscatedClasses) // should only have minecraft
			{
				this->obf_klass = obfuscated->getClass(clazz.second->ClassName.first);
				this->unobfuscated_klass = non_obfuscated->getClass(clazz.second->ClassName.second);
				if(InitClass(this->obf_klass) && InitClass(this->unobfuscated_klass))
					GetInvokeVirtual();
			}
		}
	}
};