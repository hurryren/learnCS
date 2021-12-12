#include <iostream>
#include<thread>
#include "xdata.h"
#include "test.h"

using namespace std;

void ThreadMain(){
	cout<<"Thread Main"<<endl;
}

int main(int argc, char *argv[]){
	cout<<"test make "<<endl;
	
	
	thread th(ThreadMain);	
	th.join();
	XData d();

	cout<<"test finish"<<endl;
	return 0;

}
