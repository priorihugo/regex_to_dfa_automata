#include "Automato.h"

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
void copiaVectorParaVector(vector<Estado *> &v1, vector<Estado *> &v2)
{
    for (vector<Estado *>::iterator it = v1.begin(); it != v1.end(); it++)
    {
        v2.push_back((*it));
    }
}
template <typename T>
void copiaSetParaSet(set<T> &s1, set<T> &s2)
{
    for (auto it = s1.begin(); it != s1.end(); it++)
    {
        s2.insert((*it));
    }
}
void fechoLambdaHelper(Estado *e, set<Estado *> &v)
{

    for (vector<Transicao *>::iterator it = e->transicoes.begin(); it != e->transicoes.end(); it++)
    {
        if ((*it)->tipo == '~')
        {
            v.insert((*it)->destino);
            fechoLambdaHelper((*it)->destino, v);
        }
    }
}
void fechoLambda(Estado *e, set<Estado *> &v)
{

    v.insert(e);
    fechoLambdaHelper(e, v);
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
                    // inicio->inicio = true;
                    aux->EstadosIniciais.push_back(inicio);

                    Estado *fim = new Estado();
                    // fim->fim = true;
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
                    copiaVectorParaVector(A1->Estados, aux->Estados);
                    copiaVectorParaVector(A2->Estados, aux->Estados);
                    aux->Estados.push_back(fim);

                    // junta os alfabetos
                    copiaSetParaSet(A1->Alfabeto, aux->Alfabeto);
                    copiaSetParaSet(A2->Alfabeto, aux->Alfabeto);

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

                    // linkando todos os estados finais de a2 aos iniciais de a1
                    for (vector<Estado *>::iterator it = A2->EstadosFinais.begin(); it < A2->EstadosFinais.end(); it++)
                    {
                        linkEstadoParaVectorEstados((*it), A1->EstadosIniciais, '~');
                    }
                    copiaVectorParaVector(A1->Estados, aux->Estados);
                    copiaVectorParaVector(A2->Estados, aux->Estados);
                    copiaVectorParaVector(A2->EstadosIniciais, aux->EstadosIniciais);
                    copiaVectorParaVector(A1->EstadosFinais, aux->EstadosFinais);

                    copiaSetParaSet(A1->Alfabeto, aux->Alfabeto);
                    copiaSetParaSet(A2->Alfabeto, aux->Alfabeto);

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

                    for (vector<Estado *>::iterator it = A1->EstadosFinais.begin(); it != A1->EstadosFinais.end(); it++)
                    {
                        linkEstadoParaVectorEstados((*it), A1->EstadosIniciais, '~');
                    }

                    A1->EstadosFinais.clear();
                    copiaVectorParaVector(A1->EstadosIniciais, A1->EstadosFinais);

                    copiaVectorParaVector(A1->Estados, aux->Estados);
                    copiaVectorParaVector(A1->EstadosIniciais, aux->EstadosIniciais);
                    copiaVectorParaVector(A1->EstadosFinais, aux->EstadosFinais);

                    copiaSetParaSet(A1->Alfabeto, aux->Alfabeto);

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
                // E1->inicio = true;
                aux->EstadosIniciais.push_back(E1);
                // E2->fim = true;
                aux->EstadosFinais.push_back(E2);
                // cria a transicao e o tipo dela
                Transicao *E1E2 = new Transicao();
                E1E2->destino = E2;
                E1E2->tipo = exp[i];
                aux->Alfabeto.insert(exp[i]);

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
    copiaVectorParaVector(heap[0]->Estados, this->Estados);
    copiaVectorParaVector(heap[0]->EstadosIniciais, this->EstadosIniciais);
    copiaVectorParaVector(heap[0]->EstadosFinais, this->EstadosFinais);

    copiaSetParaSet(heap[0]->Alfabeto, this->Alfabeto);


    this->reduzParaAFD();
}
void imprimeset(set<Estado *> &s){

    cout << "{" ;
    for (auto it = s.begin(); it != s.end() ; it++)
    {
        cout << " " << (*it)->id << " ";
    }
    cout << "}" << endl;
}
typedef struct ConjuntoEstados
{
    set<Estado *> conjunto;
    Estado *estadoCorrespondente;
} CE;

