//#pragma once
#include "common.h"
#include "ByteBuffer.h"
#pragma warning(disable : 26495)
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

	std::string getClassAt(int idx) {
		return getUtf8At(((CP_Class*)cpInfos.at(idx))->nameIndex);
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
		return (u4)((CP_Integer*)cpInfos.at(idx))->value;
	}

	float getFloatAt(int idx) {
		return (float)((CP_Float*)cpInfos.at(idx))->value;
	}

	long getLongAt(int idx) {
		return (long)((CP_Long*)cpInfos.at(idx))->value;
	}

	double getDoubleAt(int idx) {
		return (double)((CP_Long*)cpInfos.at(idx))->value;
	}
};

class AttributeValue {
public:
	ConstantPool* cp;
public:
	AttributeValue(ConstantPool* constantPool) : cp(constantPool){}
};

class ConstantValue : public AttributeValue {
public:
	u2 constantValueIndex;
public:
	ConstantValue(ConstantPool* c) : AttributeValue(c){}
public:
	u4 getInt() { return (u4)cp->getIntAt(constantValueIndex); }
	u4 getFloat() { return (u4)cp->getFloatAt(constantValueIndex); }
	u4 getLong() { return (u4)cp->getLongAt(constantValueIndex); }
	u4 getDouble() { return (u4)cp->getDoubleAt(constantValueIndex); }
};

class Synthetic : public AttributeValue {
public:
	Synthetic(ConstantPool* c) : AttributeValue(c) {}
};

class Signature : public AttributeValue {
public:
	u2 signatureIndex;
public:
	Signature(ConstantPool* c) : AttributeValue(c) {}
public:
	std::string getSignature() { return cp->getUtf8At(signatureIndex); }
};

class Deprecated : public AttributeValue {
public:
	Deprecated(ConstantPool* c) : AttributeValue(c) {}
};

class Exception {
public:
	u2 startPc, endPc, handlerPc, catchType;
};

class Code : public AttributeValue {
public:
	u2 maxStack;
	u2 maxLocals;

	u4 codeLength;
	std::vector<u1> code;

	u2 exceptionTableLength;
	std::vector<Exception*> exceptionTable;

	//skip inside attributes//
public:
	Code(ConstantPool* c) : AttributeValue(c) {}
};

class InnerClass {
public:
	ConstantPool* cp;
	u2 innerClassIndex, outerClassIndex, innerNameIndex, accessFlags;
public:
	InnerClass(ConstantPool* constantPool) : cp(constantPool){}
public:
	std::string getClassName() { return cp->getUtf8At(innerNameIndex); }
};

class InnerClassAV : public AttributeValue {
public:
	u2 innerClassCount;
	std::vector<InnerClass*> innerClasses;
public:
	InnerClassAV(ConstantPool* c) : AttributeValue(c) {}
};

class AttributeInfo {
public:
	ConstantPool* cp;
	u2 attributeNameIndex;
	u4 attributeLength;
	AttributeValue* value;
public:
	AttributeInfo(ConstantPool* constantPool) : cp(constantPool) {}
public:
	std::string getName() { return cp->getUtf8At(attributeNameIndex); }
};

class FieldInfo {
public:
	ConstantPool* cp;
	u2 accessFlags;
	u2 nameIndex;
	u2 descriptorIndex;
	u2 attributeCount;
	std::vector<AttributeInfo*> attributes;
public:
	FieldInfo(ConstantPool* constantPool) : cp(constantPool){}
public:
	std::string getName() { return cp->getUtf8At(nameIndex); }
	std::string getDescriptor() { return cp->getUtf8At(descriptorIndex); }

	bool isPublic() { return (accessFlags & 0x0001) != 0; }
	bool isPrivate() { return (accessFlags & 0x0002) != 0; }
	bool isProtected() { return (accessFlags & 0x0004) != 0; }
	bool isStatic() { return (accessFlags & 0x0008) != 0; }
	bool isFinal() { return (accessFlags & 0x0010) != 0; }
	bool isVolatile() { return (accessFlags & 0x0040) != 0; }
	bool isTransient() { return (accessFlags & 0x0080) != 0; }
	bool isSynthetic() { return (accessFlags & 0x1000) != 0; }
	bool isEnum() { return (accessFlags & 0x4000) != 0; }
};

