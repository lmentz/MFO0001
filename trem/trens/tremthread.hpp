#ifndef __TrEmDePeTrI__
#define __TrEmDePeTrI__
// hurrrr

// #include <vector>
// #include <mutex>
// #include <sstream>
#include <string>
#include <thread>
#include <queue>
#include "../petri/rdp.hpp"

typedef unsigned int uint;

class Trem {
  private:
	std::string nome;
	PetriNet *rdp;
	MapaTrem *trens;
	double tremPos;
	double tremSpeed;
	std::string ordem;
	// std::queue<int> fila_entrada;
	// std::queue<std::string> fila_estado;

	void tremPrint(std::string txt)
	{
		if (nome == "t1")
			trens->Trem1Txt(txt);
		else if (nome == "t2")
			trens->Trem2Txt(txt);
	}

	void updatePosTrem()
	{
		if (nome == "t1"){
			if (tremPos < 0){
				trens->Trem1Pos((-1) * tremPos, 1);
			}
			else{
				trens->Trem1Pos(tremPos, 2);
			}
		}
	}

	void rodaTrans(std::string trans)
	{
		if (trans == "t1-t0")
		{ // Partir de Lucerne
			tremPrint("Saindo da estação");
			ordem = "r";
			Thread::SleepMS(5000);
		}
		else if (trans == "t1-t1")
		{ // Aguardar B1 ligar
			while (true)
			{
				tremPrint("Em trânsito");
				if (trens->B1())
				{
					tremPrint("Aproximando de Stans");
					break;
				}
				Thread::SleepMS(25);
			}
		}
		else if (trans == "t1-t2")
		{ // T1 prossegue por Stans sem parar
		}
		else if (trans == "t1-t3")
		{ // T1 para em Stans
		}
		else if (trans == "t1-t4")
		{ //
		}
		else if (trans == "t1-t5")
		{ //
		}
		else if (trans == "t1-t6")
		{ //
		}
		else if (trans == "t1-t7")
		{ //
		}
		else if (trans == "t1-t8")
		{ //
		}
		else if (trans == "t1-t9")
		{ //
		}
		else if (trans == "t1-t10")
		{ //
		}
		else if (trans == "t1-t11")
		{ //
		}
		else if (trans == "t2-t12")
		{ //
		}
		else if (trans == "t2-t13")
		{ //
		}
		else if (trans == "t2-t14")
		{ //
		}
		else if (trans == "t2-t15")
		{ //
		}
		else if (trans == "t2-t16")
		{ //
		}
		else if (trans == "t2-t17")
		{ //
		}
		else if (trans == "t2-t18")
		{ //
		}
		else if (trans == "t2-t19")
		{ //
		}
		else if (trans == "t2-t20")
		{ //
		}
		else if (trans == "t2-t21")
		{ //
		}
		else if (trans == "t2-t22")
		{ //
		}
		else if (trans == "t2-t23")
		{ //
		}
	}

	void tremRunner()
	{
		while (true)
		{
			if (ordem == "r")
			{
				tremSpeed = MAX(tremSpeed + 0.02, 1);
			}
			else if (ordem == "l")
			{
				tremSpeed = MIN(tremSpeed - 0.02, -1);
			}
			else if (ordem == "s")
			{
				if (tremSpeed > 0)
				{
					tremSpeed = MAX(tremSpeed - 0.02, 0);
				}
				else if (tremSpeed < 0)
				{
					tremSpeed = MIN(tremSpeed + 0.02, 0);
				}
			}
			tremPos += tremSpeed;
			updatePosTrem();
			Thread::SleepMS(50);
		}
	}

  public:
	Trem(std::string nome, PetriNet *rdp, MapaTrem *trens){
		this->nome      = nome;
		this->rdp       = rdp;
		this->trens     = trens;
		this->tremPos   = -1.00;
		this->tremSpeed = 0.00;
		this->ordem     = "s";
	}

	void startWorker(){
		// cout << this->nome << endl
		// 	<< this->trens << endl
		// 	<< this->rdp << endl
		// 	<< this << endl << endl;

		std::thread threadRunner(&Trem::tremRunner, this);
		std::string trans_hab;
		while (true){
			// trens->Trem2Txt(rdp->printEstado());

			Thread::SleepMS(2000);

			rdp->lockRede();
			trans_hab = rdp->getHabilitadoTrem(nome);

			// Caso não haja transição habilitada, desbloqueia, dorme um pouco e tenta novamente.
			if (trans_hab == ""){
				rdp->unlockRede();
				tremPrint("Nada a fazer...");
				Thread::SleepMS(500);
				continue;
			}

			rdp->iniciaTransicao(trans_hab);
			//tremPrint("Rodando transicao " + trans_hab + rdp->printEstado());
			rdp->unlockRede();

			this->rodaTrans(trans_hab);

			rdp->lockRede();
			rdp->finalizaTransicao(trans_hab);
			// tremPrint("Finalizado transicao " + trans_hab + rdp->printEstado());
			rdp->unlockRede();
		}
		threadRunner.join();
	}
};



#endif