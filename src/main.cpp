#include "ObjectsInMotion.hpp"

int main(int argc, char* argv[])
{
	if(argc == 2)
	{
		std::string fn = std::string(argv[1]);
		
		ObjectsInMotionEngine eng(fn);
		return eng.run().shutdown();
	}
	else
	{
		ObjectsInMotionEngine eng;
		return eng.run().shutdown();
	}
}
