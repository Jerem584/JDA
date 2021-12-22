#include "JavaClass.h"

void JavaClass::read() {
	buf->skip(4);
	buf->get(&minorVersion);
	buf->get(&majorVersion);

	buf->get(&constantPoolCount);
	for (int i = 0; i < constantPoolCount-1; i++) {
		ConstantPoolType type = (ConstantPoolType)buf->get<u1>();
		if (type == ConstantPoolType::CONSTANT_MethodRef) {
			CP_MethodRef* methodRef = new CP_MethodRef(type);
			buf->get(&methodRef->classIndex);
			buf->get(&methodRef->nameAndTypeIndex);
			cpInfos.push_back(methodRef);
		} else if(type == ConstantPoolType::CONSTANT_FieldRef) {
			CP_FieldRef* fieldRef = new CP_FieldRef(type);
			buf->get(&fieldRef->classIndex);
			buf->get(&fieldRef->nameAndTypeIndex);
			cpInfos.push_back(fieldRef);
		} else if (type == ConstantPoolType::CONSTANT_InterfaceMethodRef) {
			CP_InterfaceMethodRef* interfaceMethodRef = new CP_InterfaceMethodRef(type);
			buf->get(&interfaceMethodRef->classIndex);
			buf->get(&interfaceMethodRef->nameAndTypeIndex);
			cpInfos.push_back(interfaceMethodRef);
		}
		else if (type == ConstantPoolType::CONSTANT_String) {
			CP_String* cpString = new CP_String(type);
			buf->get(&cpString->utf8Index);
			cpInfos.push_back(cpString);
		}
		else if (type == ConstantPoolType::CONSTANT_Integer) {
			CP_Integer* cpInteger = new CP_Integer(type);
			buf->get(&cpInteger->value);
			cpInfos.push_back(cpInteger);
		}
		else if (type == ConstantPoolType::CONSTANT_Float) {
			CP_Float* cpFloat = new CP_Float(type);
			buf->get(&cpFloat->value);
			cpInfos.push_back(cpFloat);
		}
		else if (type == ConstantPoolType::CONSTANT_Long) {
			CP_Long* cpLong = new CP_Long(type);
			buf->get(&cpLong->value);
			cpInfos.push_back(cpLong);
		}
		else if (type == ConstantPoolType::CONSTANT_Double) {
			CP_Double* cpLong = new CP_Double(type);
			buf->get(&cpLong->value);
			cpInfos.push_back(cpLong);
		}
		else if (type == ConstantPoolType::CONSTANT_NameAndType) {
			CP_NameAndType* cpNameAndType = new CP_NameAndType(type);
			buf->get(&cpNameAndType->nameIndex);
			buf->get(&cpNameAndType->descriptorIndex);
			cpInfos.push_back(cpNameAndType);
		}
		else if (type == ConstantPoolType::CONSTANT_Utf8) {
			CP_Utf8* cpUtf8 = new CP_Utf8(type);
			cpUtf8->value = buf->readString(buf->get<u2>());
			cpInfos.push_back(cpUtf8);
		}
		else if (type == ConstantPoolType::CONSTANT_MethodHandle) {
			CP_MethodHandle* cpMethodHandle = new CP_MethodHandle(type);
			buf->get(&cpMethodHandle->referenceKind);
			buf->get(&cpMethodHandle->referenceIndex);
			cpInfos.push_back(cpMethodHandle);
		}
		else if (type == ConstantPoolType::CONSTANT_MethodType) {
			CP_MethodType* cpMethodType = new CP_MethodType(type);
			buf->get(&cpMethodType->descriptorIndex);
			cpInfos.push_back(cpMethodType);
		}
		else if (type == ConstantPoolType::CONSTANT_InvokeDynamic) {
			CP_InvokeDynamic* cpInvokeDynamic = new CP_InvokeDynamic(type);
			buf->get(&cpInvokeDynamic->bootstrapMethodAttrIndex);
			buf->get(&cpInvokeDynamic->nameAndTypeIndex);
			cpInfos.push_back(cpInvokeDynamic);
		}
		else if (type == ConstantPoolType::CONSTANT_Class) {
			CP_Class* cpClass = new CP_Class(type);
			buf->get(&cpClass->nameIndex);
			cpInfos.push_back(cpClass);
		}
		else {
			std::cerr << "Ptdr c'est quoi ça poto ? " << (int)type << std::endl;
			exit(1);
		}
	}
}