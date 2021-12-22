#include <JDA.h>
#include <iostream>
#include <zip.h>
#include <filesystem>
#include <format>
#include <iostream>
#include <string>
#include <string_view>


int main() {
	int error;
	auto file_path = std::filesystem::current_path() / "../jars" / "NFix.jar";

	zip* file = zip_open(file_path.string().c_str(), ZIP_RDONLY, &error);
	JDA* jda = new JDA(file);
	for (auto b : jda->classBuffers)
	{
		auto buf = b.second;
		buf->skip(4);
		uint16_t minor = buf->get<uint16_t>();
		uint16_t major = buf->get<uint16_t>();
		uint16_t constantPoolCount = buf->get<uint16_t>();
		std::cout << b.first << " -> " << major << std::endl;
	}

	
	
	return 0;
}