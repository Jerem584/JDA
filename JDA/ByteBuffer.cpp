#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(std::vector<byte> buf) {
	this->buffer = buf;
	this->pos = 0;
}

std::vector<byte> ByteBuffer::read(int size) {
	if (pos < buffer.size() && pos < buffer.size() + size) {
		std::vector<byte> res(buffer.begin() + pos, buffer.begin() + pos + size);
		std::reverse(res.begin(), res.end());
		pos += size;
		return res;
	}
	return std::vector<byte>();
}

std::string ByteBuffer::readString(int size) {
	byte* data = read(size).data();
}
