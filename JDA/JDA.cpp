#include "JDA.h"
#pragma warning(disable:6385)
JDA::JDA(zip* archive) {
	this->jarFile = archive;
	for (int i = 0; i < zip_get_num_entries(jarFile, ZIP_FL_UNCHANGED); i++) {
		std::string name(zip_get_name(jarFile, i, ZIP_FL_UNCHANGED));
		if (name.ends_with(".class")) {


			zip_stat_t stat;
			zip_stat_index(jarFile, i, ZIP_FL_UNCHANGED, &stat);
			zip_uint64_t size = stat.size;

			zip_file_t* file = zip_fopen_index(jarFile, i, ZIP_FL_UNCHANGED);
			byte* buffer = (byte*)malloc(size); // malloc donc free
			zip_fread(file, buffer, size);
			zip_fclose(file);
			std::vector<byte> res(buffer, buffer + size);
			
			if (buffer != nullptr && buffer[0] != (byte)202/*ca*/ || buffer[1] != (byte)254/*fe*/ || buffer[2] != (byte)186/*ba*/ || buffer[3] != (byte)190/*be*/) {
				std::cout << "[-] Magic value incorrect for -> " << name << std::endl;
				free(buffer); // yes.
				continue;
			}
			free(buffer); // free (c'est mieux)


			JavaClass* clazz = new JavaClass(new ByteBuffer(res));
			classes.push_back(std::make_pair(name, clazz));
		}
	}
}
JavaClass* JDA::getClass(std::string name) {
	for (const auto& clazz : classes) {
		name = name.append(".class");
		if ((clazz.first).compare("net/minecraft/client/main/Main.class") == 0)
			return clazz.second;
	}
	return nullptr;
}
std::vector<JavaClass*> JDA::getClasses(std::string name) {
	std::vector<JavaClass*> return_value;
	for (const auto& clazz : classes) {
		if ((clazz.first).find(name) != std::string::npos)
			return_value.push_back(clazz.second);
	}
	return return_value;
}
