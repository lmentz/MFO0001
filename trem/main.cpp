#include <iostream>
#include "class/mapatrem.h"
#include "class/threads/thread.h"
#include <vector>
#include <string>
#include "class/threadtest.h"
#include "petri/rdp.hpp"
#include "trens/tremthread.hpp"
#include "trens/rdpdef.hpp"

using namespace std;

int main(int argc, char **argv){
	RDPTrens tdi; // Contém as definições da rede de petri
	PetriNet rdp_trem(tdi.pre, tdi.pos, tdi.m0,
					  tdi.lugares, tdi.transicoes);
	MapaTrem trens; // Controle da interface

	trens.Trem1Txt("Acione o trem 1 com a tecla M");
	trens.Trem2Txt("Acione o trem 2 com a tecla G");

	Trem t1("T1", &rdp_trem, &trens); 
	Trem t2("T2", &rdp_trem, &trens);

	// cout << &trens << endl
	// 	 << &rdp_trem << endl
	// 	 << &t1 << endl
	// 	 << &t2 << endl << endl;

	thread t1_thread(&Trem::startWorker, &t1);
	// Thread::SleepMS(10);
	thread t2_thread(&Trem::startWorker, &t2);

	t1_thread.join();
	t2_thread.join();
	
	return 0;
}
