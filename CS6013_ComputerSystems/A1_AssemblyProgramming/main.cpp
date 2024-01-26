#include <sys/time.h>
#include <iostream>

extern "C" {
	void sayHello(); 
	int myPuts(const char *str, int len);
       timeval myGTOD(); 	
}

int main(int argc, char *argv[]){

	sayHello();

	myPuts("Hi!", 3);

	timeval currentTime = myGTOD(); 
	timeval expectedTime; 
	gettimeofday(&expectedTime, nullptr);

	std::cout << "\nExpected seconds: " << expectedTime.tv_sec; 
	std::cout << "\nExpected microseconds: " << expectedTime.tv_usec; 
	
	std::cout << "\nCurrent seconds: " << expectedTime.tv_sec; 
	std::cout << "\nCurrent microseconds: " << expectedTime.tv_usec; 

return 0; 	
}
