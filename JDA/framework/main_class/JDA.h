#pragma once

class JDA {
public:
	zip* jarFile;
public:
	std::vector<std::pair<std::string, JavaClass*>> classes;
	JDA() {};
	JDA(zip*);
public:
	JavaClass* getClass(std::string name);
	std::vector<JavaClass*> getClasses(std::string name);
	std::vector<JavaClass*> getClasses();
};


JDA::JDA(zip* archive) {
	this->jarFile = archive;
	for (auto name : this->jarFile->namelist())
	{
		if (name.ends_with(".class")) {
			auto data = this->jarFile->read(name);
			JavaClass* clazz = new JavaClass(new ByteBuffer(std::vector<byte>(data.begin(), data.end())));
			classes.push_back(std::make_pair(name, clazz));
		}
	}
}
JavaClass* JDA::getClass(std::string name) {
	// += .class 
	name = name.append(".class");
	for (const auto& clazz : classes) {
		if ((clazz.first).compare(name) == 0)
			return clazz.second;
	}
	return nullptr;
}
std::vector<JavaClass*> JDA::getClasses(std::string name) {
	std::vector<JavaClass*> return_value;
	for (const auto& clazz : classes) {
		if ((clazz.first).find(name) != std::string::npos)
			return_value.push_back(clazz.second);
	}
	return return_value;
}
std::vector<JavaClass*> JDA::getClasses() {
	std::vector<JavaClass*> return_value;
	for (const auto& clazz : classes) {
		return_value.push_back(clazz.second);
	}
	return return_value;
}