void Automato::reduzParaAFD()
{
    this->nomeiaEstados();

    //aux representa o novo automato
    Automato *aux = new Automato();
    //o conjunto de estados vai representar um estado equivalente no novo automato
    ConjuntoEstados *inicio = new ConjuntoEstados();

    //esse set representa o conjunto dos novos estados do novo automato
    set<ConjuntoEstados *> novosEstados;
    Estado *estadoInicial = new Estado();
    // fecho lambda dos estados iniciais
    for (auto estado = this->EstadosIniciais.begin(); estado != this->EstadosIniciais.end(); estado++)
    {
        cout << "Criando conjunto de estados iniciais" << endl;
        set<Estado *> fecho;
        fechoLambda((*estado), fecho);
        copiaSetParaSet(fecho, inicio->conjunto);
        cout << "Imprimento conjunto inicial" << endl;
        cout << inicio << endl;
        imprimeset(inicio->conjunto);
    }
    inicio->estadoCorrespondente = estadoInicial;

    aux->Estados.push_back(estadoInicial);
    aux->EstadosIniciais.push_back(estadoInicial);

    novosEstados.insert(inicio);

    // iterator do set de novos estados
    auto novosEstadosIt = novosEstados.begin();
    // itera pelo conjunto de estados que vao sendo criados a medida que passamos pela tabela de transicao correspondente
    while (novosEstadosIt != novosEstados.end())
    {
        // itera sobre o alfabeto do automato

        cout <<"Saindo do conjunto de estados " ;
        cout << (*novosEstadosIt) << endl;
        imprimeset((*novosEstadosIt)->conjunto);
        cout << endl;

        for (auto simbolo = this->Alfabeto.begin(); simbolo != this->Alfabeto.end(); simbolo++)
        {
            // estados alcançados pelo simbolo
            char simboloAtual = (*simbolo);
            cout << "Com o simbolo " << simboloAtual << endl;

            //criando um novo conjunto de estados para representar os estados alcançados por casa simbolo e um novo estado que é correspondente a estes;
            ConjuntoEstados *estadosAlcancados = new ConjuntoEstados();
            Estado *novoEstado = new Estado();
            estadosAlcancados->estadoCorrespondente = novoEstado;
            //
            //iterando pelos estados de um conjunto presente nos novos estados
            for (auto estado = (*novosEstadosIt)->conjunto.begin(); estado != (*novosEstadosIt)->conjunto.end(); estado++)
            {
                // passa pelas transicoes de cada estado
                for (auto transicao = (*estado)->transicoes.begin(); transicao != (*estado)->transicoes.end(); transicao++)
                {
                    Estado *estadoDestino = (*transicao)->destino;
                    // se existe transicao com o simbolo atual e chega a algum estado
                    if ((*transicao)->tipo == simboloAtual)
                    {
                        set<Estado *> fecho;
                        // calcula o fecho lambda de todo estado atingivel
                        fechoLambda(estadoDestino, fecho);
                        // joga o fecho lambda do estado atingivel nos estados alcançaveis
                        copiaSetParaSet(fecho, estadosAlcancados->conjunto);
                    }
                }
            }
            cout << "Estados alcancados: " ;
            cout << (estadosAlcancados) << endl;
            imprimeset(estadosAlcancados->conjunto);
            cout << endl;

            // iterando pelos novos estados e verificando se o cojunto ja existe
            bool insert = true;
            for (auto conjunto = novosEstados.begin(); conjunto != novosEstados.end(); conjunto++)
            {
                if ((*conjunto)->conjunto == estadosAlcancados->conjunto)
                {
                    insert = false;
                    //criando uma transição do estado para ele mesmo com o simbolo
                    Transicao *nova = new Transicao();
                    nova->tipo = simboloAtual;
                    nova->destino = (*conjunto)->estadoCorrespondente;
                    (*conjunto)->estadoCorrespondente->transicoes.push_back(nova);
                    aux->Estados.push_back((*conjunto)->estadoCorrespondente);
                }
            }
            // caso não encontre cojunto igual
            if (insert)
            {
                novosEstados.insert(estadosAlcancados);
                Transicao *nova = new Transicao();
                nova->tipo = simboloAtual;
                nova->destino = estadosAlcancados->estadoCorrespondente;
                (*novosEstadosIt)->estadoCorrespondente->transicoes.push_back(nova);
                aux->Estados.push_back(estadosAlcancados->estadoCorrespondente);
            }
        }
        novosEstadosIt++;
    }
    aux->criaVisualizacao("minimizado");
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

    output << "digraph " << name << " {" << endl;

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

    string command = ("dot -Tpng " + name + ".dot -o " + name + ".png");
    const char *c = command.c_str();
    system(c);
}