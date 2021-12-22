#include <JDA.h>
#include <iostream>
#include <zip.h>
#include <filesystem>
#include <ByteBuffer.h>

int main() {
	int error;
	auto file_path = std::filesystem::current_path() / "../jars" / "NFix.jar";

	zip* file = zip_open(file_path.string().c_str(), ZIP_RDONLY, &error);
	JDA* jda = new JDA(file);
	
	ByteBuffer* buf = jda->classBuffers["fr/noctu/nfix/JoinListener.class"];
	buf->skip(4);
	uint16_t minor = buf->get<uint16_t>();
	uint16_t major = buf->get<uint16_t>();
	uint16_t constantPoolCount = buf->get<uint16_t>();
	std::cout << constantPoolCount << std::endl;

	return 0;
}