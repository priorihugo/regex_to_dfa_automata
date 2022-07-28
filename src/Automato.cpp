#include "Automato.h"

Automato::Automato()
{
}

Automato::~Automato()
{
}

void Automato::criaAutomato(string exp)
{
    vector<Automato *> heap;
    for (int i = 0; i < exp.length(); i++)
    {
        /// faixa de simbolos validos
        if (int(exp[i] >= 32 && int(exp[i]) <= 126))
        {
            switch (exp[i])
            {
            case '+':
                if (heap.size() >= 2)
                {
                    // cria um automato auxiliar que sera a junção dos dois ultimos da heap
                    Automato *aux = new Automato();
                    Automato *A1 = heap.back();
                    heap.pop_back();
                    Automato *A2 = heap.back();
                    heap.pop_back();
                    // novo estado inicial e final
                    Estado *inicio = new Estado();
                    inicio->inicio = true;
                    Estado *fim = new Estado();
                    fim->fim = true;
// transiçoes que ligam o novo inicio ao inicio dos automatos antigos
#define VET A1->EstadosIniciais
                    for (vector<Estado *>::iterator it = VET.begin(); it != VET.end(); it++)
                    {
                        Transicao *iA1 = new Transicao();
                        iA1->tipo = '~';
                        iA1->destino = *it;
                        (*it)->inicio = false;
                        inicio->transicoes.push_back(iA1);
                    }
                    // limpa a lista de estados iniciais de A1
                    VET.clear();

#define VET2 A2->EstadosIniciais
                    for (vector<Estado *>::iterator it = VET2.begin(); it != VET2.end(); it++)
                    {
                        Transicao *iA2 = new Transicao();
                        iA2->tipo = '~';
                        iA2->destino = *it;
                        (*it)->inicio = false;
                        inicio->transicoes.push_back(iA2);
                    }
                    VET2.clear();

/// ligando os estados finais
#define VETF A1->EstadosFinais
                    for (vector<Estado *>::iterator it = VETF.begin(); it != VETF.end(); it++)
                    {
                        Transicao *fA1 = new Transicao();
                        fA1->tipo = '~';
                        fA1->destino = fim;
                        (*it)->fim = false;
                        (*it)->transicoes.push_back(fA1);
                    }
                    VETF.clear();

#define VETF2 A2->EstadosFinais
                    for (vector<Estado *>::iterator it = VETF2.begin(); it != VETF2.end(); it++)
                    {
                        Transicao *fA2 = new Transicao();
                        fA2->tipo = '~';
                        fA2->destino = fim;
                        (*it)->fim = false;
                        (*it)->transicoes.push_back(fA2);
                    }
                    VETF2.clear();

                    /// por fim copiando os estados dos 2 ultimos automatos para o aux
                    aux->Estados.push_back(inicio);
                    for (vector<Estado *>::iterator it = A1->Estados.begin(); it != A1->Estados.end(); it++)
                    {
                        aux->Estados.push_back(*it);
                    }
                    for (vector<Estado *>::iterator it = A2->Estados.begin(); it != A2->Estados.end(); it++)
                    {
                        aux->Estados.push_back(*it);
                    }
                    aux->Estados.push_back(fim);

                    // joga aux na heap
                    heap.push_back(aux);

                    // delete na referencia para os 2 automatos anteriores
                    delete A1;
                    delete A2;
                }
                break;
            case '.':
                if (heap.size() >= 2)
                {
                }
                break;
            case '*':
                if (heap.size() >= 1)
                {
                }
                break;
            default:
                // automato simples com uma transição
                Automato *aux = new Automato();
                Estado *E1 = new Estado();
                Estado *E2 = new Estado();

                // seta inicio e fim
                E1->inicio = true;
                aux->EstadosIniciais.push_back(E1);
                E2->fim = true;
                aux->EstadosFinais.push_back(E2);

                // cria a transicao e o tipo dela
                Transicao *E1E2 = new Transicao();
                E1E2->destino = E2;
                E1E2->tipo = exp[i];
                E1->transicoes.push_back(E1E2);

                // adiciona ao automato criado
                aux->Estados.push_back(E1);
                aux->Estados.push_back(E2);

                // joga na heap
                heap.push_back(aux);

                break;
            }
        }
    }
    for (vector<Estado *>::iterator it = heap[0]->Estados.begin(); it != heap[0]->Estados.end(); it++)
    {
        this->Estados.push_back(*it);
    }
    for (vector<Estado *>::iterator it = heap[0]->EstadosIniciais.begin(); it != heap[0]->EstadosIniciais.end(); it++)
    {
        this->EstadosIniciais.push_back(*it);
    }
    for (vector<Estado *>::iterator it = heap[0]->EstadosFinais.begin(); it != heap[0]->EstadosFinais.end(); it++)
    {
        this->EstadosFinais.push_back(*it);
    }
}
void Automato::nomeiaEstados()
{
    int n = 1;
    for (vector<Estado *>::iterator it = this->Estados.begin(); it != this->Estados.end(); it++)
    {
        (*it)->id = n;
        n++;
    }
}
void Automato::criaVisualizacao(ofstream &output)
{
    nomeiaEstados();

    output.open("TESTE.dot");

    output << "digraph TESTE {" << endl;

    for (vector<Estado *>::iterator estado = this->Estados.begin(); estado != this->Estados.end(); estado++)
    {
#define V (*estado)->transicoes
        for (vector<Transicao *>::iterator transicao = V.begin(); transicao != V.end(); transicao++)
        {
            output << (*estado)->id << " -> " << (*transicao)->destino->id << "(" << (*transicao)->tipo << ")" << endl;
        }
    }
    output << "}" << endl;
}