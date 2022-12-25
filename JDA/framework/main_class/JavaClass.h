//#pragma once
//#include "common.h"
//#include "ByteBuffer.h"
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
class MethodUtils
{
public:
	static auto getType(std::string str, size_t& index) -> std::string
	{
		auto c = str[0];
		if (c != 'L')
			index++;
		if (c == ')')
			return getType(&str[1], index);
		if (c == '[')
		{
			auto nextindex = &str[1];
			auto type = getType(nextindex, index);
			return type.append("[]");
		}
		if (c == 'D')
			return ("java/lang/Double");

		if (c == 'I')
			return ("java/lang/Integer");

		if (c == 'Z')
			return ("java/lang/Boolean");

		if (c == 'J')
			return ("java/lang/Long");

		if (c == 'C')
			return ("java/lang/Character");

		if (c == 'F')
			return ("java/lang/Float");

		if (c == 'B')
			return ("java/lang/Byte");

		if (c == 'V')
			return ("java/lang/Void");

		if (c == 'S')
			return ("java/lang/Short");

		if (c == 'L')
		{
			auto copy_str = std::string(&str[1]);
			auto end_index = copy_str.find(";");
			copy_str.resize(end_index);
			index += end_index + 2;
			return copy_str;
		}
		return "ERROR";
	}

	static auto getReturnType(std::string descriptor) -> std::string {
		size_t end_index = descriptor.find(")");
		return getType(std::string(&descriptor[end_index]), end_index);
	}

	static auto getArguments(std::string desc) -> std::vector<std::string>
	{
		std::vector<std::string> arguments;

		auto descriptor = std::string(&desc[1]); // skip the (
		if (descriptor.find("()") != std::string::npos) // no args
			return arguments;
		descriptor.resize(descriptor.find(")")); // skip the ) 

		size_t index = 0;
		while (true)
		{
			if (index > (descriptor.size()))
				break;
			auto current_string = std::string(&descriptor[index]);
			if (current_string.empty())
				break;
			arguments.push_back(getType(current_string, index));
		}
		return arguments;
	}
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

	CP_NameAndType* getNameAndType(int idx) {
		return ((CP_NameAndType*)cpInfos.at(idx));
	}

	CP_FieldRef* getFieldRef(int idx) {
		return ((CP_FieldRef*)cpInfos.at(idx));
	}

