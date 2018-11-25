#ifndef __TrEmDePeTrI__
#define __TrEmDePeTrI__
// hurrrr

// #include <vector>
#include <string>
#include <thread>
#include <chrono>
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

	void tremPrint(std::string txt)
	{
		if (nome == "T1")
			trens->Trem1Txt(txt);
		else if (nome == "T2")
			trens->Trem2Txt(txt);
	}

	void updatePosTrem()
	{
		if (nome == "T1"){
			if (tremPos < 0){
				trens->Trem1Pos((-1) * tremPos, 1);
			}
			else{
				trens->Trem1Pos(tremPos, 0);
			}
		}
		else if (nome == "T2"){
			if (tremPos < 0){
				trens->Trem2Pos((-1) * tremPos, 2);
			}
			else{
				trens->Trem2Pos(tremPos, 0);
			}
		}
		// printf("%s speed = %f\n", nome.data(), tremSpeed);
	}

	void tremRunner()
	{
		double accel = 0.0001; // units/second²
		double max_speed = 0.001;
		auto start = std::chrono::high_resolution_clock::now(), finish = start;

		while (true)
		{
			finish = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = finish - start;
			start = std::chrono::high_resolution_clock::now();

			double dif = accel * elapsedTime.count();
			if (ordem == "r")
			{
				tremSpeed = MIN(tremSpeed + dif, max_speed);
			}
			else if (ordem == "l")
			{
				tremSpeed = MAX(tremSpeed - dif, -max_speed);
			}
			else if (ordem == "s")
			{
				if (tremSpeed > 0)
				{
					tremSpeed = MAX(tremSpeed - 1.7*dif, 0);
				}
				else if (tremSpeed < 0)
				{
					tremSpeed = MIN(tremSpeed + 1.7*dif, 0);
				}
			}
			else if (ordem == "s-d")
			{
				if (tremSpeed > 0)
				{
					tremSpeed = MAX(tremSpeed - 0.74*dif, 0);
				}
			}
			else if (ordem == "s-e")
			{
				if (tremSpeed < 0)
				{
					tremSpeed = MIN(tremSpeed + 1.18*dif, 0);
				}
			}
			tremPos += tremSpeed;
			tremPos = MIN(1, MAX(-1, tremPos));
			updatePosTrem();
			Thread::SleepMS(20);
		}
	}

	void rodaTrans(std::string trans)
	{
		if (trans == "T1-t0")
		{ // Partir de Lucerne
			tremPrint("Saindo da estacao Lucerne");
			Thread::SleepMS(rand() % 1000);
			this->ordem = "r";
			Thread::SleepMS(1000);
		}
		
		else if (trans == "T1-t1")
		{ // Aguardar B1 ligar
			while (true)
			{
				if (trens->A1())
				{
					tremPrint("Em transito");
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}
			while (true)
			{
				if (trens->B1())
				{
					tremPrint("Aproximando de Stans");
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}
		}
		
		else if (trans == "T1-t2")
		{ // T1 prossegue por Stans sem parar
			trens->Gate(1);
			tremPrint("Caminho livre, prosseguir");
			// Thread::SleepMS(1000);
			//nao pode ter tempo pq senao buga
		}

		else if (trans == "T1-t3")
		{ // Parar trem 1
			this->ordem = "s";
			tremPrint("Parar, caminho em uso");
			Thread::SleepMS(2000); //2000ms? éh
			//nao pode ter tempo pq senao buga
		}

		else if (trans == "T1-t4")
		{ // T1 pega Mutex (vai para estado T1 verificar caminho)
			trens->Gate(1);
			tremPrint("Seguir, caminho liberado");
			this->ordem = "r";
			//nao pode ter tempo pq senao buga
		}

		else if (trans == "T1-t5")
		{ // T1 seguir pelo gate
			while (tremPos < 0) {
				Thread::SleepMS(100);
			}
			tremPrint("Seguindo viagem");
		}
		
		else if (trans == "T1-t6")
		{ // T1 apertar m1 para saida '-'
			while (true){
				// printf("%d %d\n", 'm', trens->GetLastKey());
				if (trens->GetLastKey() == 'm'){
					tremPrint("Ultima chamada (embarque)");
					trens->GetLastKeyAndEmpty(); // limpar
					break;
				}
				Thread::SleepMS((rand() % 10) + 5);
			}
			Thread::SleepMS(4000);
			tremPrint("T1 partindo");
			this->ordem = "r";
		}

		else if (trans == "T1-t7")
		{ // sensor C=1, T1 = S
			tremPrint("Aproximando de Engelberg");
			while (true)
			{
				// tremPrint("Em transito");
				if (trens->C())
				{
					tremPrint("Chegando em Engelberg (frear)");
					this->ordem = "s-d";
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}

			// Aguardar parar
			while (tremSpeed > 0)
			{
				Thread::SleepMS(10);
			}
			tremPrint("Chegada em Engelberg (desembarque)");
			Thread::SleepMS(10000);
		}

		else if (trans == "T1-t8")
		{ // Saindo de engelberg
			tremPrint("Saindo de Engelberg (embarque)");
			Thread::SleepMS(1000); //inverteu o sleep pra esperar embarque antes de começar a ir pra left
			this->ordem = "l";
		}
		
		else if (trans == "T1-t9")
		{ // devolve o mutex, ou seja, gate muda de valor
			tremPrint("Aproximando de Stans");
			while (true)
			{
				if (trens->B1())
				{
					tremPrint("Pronto, outro trem. Sua vez");
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}
			Thread::SleepMS(3000);
			//ok go ahead
			//nao pode ter tempo pq senao buga
		}
		
		else if (trans == "T1-t10")
		{ // T1 segue viagem (está saindo do GATE e voltando pra origem!!!)
			tremPrint("Trem 1 segue viagem");
		}
		
		else if (trans == "T1-t11")
		{ // Aparentemente T1 chegada em Lucerne, deverá parar!!!1111!!!
			while (true)
			{
				if (trens->A1())
				{
					tremPrint("Chegando em Lucerne");
					this->ordem = "s-e";
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}

			Thread::SleepMS(4000);
			tremPrint("Desembarque em Lucerne");
			Thread::SleepMS(4000);
			tremPrint("Aguardando comando para partida");
		}

		else if (trans == "T2-t12")
		{ // Parar trem 2
			this->ordem = "s";
			tremPrint("Parar, caminho em uso");
			Thread::SleepMS(2000); //2000ms? éh
			//nao pode ter tempo pq senao buga
		}
		else if (trans == "T2-t13")
		{ // T2 prossegue por Stans sem parar
			trens->Gate(0);
			tremPrint("Caminho livre, prosseguir");
			// Thread::SleepMS(1000);
			//nao pode ter tempo pq senao buga
		}
		else if (trans == "T2-t14")
		{ // T1 teve liberação para prosseguir
			trens->Gate(0);
			tremPrint("Seguir, caminho liberado");
			this->ordem = "r";
			//nao pode ter tempo pq senao buga
		}
		else if (trans == "T2-t15")
		{ // Seguir viagem
			while (tremPos < 0)
			{
				Thread::SleepMS(100);
			}
			tremPrint("Seguindo viagem");
		}
		else if (trans == "T2-t16")
		{ //
			tremPrint("Aproximando de Engelberg");
			while (true)
			{
				// tremPrint("Em transito");
				if (trens->C())
				{
					tremPrint("Chegando em Engelberg (frear)");
					this->ordem = "s-d";
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}

			// Aguardar parar
			while (tremSpeed > 0)
			{
				Thread::SleepMS(10);
			}
			tremPrint("Chegada em Engelberg (desembarque)");
			Thread::SleepMS(10000);
		}
		else if (trans == "T2-t17")
		{ //
			tremPrint("Saindo de Engelberg (embarque)");
			Thread::SleepMS(1000); //inverteu o sleep pra esperar embarque antes de começar a ir pra left
			this->ordem = "l";
		}
		else if (trans == "T2-t18")
		{ //
			while (true)
			{
				if (trens->C())
				{
					tremPrint("Aproximando de Stans");
					this->ordem = "s-d";
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}
			
			while (true)
			{
				if (trens->B2())
				{
					tremPrint("Pronto, outro trem. Sua vez");
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}
			Thread::SleepMS(3000);
			//ok go ahead
			//nao pode ter tempo pq senao buga
		}
		else if (trans == "T2-t19")
		{ //
			tremPrint("Trem 2 segue viagem");
		}
		else if (trans == "T2-t20")
		{ //
			while (true)
			{
				if (trens->A2())
				{
					tremPrint("Chegando em Sarnen");
					this->ordem = "s-e";
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}

			Thread::SleepMS(4000);
			tremPrint("Desembarque em Sarnen");
			Thread::SleepMS(4000);
			tremPrint("Aguardando comando para partida");
		}
		else if (trans == "T2-t21")
		{ // Partir de Sarnen
			tremPrint("Saindo da estacao Sarnen");
			Thread::SleepMS(rand() % 1000);
			this->ordem = "r";
			Thread::SleepMS(1000);
		}
		else if (trans == "T2-t22")
		{ //
			while (true)
			{
				// printf("%d %d\n", 'g', trens->GetLastKey());
				if (trens->GetLastKey() == 'g')
				{
					tremPrint("Ultima chamada (embarque)");
					trens->GetLastKeyAndEmpty(); // limpar
					break;
				}
				Thread::SleepMS((rand() % 10) + 5);
			}
			Thread::SleepMS(4000);
			tremPrint("T1 partindo");
			this->ordem = "r";
		}
		else if (trans == "T2-t23")
		{ // Aguardar B2 ligar
			while (true)
			{
				if (trens->A2())
				{
					tremPrint("Em transito");
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}
			while (true)
			{
				if (trens->B2())
				{
					tremPrint("Aproximando de Stans");
					break;
				}
				Thread::SleepMS(1); // 25 é muito, ele perde o sensor as vezes.
			}
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

			Thread::SleepMS(rand() % 50);

			rdp->lockRede();
			trans_hab = rdp->getHabilitadoTrem(nome);

			printf("Trem %s -> transição start %s\n", nome.data(), trans_hab.data());

			// Caso não haja transição habilitada, desbloqueia, dorme um pouco e tenta novamente.
			if (trans_hab == ""){
				rdp->unlockRede();
				// tremPrint("Nada a fazer...");
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

			// printf("Trem %s -> transição end   %s\n", nome.data(), trans_hab.data());
		}
		threadRunner.join();
	}
};



#endif