#pragma once
#include "common.h"
#include "ByteBuffer.h"

enum class ConstantPoolType {
	CONSTANT_Null = 0,
	CONSTANT_Class = 7,
	CONSTANT_FieldRef = 9,
	CONSTANT_MethodRef = 10,
	CONSTANT_InterfaceMethodRef = 11,
	CONSTANT_String = 8,
	CONSTANT_Integer = 3,
	CONSTANT_Float = 4,
	CONSTANT_Long = 5,
	CONSTANT_Double = 6,
	CONSTANT_NameAndType = 12,
	CONSTANT_Utf8 = 1,
	CONSTANT_MethodHandle = 15,
	CONSTANT_MethodType = 16,
	CONSTANT_InvokeDynamic = 18,
};

class ConstantPoolInfo {
public:
	ConstantPoolType type;
public:
	ConstantPoolInfo(ConstantPoolType cpType) : type(cpType){}
};

class CP_MethodRef : public ConstantPoolInfo {
public:
	u2 classIndex, nameAndTypeIndex;
public:
	CP_MethodRef(ConstantPoolType cpType) : ConstantPoolInfo(cpType){}
};

class CP_InterfaceMethodRef : public ConstantPoolInfo {
public:
	u2 classIndex, nameAndTypeIndex;
public:
	CP_InterfaceMethodRef(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_FieldRef : public ConstantPoolInfo {
public:
	u2 classIndex, nameAndTypeIndex;
public:
	CP_FieldRef(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_Class : public ConstantPoolInfo {
public:
	u2 nameIndex;
public:
	CP_Class(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_String : public ConstantPoolInfo {
public:
	u2 utf8Index;
public:
	CP_String(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_Integer : public ConstantPoolInfo {
public:
	u4 value;
public:
	CP_Integer(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_Float : public ConstantPoolInfo {
public:
	float value;
public:
	CP_Float(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_Long : public ConstantPoolInfo {
public:
	u8 value;
public:
	CP_Long(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_Double : public ConstantPoolInfo {
public:
	double value;
public:
	CP_Double(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_NameAndType : public ConstantPoolInfo {
public:
	u2 nameIndex;
	u2 descriptorIndex;
public:
	CP_NameAndType(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_Utf8 : public ConstantPoolInfo {
public:
	std::string value;
public:
	CP_Utf8(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_MethodHandle : public ConstantPoolInfo {
public:
	u1 referenceKind;
	u2 referenceIndex;
public:
	CP_MethodHandle(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_MethodType : public ConstantPoolInfo {
public:
	u2 descriptorIndex;
public:
	CP_MethodType(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class CP_InvokeDynamic : public ConstantPoolInfo {
public:
	u2 bootstrapMethodAttrIndex;
	u2 nameAndTypeIndex;
public:
	CP_InvokeDynamic(ConstantPoolType cpType) : ConstantPoolInfo(cpType) {}
};

class ConstantPool {
public:
	u2 constantPoolCount;
	std::vector<ConstantPoolInfo*> cpInfos;
public:
	std::string getUtf8At(int idx) {
		return ((CP_Utf8*)cpInfos.at(idx))->value;
	}

	std::string getStringAt(int idx) {
		return getUtf8At(((CP_String*)cpInfos.at(idx))->utf8Index);
	}

	std::string getNameAndTypeName(int idx) {
		return getUtf8At(((CP_NameAndType*)cpInfos.at(idx))->nameIndex);
	}

	std::string getNameAndTypeDescriptor(int idx) {
		return getUtf8At(((CP_NameAndType*)cpInfos.at(idx))->descriptorIndex);
	}

	u4 getIntAt(int idx) {
		return ((CP_Integer*)cpInfos.at(idx))->value;
	}

	float getFloatAt(int idx) {
		return ((CP_Float*)cpInfos.at(idx))->value;
	}

	long getLongAt(int idx) {
		return ((CP_Long*)cpInfos.at(idx))->value;
	}

	double getDoubleAt(int idx) {
		return ((CP_Long*)cpInfos.at(idx))->value;
	}
};

class JavaClass {
private:
	ByteBuffer* buf;
public:
	u2 minorVersion;
	u2 majorVersion;

	ConstantPool* constantPool;
public:
	JavaClass(ByteBuffer* buffer) : buf(buffer){}
	void read();
};