#include "JDA.h"

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
			if (buffer[0] != 202/*ca*/ || buffer[1] != 254/*fe*/ || buffer[2] != 186/*ba*/ || buffer[3] != 190/*be*/) {
				std::cout << "[-] Magic value incorrect for -> " << name << std::endl;
				continue;
			}
			free(buffer); // free (c'est mieux)
			JavaClass* clazz = new JavaClass(new ByteBuffer(res));
			clazz->read();
			classes.push_back(clazz);
		}
	}
}

JavaClass* JDA::getClass(std::string name) {
	for (const auto& clazz : classes) {
		if (clazz->getClassName().compare(name) == 0)
			return clazz;
	}
	return nullptr;
}
