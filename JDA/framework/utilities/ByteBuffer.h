#pragma once

//#include "common.h"

class ByteBuffer {
public:
	std::vector<byte> buffer;
	std::vector<byte> reverse_buffer;
	int pos;
public:
	ByteBuffer(std::vector<byte> buf) {
		this->buffer = buf;
		this->pos = 0;
	}
public:
	[[deprecated("use best_read for now")]]
	std::vector<byte> read(int);

	byte* best_read(int size, bool reverse = true);

	template<typename T>
	T get();
	template<typename T>
	void get(T*);

	auto skip(int size) -> void { pos += size; };
	
	const char* readString(int);


};

template<typename T>
T ByteBuffer::get() {
		return *reinterpret_cast<T*>(best_read(sizeof(T)));
}

template<typename T>
void ByteBuffer::get(T* var) {
	*var = get<T>();
}

byte* ByteBuffer::best_read(int size, bool reverse) {
	if (pos < buffer.size() && pos + static_cast<unsigned long long>(size) < buffer.size() + 1) {
		if(reverse)
			std::reverse(buffer.begin() + pos, buffer.begin() + pos + size); // reverse buffer directly
		pos += size;
		return &buffer.data()[pos - size];
	}

	throw std::exception("Couldn't read memory in bytebuffer");
	return 0;
}

const char* ByteBuffer::readString(int size) {
	//auto read_res = best_read(size, false);
	//read_res[size] = 0x00;

	char* data = new char[size];
	auto read_res = best_read(size, false);
	memcpy(data, read_res, size);
	data[size] = 0x00;
	return data;
}