#pragma once

#include "common.h"

class ByteBuffer {
public:
	std::vector<byte> buffer;
	int pos;
public:
	ByteBuffer(std::vector<byte>);
public:
	std::vector<byte> read(int);

	template<typename T>
	T get();
	
	std::string readString(int);
	void skip(int amount) {
		pos += amount;
	}
};

template<typename T>
T ByteBuffer::get() {
	return *reinterpret_cast<T*>(read(sizeof(T)).data());
}