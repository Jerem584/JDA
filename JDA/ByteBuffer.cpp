#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(std::vector<byte> buf) {
	this->buffer = buf;
	this->pos = 0;
}

std::vector<byte> ByteBuffer::read(int size) {
	std::cout << "read " << pos << " " << size << std::endl;
	if (pos < buffer.size() && pos+size < buffer.size()) {
		std::vector<u1> res(buffer.begin() + pos, buffer.begin() + pos + size);
		std::reverse(res.begin(), res.end());
		pos += size;
		return res;
	}
	return std::vector<byte>();
}

const char* ByteBuffer::readString(int size) {
	std::cout << "reading string " << size << std::endl;
	char* data = new char[size];
	std::vector<u1> buf = read(size);
	std::reverse(buf.begin(), buf.end());
	memcpy(data, buf.data(), size);
	data[size] = 0x00;
	return data;
}