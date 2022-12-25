#pragma once

//#include "common.h"

class ByteBuffer {
public:
	std::vector<byte> buffer;
	int pos;
public:
	ByteBuffer(std::vector<byte> buf) {
		this->buffer = buf;
		this->pos = 0;
	}
public:
	std::vector<byte> read(int);

	template<typename T>
	T get();
	template<typename T>
	void get(T*);

	auto skip(int size) -> void { pos += size; };
	
	const char* readString(int);


};

template<typename T>
T ByteBuffer::get() {
	return *reinterpret_cast<T*>(read(sizeof(T)).data());
}

template<typename T>
void ByteBuffer::get(T* var) {
	*var = get<T>();
}

std::vector<byte> ByteBuffer::read(int size) {
	if (pos < buffer.size() && pos + size < buffer.size() + 1) {
		std::vector<u1> res(buffer.begin() + pos, buffer.begin() + pos + size);
		std::reverse(res.begin(), res.end());
		pos += size;
		return res;
	}
	return std::vector<byte>();
}

const char* ByteBuffer::readString(int size) {
	char* data = new char[size];
	std::vector<u1> buf = read(size);
	std::reverse(buf.begin(), buf.end());
	memcpy(data, buf.data(), size);
	data[size] = 0x00;
	return data;
}