	CP_MethodRef* getMethodRef(int idx) {
		return ((CP_MethodRef*)cpInfos.at(idx));
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

class Exceptions : public AttributeValue {
public:
	u2 numberOfExceptions;
	std::vector<std::string> exceptions;
public:
	Exceptions(ConstantPool* c) : AttributeValue(c) {}
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

	std::vector<u1> getCode() {
		for (const auto& attr : attributes) {
			if (attr->getName().compare("Code") == 0) {
				return ((Code*)attr->value)->code;
			}
		}
		return std::vector<u1>();
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
	char is_readed;
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
	JavaClass(ByteBuffer* buffer) : buf(buffer) { this->read(); }
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


void JavaClass::readAttributeList(u2 count, std::vector<AttributeInfo*>* list) {
	for (short i = 0; i < count; i++) {
		std::string name;

		AttributeInfo* attribute = new AttributeInfo(constantPool);
		buf->get(&attribute->attributeNameIndex);
		buf->get(&attribute->attributeLength);

		name = attribute->getName();
		if (name.compare("ConstantValue") == 0) {
			ConstantValue* cv = new ConstantValue(constantPool);
			buf->get(&cv->constantValueIndex);
			attribute->value = cv;
		}
		else if (name.compare("Synthetic") == 0) {
			attribute->value = new Synthetic(constantPool);
		}
		else if (name.compare("Signature") == 0) {
			Signature* sig = new Signature(constantPool);
			buf->get(&sig->signatureIndex);
			attribute->value = sig;
		}
		else if (name.compare("Deprecated") == 0) {
			attribute->value = new Deprecated(constantPool);
		}
		/*TO:DO*/
		else if (name.compare("RuntimeVisibleAnnotations") == 0 || name.compare("RuntimeInvisibleAnnotations") == 0 || name.compare("RuntimeVisibleParameterAnnotations") == 0 || name.compare("RuntimeInvisibleParameterAnnotations") == 0 || name.compare("AnnotationDefault") == 0 || name.compare("SourceFile") == 0 || name.compare("SourceFileDebugExtension") == 0 || name.compare("BootstrapMethods") == 0 || name.compare("EnclosingMethod") == 0) {
			buf->skip(attribute->attributeLength);
		}
		else if (name.compare("Code") == 0) {
			Code* code = new Code(constantPool);
			buf->get(&code->maxStack);
			buf->get(&code->maxLocals);
			buf->get(&code->codeLength);
			for (uint32_t i = 0; i < code->codeLength; i++) {
				code->code.push_back(buf->get<u1>());
			}

			buf->get(&code->exceptionTableLength);
			for (short i = 0; i < code->exceptionTableLength; i++) {
				Exception* exception = new Exception();
				buf->get(&exception->startPc);
				buf->get(&exception->endPc);
				buf->get(&exception->handlerPc);
				buf->get(&exception->catchType);
			}

			short attributeCount = buf->get<u2>();
			for (short i = 0; i < attributeCount; i++) {
				buf->skip(2);
				buf->skip(buf->get<u4>());
			}

			attribute->value = code;
		}
		else if (name.compare("InnerClasses") == 0) {
			InnerClassAV* innerClassAV = new InnerClassAV(constantPool);
			buf->get(&innerClassAV->innerClassCount);
			for (short i = 0; i < innerClassAV->innerClassCount; i++) {
				InnerClass* innerClass = new InnerClass(constantPool);
				buf->get(&innerClass->innerClassIndex);
				buf->get(&innerClass->outerClassIndex);
				buf->get(&innerClass->innerNameIndex);
				buf->get(&innerClass->accessFlags);
				innerClassAV->innerClasses.push_back(innerClass);
			}
			attribute->value = innerClassAV;
		}
		else if (name.compare("Exceptions") == 0) {
			Exceptions* exceptions = new Exceptions(constantPool);
			buf->get(&exceptions->numberOfExceptions);

			for (short i = 0; i < exceptions->numberOfExceptions; i++) {
				exceptions->exceptions.push_back(constantPool->getClassAt(buf->get<u2>()));
			}

			attribute->value = exceptions;
		}

		list->push_back(attribute);
	}
}

void JavaClass::read() {
	if (is_readed)
		return;
	is_readed = true;
	buf->skip(4);
	buf->get(&minorVersion);
	buf->get(&majorVersion);

	//CONSTANT POOL READ//
	constantPool = new ConstantPool();
	constantPool->cpInfos.push_back(new ConstantPoolInfo(ConstantPoolType::CONSTANT_Null));
	buf->get(&constantPool->constantPoolCount);
	for (int i = 1; i < constantPool->constantPoolCount; i++) {
		ConstantPoolType type = (ConstantPoolType)buf->get<u1>();
		if (type == ConstantPoolType::CONSTANT_MethodRef) {
			CP_MethodRef* methodRef = new CP_MethodRef(type);
			buf->get(&methodRef->classIndex);
			buf->get(&methodRef->nameAndTypeIndex);
			constantPool->cpInfos.push_back(methodRef);
		}
		else if (type == ConstantPoolType::CONSTANT_FieldRef) {
			CP_FieldRef* fieldRef = new CP_FieldRef(type);
			buf->get(&fieldRef->classIndex);
			buf->get(&fieldRef->nameAndTypeIndex);
			constantPool->cpInfos.push_back(fieldRef);
		}
		else if (type == ConstantPoolType::CONSTANT_InterfaceMethodRef) {
			CP_InterfaceMethodRef* interfaceMethodRef = new CP_InterfaceMethodRef(type);
			buf->get(&interfaceMethodRef->classIndex);
			buf->get(&interfaceMethodRef->nameAndTypeIndex);
			constantPool->cpInfos.push_back(interfaceMethodRef);
		}
		else if (type == ConstantPoolType::CONSTANT_String) {
			CP_String* cpString = new CP_String(type);
			buf->get(&cpString->utf8Index);
			constantPool->cpInfos.push_back(cpString);
		}
		else if (type == ConstantPoolType::CONSTANT_Integer) {
			CP_Integer* cpInteger = new CP_Integer(type);
			buf->get(&cpInteger->value);
			constantPool->cpInfos.push_back(cpInteger);
		}
		else if (type == ConstantPoolType::CONSTANT_Float) {
			CP_Float* cpFloat = new CP_Float(type);
			buf->get(&cpFloat->value);
			constantPool->cpInfos.push_back(cpFloat);
		}
		else if (type == ConstantPoolType::CONSTANT_Long) {
			CP_Long* cpLong = new CP_Long(type);
			buf->get(&cpLong->value);
			constantPool->cpInfos.push_back(cpLong);
			constantPool->cpInfos.push_back(new ConstantPoolInfo(ConstantPoolType::CONSTANT_Null));
			i++;
		}
		else if (type == ConstantPoolType::CONSTANT_Double) {
			CP_Double* cpLong = new CP_Double(type);
			buf->get(&cpLong->value);
			constantPool->cpInfos.push_back(cpLong);
			constantPool->cpInfos.push_back(new ConstantPoolInfo(ConstantPoolType::CONSTANT_Null));
			i++;
		}
		else if (type == ConstantPoolType::CONSTANT_NameAndType) {
			CP_NameAndType* cpNameAndType = new CP_NameAndType(type);
			buf->get(&cpNameAndType->nameIndex);
			buf->get(&cpNameAndType->descriptorIndex);
			constantPool->cpInfos.push_back(cpNameAndType);
		}
		else if (type == ConstantPoolType::CONSTANT_Utf8) {
			CP_Utf8* cpUtf8 = new CP_Utf8(type);
			cpUtf8->value = buf->readString(buf->get<u2>());
			constantPool->cpInfos.push_back(cpUtf8);
		}
		else if (type == ConstantPoolType::CONSTANT_MethodHandle) {
			CP_MethodHandle* cpMethodHandle = new CP_MethodHandle(type);
			buf->get(&cpMethodHandle->referenceKind);
			buf->get(&cpMethodHandle->referenceIndex);
			constantPool->cpInfos.push_back(cpMethodHandle);
		}
		else if (type == ConstantPoolType::CONSTANT_MethodType) {
			CP_MethodType* cpMethodType = new CP_MethodType(type);
			buf->get(&cpMethodType->descriptorIndex);
			constantPool->cpInfos.push_back(cpMethodType);
		}
		else if (type == ConstantPoolType::CONSTANT_InvokeDynamic) {
			CP_InvokeDynamic* cpInvokeDynamic = new CP_InvokeDynamic(type);
			buf->get(&cpInvokeDynamic->bootstrapMethodAttrIndex);
			buf->get(&cpInvokeDynamic->nameAndTypeIndex);
			constantPool->cpInfos.push_back(cpInvokeDynamic);
		}
		else if (type == ConstantPoolType::CONSTANT_Class) {
			CP_Class* cpClass = new CP_Class(type);
			buf->get(&cpClass->nameIndex);
			constantPool->cpInfos.push_back(cpClass);
		}
		else {
			std::cerr << "[-] Couldn't retrieve type -> " << (int)type << std::endl;
			exit(1);
		}
	}

	buf->get(&accessFlags);
	buf->get(&thisClass);
	buf->get(&superClass);

	buf->get(&interfaceCount);
	for (short i = 0; i < interfaceCount; i++) {
		interfaces.push_back(constantPool->getClassAt(buf->get<u2>()));
	}

	buf->get(&fieldCount);
	for (short i = 0; i < fieldCount; i++) {
		FieldInfo* field = new FieldInfo(constantPool);
		buf->get(&field->accessFlags);
		buf->get(&field->nameIndex);
		buf->get(&field->descriptorIndex);

		buf->get(&field->attributeCount);
		readAttributeList(field->attributeCount, &field->attributes);

		fields.push_back(field);
	}

	buf->get(&methodCount);
	for (short i = 0; i < methodCount; i++) {
		MethodInfo* method = new MethodInfo(constantPool);
		buf->get(&method->accessFlags);
		buf->get(&method->nameIndex);
		buf->get(&method->descriptorIndex);

		buf->get(&method->attributeCount);
		readAttributeList(method->attributeCount, &method->attributes);

		methods.push_back(method);
	}
	buf->get(&attributeCount);
	readAttributeList(attributeCount, &attributes);
}

bool JavaClass::hasAttribute(std::string name) {
	for (const auto& attr : attributes) {
		if (attr->getName().compare(name) == 0)
			return true;
	}
	return false;
}

AttributeInfo* JavaClass::getAttribute(std::string name) {
	if (hasAttribute(name)) {
		for (const auto& attr : attributes) {
			if (attr->getName().compare(name) == 0)
				return attr;
		}
	}
	return nullptr;
}
