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

    std::string proxTransicao(){
        std::vector<int> ligados(transicoes.size(), 0);
        for(int i = 0; i < transicoes.size(); i++){
            bool habilitado = true;
            for(int j = 0; j < transicoes[i].size(); j++){
                if (m[j] < )
            }
        }
    }

};


#endif