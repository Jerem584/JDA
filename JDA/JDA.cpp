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
			byte* buffer = (byte*)malloc(size);
			zip_fread(file, buffer, size);
			zip_fclose(file);

			std::vector<byte> res(buffer, buffer + size);
			classes.insert(std::pair(name, new ByteBuffer(res)));
		}
	}
}