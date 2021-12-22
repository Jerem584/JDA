#include "JavaClass.h"

void JavaClass::readAttributeList(u2 count, std::vector<AttributeInfo*>* list) {
	for (short i = 0; i < count; i++) {
		AttributeInfo* attribute = new AttributeInfo(constantPool);
		buf->get(&attribute->attributeNameIndex);
		buf->get(&attribute->attributeLength);

		std::string name = attribute->getName();
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
		else if (name.compare("RuntimeVisibleAnnotations") == 0 || name.compare("RuntimeInvisibleAnnotations") == 0 || name.compare("RuntimeVisibleParameterAnnotations") == 0 || name.compare("RuntimeInvisibleParameterAnnotations") == 0 || name.compare("AnnotationDefault") == 0 || name.compare("SourceFile") == 0 || name.compare("SourceFileDebugExtension") == 0 || name.compare("BootstrapMethods") == 0) {
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

		list->push_back(attribute);
	}
}

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
			constantPool->cpInfos.push_back(new ConstantPoolInfo(ConstantPoolType::CONSTANT_Null));
			i ++;
		}
		else if (type == ConstantPoolType::CONSTANT_Double) {
			CP_Double* cpLong = new CP_Double(type);
			buf->get(&cpLong->value);
			constantPool->cpInfos.push_back(cpLong);
			constantPool->cpInfos.push_back(new ConstantPoolInfo(ConstantPoolType::CONSTANT_Null));
			i ++;
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
		if(attr->getName().compare(name) == 0)
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
