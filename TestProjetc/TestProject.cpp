#include <JDA.h>
#include <iostream>
#include <zip.h>
#include <filesystem>

int main() {
	int error;
	auto file_path = std::filesystem::current_path() / "../jars" / "TestProject.jar";

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
			for (auto argument : method->getArguments())
			{
				std::cout << argument.second << (method->getArguments().size() >= argument.first+size_t(1) ? "," : "");
			}
			std::cout << ")" << method->getReturnType() << std::endl;
		}
		std::cout << std::endl;
	}
	
	return 0;
}