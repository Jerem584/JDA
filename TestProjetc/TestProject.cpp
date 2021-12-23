#include <JDA.h>
#include <iostream>
#include <zip.h>
#include <filesystem>
#include "CodeUtils.h"

int main() {
	int error;
	auto file_path = std::filesystem::current_path() / "../jars" / "erazion.jar";

	zip* file = zip_open(file_path.string().c_str(), ZIP_RDONLY, &error);
	JDA* jda = new JDA(file);

	for (auto& b : jda->classes)
	{
		std::cout << b->getClassName() << " : " << b->getSuperClassName() << " : " << (b->interfaces.size() != 0 ? b->interfaces[0] : "none") << std::endl;

		std::cout << "Fields:" << std::endl;
		for (auto field : b->fields) {
			std::cout << "-- " << field->getName() << field->getDescriptor() << std::endl;
		}

		std::cout << "Methods:" << std::endl;
		for (auto method : b->methods) {
			std::cout << "-- " << method->getName() << "(";
			for (auto args : method->getArguments())
			{
				std::cout << args << ", ";
			}
			std::cout << ") -> " << method->getReturnType() << std::endl;
		}

		std::cout << std::endl;
	}


	//--OPCODE MANIPULATION--//

	/*auto b = jda->classes[0];
	auto method = b->methods[1];
	for (auto opcode : CodeUtils::getOpcodesFromVector(method->getCode())) {
		std::cout << std::format("{:x}", (int)opcode.first) << std::endl;
		for (auto arg : opcode.second) {
			std::cout << "- " << std::format("{:x}", arg) << std::endl;
		}
	}

	std::vector<std::pair<JvmOpcode, std::vector<u1>>> opcodes = CodeUtils::getOpcodesFromVector(method->getCode());
	for (const auto& opcode : opcodes) {
		if (opcode.first == JvmOpcode::INVOKESPECIAL) {
			u2 methodRefIdx = CodeUtils::getInvokeIndex(opcode.second);
			CP_MethodRef* methodRef = b->constantPool->getMethodRef(methodRefIdx);
			std::string methodClass = b->constantPool->getClassAt(methodRef->classIndex);
			std::string name = b->constantPool->getNameAndTypeName(methodRef->nameAndTypeIndex);
			std::string descriptor = b->constantPool->getNameAndTypeDescriptor(methodRef->nameAndTypeIndex);

			std::cout << methodClass << "  " << name << "  " << descriptor << std::endl;
		}
	}*/

	return 0;
}