#include <JDA.h>
#include <iostream>
#include <zip.h>

int main() {
	int error;
	zip* file = zip_open("C:\\Users\\noctu\\source\\repos\\JDA\\jars\\NFix.jar", ZIP_RDONLY, &error);
	JDA* jda = new JDA(file);
	
	
	
	return 0;
}