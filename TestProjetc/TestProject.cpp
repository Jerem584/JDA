#include <framework/common.h>
#include <iostream>
#include <filesystem>
#include <format>
#include "Timer.hpp"
#include "ClassNodes.h"
/* Java Deobfuscator */

int main() {

	auto test_shitnoctu = std::filesystem::current_path() / "../jars" / "NFix.jar";
	auto nfix_path = std::filesystem::current_path() / "../jars" / "erazion.jar";
	auto nfix_path_assetsless = std::filesystem::current_path() / "../jars" / "erazion_assetsless.jar";

	Timer t;
	t.start();
	std::cout << "[+] Reading zip file -> " << nfix_path << std::endl;
	zip* file_obf = new zip(nfix_path.string().c_str());
	std::cout << "[+] Zip reading time -> " << t.milliseconds_elapsed() << "ms\n";

	t.reset();
	std::cout << "[+] Reading zip file -> " << nfix_path_assetsless << std::endl;
	file_obf = new zip(nfix_path_assetsless.string().c_str());
	std::cout << "[+] Zip reading time -> " << t.milliseconds_elapsed() << "ms\n";

	//file_obf = new zip(test_shitnoctu.string().c_str());
	t.reset();
	std::cout << "[+] Constructing JDA " << std::endl;
	auto jda = new JDA(file_obf);
	printf("Read = %llX\n", g_ReadCount);
	std::cout << "[+] Constructed JDA in -> " << t.milliseconds_elapsed() << "ms" << std::endl;


	//for (auto klass : jda->getClasses())
	//{
	//	std::cout << "Found class with name = " << klass->getClassName() << std::endl;
	//	for (auto method : klass->methods)
	//	{
	//		std::cout << "found method " << method->getName() << " in " << klass->getClassName() << std::endl;
	//	}
	//	for (auto field : klass->fields) {
	//		std::cout << "found field " << field->getName() << " in " << klass->getClassName() << std::endl;
	//	}
	//}
	

	return 0;
}