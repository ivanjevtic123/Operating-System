#include "os_core.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	OSCore::load();

	int value = userMain(argc, argv);

	OSCore::unload();

	return value;
}
