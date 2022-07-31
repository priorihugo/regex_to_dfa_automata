#include "Automato.h"
#include <iostream>

using namespace std;

Automato::Automato()
{
}

Automato::~Automato()
{
}
void linkEstadoParaVectorEstados(Estado *e, vector<Estado *> &v, char c)
{

    for (vector<Estado *>::iterator it = v.begin(); it != v.end(); it++)
    {
        Transicao *t = new Transicao();
        t->destino = (*it);
        t->tipo = c;
        e->transicoes.push_back(t);
    }
}
void linkVectorEstadosParaEstado(vector<Estado *> &v, Estado *e, char c)
{
    for (vector<Estado *>::iterator it = v.begin(); it != v.end(); it++)
    {
        Transicao *t = new Transicao();
        t->destino = e;
        t->tipo = c;
        (*it)->transicoes.push_back(t);
    }
}
void copiaVectorParaVector(vector<Estado *> &v1 , vector<Estado *> &v2){
    for(vector<Estado * >::iterator it = v1.begin() ; it != v1.end() ; it ++){
        v2.push_back((*it));
    }
}
void fechoLambdaHelper(Estado* e , vector<Estado* > &v){

    for (vector<Transicao* >::iterator it = e->transicoes.begin(); it != e->transicoes.end(); it++)
    {
        if((*it)->tipo == '~'){
            fechoLambdaHelper((*it)->destino , v);
            v.push_back((*it)->destino);
        }
    } 
}
void fechoLambda(Estado* e){
    vector<Estado* > v;

    fechoLambdaHelper(e , v);
    cout << "[teste fecho lambda ~ de "<< e->id << " ]" << endl;
    for(vector<Estado*>::iterator it = v.begin(); it != v.end() ; it++){
        cout << (*it)->id << endl;
    }
}
void Automato::criaAutomato(string exp)
{
    vector<Automato *> heap;

    int counter = 0;

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
                    //inicio->inicio = true;
                    aux->EstadosIniciais.push_back(inicio);

                    Estado *fim = new Estado();
                    //fim->fim = true;
                    aux->EstadosFinais.push_back(fim);

                    // transiçoes que ligam o novo inicio ao inicio dos automatos antigos

                    linkEstadoParaVectorEstados(inicio, A1->EstadosIniciais, '~');
                    A1->EstadosIniciais.clear();
                    linkEstadoParaVectorEstados(inicio, A2->EstadosIniciais, '~');
                    A2->EstadosIniciais.clear();

                    /// ligando os estados finais
                    linkVectorEstadosParaEstado(A1->EstadosFinais, fim, '~');
                    A1->EstadosFinais.clear();
                    linkVectorEstadosParaEstado(A2->EstadosFinais, fim, '~');
                    A2->EstadosFinais.clear();

                    /// por fim copiando os estados dos 2 ultimos automatos para o aux
                    aux->Estados.push_back(inicio);
                    copiaVectorParaVector(A1->Estados , aux->Estados);
                    copiaVectorParaVector(A2->Estados , aux->Estados);
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
                    Automato *aux = new Automato();
                    Automato *A1 = heap.back();
                    heap.pop_back();
                    Automato *A2 = heap.back();
                    heap.pop_back();

                    //linkando todos os estados finais de a2 aos iniciais de a1
                    for (vector<Estado* >::iterator it = A2->EstadosFinais.begin(); it < A2->EstadosFinais.end(); it++)
                    {
                        linkEstadoParaVectorEstados((*it) , A1->EstadosIniciais , '~');
                    }
                    copiaVectorParaVector(A1->Estados , aux->Estados);
                    copiaVectorParaVector(A2->Estados , aux->Estados);
                    copiaVectorParaVector(A2->EstadosIniciais , aux->EstadosIniciais);
                    copiaVectorParaVector(A1->EstadosFinais , aux->EstadosFinais);
                    
                    heap.push_back(aux);

                    delete A1;
                    delete A2;

                }
                break;
            case '*':
                if (heap.size() >= 1)
                {
                    Automato *aux = new Automato();
                    Automato *A1 = heap.back();
                    heap.pop_back();

                    for(vector<Estado *>::iterator it = A1->EstadosFinais.begin(); it != A1->EstadosFinais.end() ; it++){
                        linkEstadoParaVectorEstados((*it) , A1->EstadosIniciais , '~');
                    }
                    
                    A1->EstadosFinais.clear();
                    copiaVectorParaVector(A1->EstadosIniciais , A1->EstadosFinais);

                    copiaVectorParaVector(A1->Estados , aux->Estados);
                    copiaVectorParaVector(A1->EstadosIniciais , aux->EstadosIniciais);
                    copiaVectorParaVector(A1->EstadosFinais , aux->EstadosFinais);

                    heap.push_back(aux);
                    delete A1;
                    
                }
                break;
            default:
                // automato simples com uma transição
                Automato *aux = new Automato();
                Estado *E1 = new Estado();
                Estado *E2 = new Estado();
                // seta inicio e fim
                //E1->inicio = true;
                aux->EstadosIniciais.push_back(E1);
                //E2->fim = true;
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
    /// copia o automato auxiliar para o automato da classe por assim dizer
    copiaVectorParaVector(heap[0]->Estados , this->Estados);
    copiaVectorParaVector(heap[0]->EstadosIniciais , this->EstadosIniciais);
    copiaVectorParaVector(heap[0]->EstadosFinais , this->EstadosFinais);
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
void Automato::criaVisualizacao(string name)
{
    nomeiaEstados();

    ofstream output;

    output.open(name + ".dot");

    output << "digraph "<< name << " {" << endl;

    for (vector<Estado *>::iterator estado = this->EstadosIniciais.begin(); estado != this->EstadosIniciais.end(); estado++)
    {
        output << (*estado)->id << "[ color = blue ]" << endl;
    }

    for (vector<Estado *>::iterator estado = this->EstadosFinais.begin(); estado != this->EstadosFinais.end(); estado++)
    {
        output << (*estado)->id << "[ color = red ]" << endl;
    }

    for (vector<Estado *>::iterator estado = this->Estados.begin(); estado != this->Estados.end(); estado++)
    {
        fechoLambda((*estado));
#define V (*estado)->transicoes
        for (vector<Transicao *>::iterator transicao = V.begin(); transicao != V.end(); transicao++)
        {
            output << (*estado)->id;
            /*
            if((*estado)->inicio){
                output << "(inicio) " ;
            }
            if((*estado)->fim){
                output << "(fim) ";
            }*/
            output << " -> " << (*transicao)->destino->id << "[ label = \" " << (*transicao)->tipo << "\" ]" << endl;
        }
    }
    output << "}" << endl;

    string command = ("dot -Tpng "+ name + ".dot -o " + name + ".png");
    const char* c = command.c_str();

    system(c);
}