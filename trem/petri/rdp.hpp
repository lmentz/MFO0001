#ifndef __PeTrInEt__
#define __PeTrInEt__

#include <vector>
#include <string>
#include <mutex>
#include <sstream>

typedef unsigned int uint;

class PetriNet {
private:
	std::vector<std::vector<int> > pre;  // Linhas = Lugares; Colunas = Transicoes
	std::vector<std::vector<int> > pos;  // Linhas = Lugares; Colunas = Transicoes
	std::vector<int> m;                  // Marcação atual
	std::vector<std::string> lugares;    // Nomes apenas
	std::vector<std::string> transicoes; // Nomes apenas
	std::vector<std::mutex> mut_trans;   // Mutex de transição (coluna no pre/pos)
	std::vector<std::mutex> mut_lugar;   // Mutex de lugar (linha no pre/pos)
	std::mutex mut_global;               // Mutex global da rede

	int findTransicao(std::string trans){
		for (uint i = 0; i < transicoes.size(); i++){
			if (trans == transicoes[i]) return int(i);
		}
		return -1;
	}

	bool getTransacaoHabilitada(std::string trans)
	{
		int i = findTransicao(trans);
		bool habilitado = true;
		// mutex lock na coluna i do pre
		mut_trans[i].lock();
		for (uint j = 0; j < m.size(); j++)
		{
			// mut_lugar[j].lock();
			if (m[j] < pre[j][i])
			{
				habilitado = false;
				break;
			}
			// mut_lugar[j].unlock();
		}
		mut_trans[i].unlock(); // mutex unlock na coluna i do pre

		return habilitado;
	}

public:
	PetriNet(std::vector<std::vector<int> > pre, std::vector<std::vector<int> > pos, std::vector<int> m0,
			 std::vector<std::string> lugares, std::vector<std::string> transicoes){
		this->pre = pre;
		this->pos = pos;
		this->m = m0;
		// Carregar nomes das coisas
		this->lugares = lugares;
		this->transicoes = transicoes;
		this->mut_trans = std::vector<std::mutex> (transicoes.size());
		this->mut_lugar = std::vector<std::mutex> (lugares.size());
	}

	void lockRede(){
		mut_global.lock();
	}

	void unlockRede(){
		mut_global.unlock();
	}

	std::string getHabilitadoTrem(std::string trem) {
		std::vector<std::string> habilitados;
		for (uint i = 0; i < transicoes.size(); i++){
			if (transicoes[i].find(trem) != std::string::npos) {
				bool habilitado = true;
				
				// mutex lock na coluna i do pre
				// mut_trans[i].lock();
				for (uint j = 0; j < m.size(); j++){
					// mut_lugar[j].lock();
					if (m[j] < pre[j][i]){
						habilitado = false;
						break;
					}
					// mut_lugar[j].unlock();
				}
				// mut_trans[i].unlock(); // mutex unlock na coluna i do pre
				
				if (habilitado) habilitados.push_back(transicoes[i]);
			}
		}
		
		if (habilitados.size() > 0) {
			int elem_aleatorio = rand() % habilitados.size();
			return habilitados[elem_aleatorio];
		}

		return "";
	}

	int iniciaTransicao(std::string trans){
		int ret = 0;
		// m = m - pre; da transição trans
		int i = findTransicao(trans);
		if (i == -1 || !getTransacaoHabilitada(trans)){ // Falha
			ret = 1;
			printf("CAGOUUUU\n");
		}
		else {
			for (uint j = 0; j < lugares.size(); j++){
				m[j] -= pre[j][i];
			}
		}
		return ret;
	}

	int finalizaTransicao(std::string trans){
		int ret = 0;
		// m = m + post; da transição trans
		int i = findTransicao(trans);
		if (i == -1){ // Falha
			ret = 1;
			print("CAGOUUUU\n");
		}
		else
		{
			for (uint j = 0; j < lugares.size(); j++)
			{
				m[j] += pos[j][i];
			}
		}
		return ret;
	}

	std::string printEstado(){
		std::stringstream estado;
		for (uint i = 0; i < lugares.size(); i++){
			estado << m[i];
		}
		return estado.str();
	}
};


#endif