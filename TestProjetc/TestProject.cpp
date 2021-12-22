#include <JDA.h>
#include <iostream>
#include <zip.h>
#include <filesystem>

typedef uint16_t u2;

class JavaHeader
{
public:
	byte magic[4];
	u2 minor_version;
	u2 major_version;
	u2 constant_pool_count;
};

int main() {
	int error;
	auto file_path = std::filesystem::current_path() / "../jars" / "NFix.jar";

	zip* file = zip_open(file_path.string().c_str(), ZIP_RDONLY, &error);
	JDA* jda = new JDA(file);

	/* added simple test */
	for (auto b : jda->classBuffers) {
		auto klass = *reinterpret_cast<JavaHeader*>(&b.second->buffer.data()[0]);
		std::cout << b.first << " -> " << std::format("{:x}{:x}{:x}{:x}", klass.magic[0], klass.magic[1], klass.magic[2], klass.magic[3]) << std::endl;
	}

	return 0;
}