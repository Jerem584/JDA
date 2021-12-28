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
	JDA* jda_obf = new JDA(file_obf);
	JDA* jda_unobf = new JDA(file_unobf);

	zip_close(file_obf);
	zip_close(file_unobf);
	std::cout << "[+] Obfuscated file loaded -> " << file_path_era.string() << std::endl;
	std::cout << "[+] UnObfuscated file loaded -> " << file_path_mc.string() << std::endl;
	
	//--OPCODE MANIPULATION--//
	ClassUtils* CUtils = new ClassUtils(jda_obf, jda_unobf);
	CUtils->SpotDifference();


	return 0;
}