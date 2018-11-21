#ifndef __PeTrInEt__
#define __PeTrInEt__

#include <vector>
#include <string>

class PetriNet {
private:
	std::vector<std::vector<int> > pre; // Linhas = Lugares; Colunas = Transicoes
	std::vector<std::vector<int> > pos; // Linhas = Lugares; Colunas = Transicoes
	std::vector<int> m; // Marcação atual
	std::vector<std::string> lugares; // Nomes apenas
	std::vector<std::string> transicoes; // Nomes apenas

public:
	PetriNet(std::vector<std::vector<int> > pre, std::vector<std::vector<int> > pos, std::vector<int> m0,
			 std::vector<std::string> lugares, std::vector<std::string> transicoes){
		this->pre = pre;
		this->pos = pos;
		this->m = m0;
		// Carregar nomes das coisas
		this->lugares = lugares;
		this->transicoes = transicoes;
	}
	
	std::string getHabilitadoTrem(std::string trem) {
		for(int i = 0; i < transicoes.size(); i++){
			if(transicoes[i].find(trem) != std::string::npos) {
				bool habilitado = true;
				
				// mutex lock na coluna i do pre
				for(int j = 0; j < m.size(); j++){
					if(m[j] < pre[j][i]){
						habilitado = false;
						break;
					}
				}
				// mutex unlock na coluna i do pre
				
				if (habilitado) return transicoes[i];
			}
		}
		return "";
	}

	void iniciaTransicao(std::string trans){
		// mutex lock global
		// m = m - pre; da transição trans
		// mutex unlock global
	}

	void finalizaTransicao(std::string trans){
		// mutex lock global
		// m = m + post; da transição trans
		// mutex unlock global
	}
};


#endif