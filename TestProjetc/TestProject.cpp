#include <JDA.h>
#include <iostream>
#include <zip.h>
#include <filesystem>

int main() {
	int error;
	auto file_path = std::filesystem::current_path() / "../jars" / "NFix.jar";

	zip* file = zip_open(file_path.string().c_str(), ZIP_RDONLY, &error);
	JDA* jda = new JDA(file);

	auto b = jda->classes[0];
	std::cout << b->getClassName() << " : " << b->getSuperClassName() << std::endl;

	return 0;
}