class MethodInfo {
public:
	ConstantPool* cp;
	u2 accessFlags, nameIndex, descriptorIndex;
	
	u2 attributeCount;
	std::vector<AttributeInfo*> attributes;
public:
	MethodInfo(ConstantPool* constantPool) : cp(constantPool){}
public:
	std::string getName() { return cp->getUtf8At(nameIndex); }
	std::string getDescriptors() { return cp->getUtf8At(descriptorIndex); }
	
	auto getReturnValue() -> std::string { 
		auto descriptor = cp->getUtf8At(descriptorIndex); 
		size_t index = (index = descriptor.find("()")) != std::string::npos ? index : (index = descriptor.find(";)")) != std::string::npos ? index : -1;
		assert(index != -1); 
		return &descriptor.data()[index + 2];
	}

	auto getArguments() -> std::vector<std::pair<size_t, std::string>>
	{
		std::vector<std::pair<size_t, std::string>> arguments;

		auto descriptor = cp->getUtf8At(descriptorIndex);
		size_t end_index = (end_index = descriptor.find(";)")) != std::string::npos ? end_index : -1;
		if (descriptor.find("()") != std::string::npos) // no args
			return arguments;

		size_t index = 0;
		while (index != -1)
		{
			index = (index = std::string(&descriptor.data()[index]).find("(")) != std::string::npos ? (index +=1) : -1;
			if (index == -1)
				index = (index = std::string(&descriptor.data()[index]).find(";")) != std::string::npos ? (index += 1) : -1;

			size_t end_index = (end_index = std::string(&descriptor.data()[index]).find(";")) != std::string::npos ? end_index : -1;
			if (end_index == -1)
				break;

			if (index == -1)
				break;
			auto str_cpy = std::string(&descriptor[index]);
			str_cpy.resize(end_index - (index-1));
			arguments.push_back(std::make_pair(arguments.size()+1, str_cpy));
		}
		return arguments;
	}

	bool isPublic() { return (accessFlags & 0x0001) != 0; }
	bool isPrivate() { return (accessFlags & 0x0002) != 0; }
	bool isProtected() { return (accessFlags & 0x0004) != 0; }
	bool isStatic() { return (accessFlags & 0x0008) != 0; }
	bool isFinal() { return (accessFlags & 0x0010) != 0; }
	bool isSynchronized() { return (accessFlags & 0x0020) != 0; }
	bool isBridge() { return (accessFlags & 0x0040) != 0; }
	bool isVarArgs() { return (accessFlags & 0x0080) != 0; }
	bool isNative() { return (accessFlags & 0x0100) != 0; }
	bool isAbstract() { return (accessFlags & 0x0400) != 0; }
	bool isStrict() { return (accessFlags & 0x0800) != 0; }
	bool isSynthetic() { return (accessFlags & 0x1000) != 0; }
};

class JavaClass {
private:
	ByteBuffer* buf;
public:
	u2 minorVersion;
	u2 majorVersion;

	ConstantPool* constantPool;

	u2 accessFlags;
	u2 thisClass;
	u2 superClass;

	u2 interfaceCount;
	std::vector<std::string> interfaces;

	u2 fieldCount;
	std::vector<FieldInfo*> fields;

	u2 methodCount;
	std::vector<MethodInfo*> methods;

	u2 attributeCount;
	std::vector<AttributeInfo*> attributes;
public:
	JavaClass(ByteBuffer* buffer) : buf(buffer) {}
private:
	void readAttributeList(u2, std::vector<AttributeInfo*>*);
public:
	void read();
public:
	bool isPublic() { return (accessFlags & 0x0001) != 0; }
	bool isFinal() { return (accessFlags & 0x0010) != 0; }
	bool isSuper() { return (accessFlags & 0x0020) != 0; }
	bool isInterface() { return (accessFlags & 0x0200) != 0; }
	bool isAbstract() { return (accessFlags & 0x0400) != 0; }
	bool isSynthetic() { return (accessFlags & 0x1000) != 0; }
	bool isAnnotation() { return (accessFlags & 0x2000) != 0; }
	bool isEnum() { return (accessFlags & 0x4000) != 0; }

	std::string getClassName() {
		return constantPool->getClassAt(thisClass);
	}

	std::string getSuperClassName() {
		return constantPool->getClassAt(superClass);
	}

	bool hasAttribute(std::string name);
	AttributeInfo* getAttribute(std::string name);
};