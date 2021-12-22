#include "JavaClass.h"

void JavaClass::read() {
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
		}
		else if (type == ConstantPoolType::CONSTANT_Double) {
			CP_Double* cpLong = new CP_Double(type);
			buf->get(&cpLong->value);
			constantPool->cpInfos.push_back(cpLong);
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
			std::cerr << "Ptdr c'est quoi ça poto ? " << (int)type << std::endl;
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
}