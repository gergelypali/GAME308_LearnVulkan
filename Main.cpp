#define SDL_MAIN_HANDLED

#include <stdlib.h>  
#include <string>
#include "SceneManager.h"
#include "Debug.h"
#include "MMath.h"

static long total = 0;
void* operator new(std::size_t amount) {
  ///  std::cout << "allocating " << amount << " bytes of memory\n";
	total += amount;
    return malloc(amount);
}

void operator delete(void* memoryLocation, std::size_t amount) {
  ///  std::cout << "freeing " << amount << " bytes of memory\n";
	total -= amount;
    free(memoryLocation);
}

using namespace MATH;
 
int main(int argc, char* args[]) {
	std::string name = { "Graphics Game Engine" };
	std::cout << total << std::endl;
	Debug::DebugInit(name + "_Log");
	std::cout << total << std::endl;
	Debug::Info("Starting the GameSceneManager", __FILE__, __LINE__);
	std::cout << total << std::endl;
	SceneManager* gsm = new SceneManager();
	if (gsm->Initialize(name, 1280, 720) ==  true) {
		gsm->Run();
	} 
	delete gsm;
	
	std::cout << total << std::endl;
	exit(0);

}