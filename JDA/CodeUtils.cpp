#include "CodeUtils.h"

int CodeUtils::getArgCount(JvmOpcode opcode) {
	switch (opcode) {
	case JvmOpcode::LOOKUPSWITCH:
	case JvmOpcode::TABLESWITCH:
	case JvmOpcode::WIDE:
		return -1;
	case JvmOpcode::ASTORE:
	case JvmOpcode::BIPUSH:
	case JvmOpcode::ALOAD:
	case JvmOpcode::DLOAD:
	case JvmOpcode::DSTORE:
	case JvmOpcode::FLOAD:
	case JvmOpcode::FSTORE:
	case JvmOpcode::ILOAD:
	case JvmOpcode::ISTORE:
	case JvmOpcode::LDC:
	case JvmOpcode::LLOAD:
	case JvmOpcode::LSTORE:
	case JvmOpcode::NEWARRAY:
	case JvmOpcode::RET:
		return 1;
	case JvmOpcode::ANEWARRAY:
	case JvmOpcode::CHECKCAST:
	case JvmOpcode::GOTO:
	case JvmOpcode::IFEQ:
	case JvmOpcode::IFGE:
	case JvmOpcode::IFGT:
	case JvmOpcode::IFLE:
	case JvmOpcode::IFLT:
	case JvmOpcode::IFNE:
	case JvmOpcode::IFNONNULL:
	case JvmOpcode::IFNULL:
	case JvmOpcode::IF_ACMPEQ:
	case JvmOpcode::IF_ACMPNE:
	case JvmOpcode::IF_ICMPEQ:
	case JvmOpcode::IF_ICMPNE:
	case JvmOpcode::IF_ICMPGE:
	case JvmOpcode::IF_ICMPGT:
	case JvmOpcode::IF_ICMPLE:
	case JvmOpcode::IF_ICMPLT:
	case JvmOpcode::INSTANCEOF:
	case JvmOpcode::INVOKESPECIAL:
	case JvmOpcode::INVOKESTATIC:
	case JvmOpcode::INVOKEVIRTUAL:
	case JvmOpcode::JSR:
	case JvmOpcode::LDC_W:
	case JvmOpcode::LDC2_W:
	case JvmOpcode::NEW:
	case JvmOpcode::PUTFIELD:
	case JvmOpcode::PUTSTATIC:
	case JvmOpcode::SIPUSH:
	case JvmOpcode::GETFIELD:
	case JvmOpcode::GETSTATIC:
		return 2;
	case JvmOpcode::MULTIANEWARRAY:
		return 3;
	case JvmOpcode::GOTO_W:
	case JvmOpcode::INVOKEDYNAMIC:
	case JvmOpcode::INVOKEINTERFACE:
	case JvmOpcode::JSR_W:
		return 4;
	default:
		return 0;
	}
}

std::vector<std::pair<JvmOpcode, std::vector<u1>>> CodeUtils::getOpcodesFromVector(std::vector<u1> arr) {
	std::vector<std::pair<JvmOpcode, std::vector<u1>>> res;
	for (int i = 0; i < arr.size(); i++) {
		JvmOpcode opcode = (JvmOpcode)arr.at(i);
		int argCount = getArgCount(opcode);
		std::vector<u1> args;

		if (argCount == -1) {
			if (opcode == JvmOpcode::LOOKUPSWITCH) {
				i += 3;
				u4 count = ((arr[i++] & 255) << 24) | ((arr[i++] & 255) << 16) | ((arr[i++] & 255) << 8) | (arr[i++] & 255);
				i += (8 * count) - 1;
			}
			else if (opcode == JvmOpcode::TABLESWITCH) {
				i += 3;

				int low = ((arr[i++] & 255) << 24) | ((arr[i++] & 255) << 16) | ((arr[i++] & 255) << 8) | (arr[i++] & 255);
				int high = ((arr[i++] & 255) << 24) | ((arr[i++] & 255) << 16) | ((arr[i++] & 255) << 8) | (arr[i++] & 255);

				i += (4 * (high - low + 1)) - 1;
			}
			else if (opcode == JvmOpcode::WIDE) {
				opcode = (JvmOpcode)arr[i];
				if (opcode == JvmOpcode::IINC)
					i += 4;
				else
					i += 2;
			}
		}
		else {
			for (int j = 0; j < argCount; j++) {
				args.push_back(arr[i + j + 1]);
			}
			i += argCount;
		}

		res.push_back(std::pair(opcode, args));
	}
	return res;
}
