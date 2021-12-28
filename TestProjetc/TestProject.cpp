#include <JDA.h>
#include <iostream>
#include <zip.h>
#include <filesystem>
#include "CodeUtils.h"
#include "ClassNodes.h"
/* Java Deobfuscator */

int main() {

	int error;
	auto file_path_era = std::filesystem::current_path() / "../jars" / "averfight.jar";
	auto file_path_mc  = std::filesystem::current_path() / "../jars" / "cleaned_minecraft.jar";

	zip* file_obf = zip_open(file_path_era.string().c_str(), ZIP_RDONLY, &error);
	zip* file_unobf = zip_open(file_path_mc.string().c_str(), ZIP_RDONLY, &error);
	auto t1 = std::chrono::high_resolution_clock::now();
	JDA* jda_obf = new JDA(file_obf);
	JDA* jda_unobf = new JDA(file_unobf);

	zip_close(file_obf);
	zip_close(file_unobf);
	std::cout << "[+] Obfuscated file loaded -> " << file_path_era.string() << std::endl;
	std::cout << "[+] UnObfuscated file loaded -> " << file_path_mc.string() << std::endl;
	
	//--OPCODE MANIPULATION--//
	ClassUtils* CUtils = new ClassUtils(jda_obf, jda_unobf);
	CUtils->SpotDifference();

	auto delta = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::high_resolution_clock::now() - t1);
	std::cout << "[+] Deobfuscation Done in -> " << delta.count() << "ms" << std::endl;

	//for (auto b : DeObfuscatedClasses)
	//{
	//	std::cout << b.first << " -> " << b.second->ClassName.second << ";" << std::endl;
	//	for (auto p : b.second->Methods)
	//	{
	//		std::cout << b.first << "." << p.first << " -> " << p.second << std::endl;
	//	}
	//}
	return 0;
}