#include <framework/common.h>
#include <iostream>
#include <filesystem>
#include <format>
#include "ClassNodes.h"
/* Java Deobfuscator */

int main() {

	int error;
	auto nfix_path = std::filesystem::current_path() / "../jars" / "NFix.jar";
	std::cout << std::format("File path = {}\n", nfix_path.string()).c_str() << std::endl;
	zip* file_obf = new zip(nfix_path.string().c_str());

	file_obf->printdir();
	for (auto e : file_obf->namelist())
	{
		std::cout << "E = " << e.c_str() << std::endl;
		std::cout << "is_dir = " << file_obf->is_directory(e) << std::endl;
	}

	auto jda = new JDA(file_obf);
	for (auto klass : jda->getClasses())
	{
		if(!klass->is_readed)
			klass->read();
		std::cout << "Found class with name = " << klass->getClassName() << std::endl;
		for (auto method : klass->methods)
		{
			std::cout << "found method " << method->getName() << " in " << klass->getClassName() << std::endl;
		}
	}
	

	return 0;
}