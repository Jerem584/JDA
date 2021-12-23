#pragma once

#include "common.h"

class CodeUtils {
public:
	static int getArgCount(JvmOpcode);
	static std::vector<std::pair<JvmOpcode, std::vector<u1>>> getOpcodesFromVector(std::vector<u1>);

	static u2 getLdc2WIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u2 getLdc2Index(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u2 getLdcIndex(std::vector<u1> args) {
		return args[0];
	}

	static u2 getFieldOpcodeIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u2 getNewIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u2 getANewArrayIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u2 getCheckcastIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u2 getGotoIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u4 getGotoWIndex(std::vector<u1> args) {
		return args[0] << 24 | args[1] << 16 | args[2] << 8 | args[3];
	}

	static u2 getIfIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u2 getInstanceOfIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u2 getInvokeIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u2 getJsrIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u4 getJsrWIndex(std::vector<u1> args) {
		return args[0] << 24 | args[1] << 16 | args[2] << 8 | args[3];
	}

	static u2 getMultiANewArrayIndex(std::vector<u1> args) {
		return args[0] << 8 | args[1];
	}

	static u1 getMultiANewArrayDimensionIndex(std::vector<u1> args) {
		return args[2];
	}
};