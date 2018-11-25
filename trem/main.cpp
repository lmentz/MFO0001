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
	MapaTrem trens;

	Trem t1("t1", &rdp_trem, &trens);
	Trem t2("t2", &rdp_trem, &trens);

	// cout << &trens << endl
	// 	 << &rdp_trem << endl
	// 	 << &t1 << endl
	// 	 << &t2 << endl << endl;

	thread t1_thread(&Trem::startWorker, &t1);
	Thread::SleepMS(10);
	thread t2_thread(&Trem::startWorker, &t2);

	// trens.Gate(1);
   
	// for(float p = -1; p <= 1; p += 0.01)
	// {
	//     if(p < 0)
	//         trens.Trem1Pos((-1) * p, 1);
	//     else
	//         trens.Trem1Pos(p, 0);
	//     //trens.Trem1Pos(p.x,p.y);
	//     Thread::SleepMS(10);
		
	//     trens.Trem1Txt("a1=" + to_string((int) trens.A1()) + " a2=" + to_string((int) trens.A2()) + " b1=" + to_string((int) trens.B1()) + " b2=" + to_string((int) trens.B2()) + " c=" + to_string((int) trens.C()));
		
	//     int key = trens.GetLastKey();
	//     cout << "LastKey = " << key << endl;
	//     if(key == 27)
	//         return 1;
	// }
	
	// trens.Trem1Txt("Mensagem trem 1.");
	
	// trens.Gate(0);
	// for(float p = -1; p <= 1; p += 0.01)
	// {
	//     if(p < 0)
	//         trens.Trem2Pos((-1) * p, 2);
	//     else
	//         trens.Trem2Pos(p, 0);
	//     //trens.Trem1Pos(p.x,p.y);
	//     Thread::SleepMS(10);
		
	//     trens.Trem2Txt("a1=" + to_string((int) trens.A1()) + " a2=" + to_string((int) trens.A2()) + " b1=" + to_string((int) trens.B1()) + " b2=" + to_string((int) trens.B2()) + " c=" + to_string((int) trens.C()));
		
	//     int key = trens.GetLastKey();
	//     cout << "LastKey = " << key << endl;
	//     if(key == 27)
	//         return 1;
	// }
	
	// trens.Trem2Txt("Mensagem trem 2.");

	t1_thread.join();
	t2_thread.join();
	
	return 0;
}
