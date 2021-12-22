#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(std::vector<byte> buf) {
	this->buffer = buf;
	this->pos = 0;
}

std::vector<byte> ByteBuffer::read(int size) {
	if (pos < buffer.size() && pos+size < buffer.size()+1) {
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