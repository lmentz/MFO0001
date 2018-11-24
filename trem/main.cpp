#include <iostream>
#include "class/mapatrem.h"
#include "class/threads/thread.h"
#include <vector>
#include <string>
#include "class/threadtest.h"
#include "petri/rdp.hpp"

using namespace std;

int main(int argc, char **argv) {
	/*
	ThreadTest teste(4);
	
	Thread::SleepMS(10000);
	
	return 0;
	*/

	// Exemplo: impressora
	vector<vector<int>> pre =
		{{1, 0, 0, 0, 0, 0},
		 {0, 1, 0, 0, 0, 0},
		 {0, 0, 1, 1, 0, 0},
		 {0, 0, 0, 0, 1, 0},
		 {0, 1, 0, 0, 0, 1}};
	vector<vector<int>> pos =
		{{0, 0, 0, 1, 0, 0},
		 {1, 0, 1, 0, 0, 0},
		 {0, 1, 0, 0, 0, 0},
		 {0, 0, 0, 0, 0, 1},
		 {0, 1, 0, 0, 1, 0}};
	vector<int> m0 =
		{1, 0, 0, 1, 0};
	vector<string> lugares =
		{
			"t1-p0",
			"t1-p1",
			"t1-p2",
			"t1-p3",
			"t1-p4"
		};
	vector<string> transicoes =
		{
			"t1-t0",
			"t1-t1",
			"t1-t2",
			"t1-t3",
			"t1-t4",
			"t1-t5"
		};

	PetriNet rdp_trem(pre, pos, m0, lugares, transicoes);

	MapaTrem trens;

	trens.Trem1Pos(1, 1);
	trens.Trem1Txt("Mensagem trem 1.");
	trens.Trem2Pos(1, 2);
	trens.Trem2Txt("Mensagem trem 2.");

	string t1_hab, t2_hab;

	for (int i = 0; i < 1000; i++){
		trens.Trem2Txt(rdp_trem.printEstado());

		Thread::SleepMS(2000);

		rdp_trem.lockRede();
		t1_hab = rdp_trem.getHabilitadoTrem("t1");
		rdp_trem.iniciaTransicao(t1_hab);
		trens.Trem1Txt("Rodando transicao " + t1_hab);
		trens.Trem2Txt(rdp_trem.printEstado());

		Thread::SleepMS(2000);
		
		rdp_trem.finalizaTransicao(t1_hab);
		trens.Trem1Txt("Finalizado transicao " + t1_hab);
		rdp_trem.unlockRede();

		// rdp_trem.lockRede();
		// t2_hab = rdp_trem.getHabilitadoTrem("t2");
		// rdp_trem.iniciaTransicao(t2_hab);
		// trens.Trem2Txt("Rodando transicao " + t2_hab);
		// Thread::SleepMS(2000);
		// rdp_trem.finalizaTransicao(t2_hab);
		// trens.Trem2Txt("Finalizado transicao " + t2_hab);
		// rdp_trem.unlockRede();

		// Thread::SleepMS(2000);
	}

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
	
	return 0;
}
