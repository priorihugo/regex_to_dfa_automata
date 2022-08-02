#include "Automato.h"

Automato::Automato()
{
}
Automato::~Automato()
{
}
void linkEstadoParaSetEstados(Estado *e, set<Estado *> &v, char c)
{
    for (auto it = v.begin(); it != v.end(); it++)
    {
        Transicao *t = new Transicao();
        t->destino = (*it);
        t->tipo = c;
        e->transicoes.insert(t);
    }
}
void linkSetEstadosParaEstado(set<Estado *> &v, Estado *e, char c)
{
    for (set<Estado *>::iterator it = v.begin(); it != v.end(); it++)
    {
        Transicao *t = new Transicao();
        t->destino = e;
        t->tipo = c;
        (*it)->transicoes.insert(t);
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
    for (auto it = e->transicoes.begin(); it != e->transicoes.end(); it++)
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
void Automato::criaAutomato(string exp, string name_tag)
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
                    aux->EstadosIniciais.insert(inicio);
                    Estado *fim = new Estado();
                    aux->EstadosFinais.insert(fim);

                    // transiçoes que ligam o novo inicio ao inicio dos automatos antigos
                    linkEstadoParaSetEstados(inicio, A1->EstadosIniciais, '~');
                    A1->EstadosIniciais.clear();
                    linkEstadoParaSetEstados(inicio, A2->EstadosIniciais, '~');
                    A2->EstadosIniciais.clear();

                    /// ligando os estados finais
                    linkSetEstadosParaEstado(A1->EstadosFinais, fim, '~');
                    A1->EstadosFinais.clear();
                    linkSetEstadosParaEstado(A2->EstadosFinais, fim, '~');
                    A2->EstadosFinais.clear();

                    /// por fim copiando os estados dos 2 ultimos automatos para o aux
                    aux->Estados.insert(inicio);
                    copiaSetParaSet(A1->Estados, aux->Estados);
                    copiaSetParaSet(A2->Estados, aux->Estados);
                    aux->Estados.insert(fim);

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
                    for (auto it = A2->EstadosFinais.begin(); it != A2->EstadosFinais.end(); it++)
                    {
                        linkEstadoParaSetEstados((*it), A1->EstadosIniciais, '~');
                    }
                    copiaSetParaSet(A1->Estados, aux->Estados);
                    copiaSetParaSet(A2->Estados, aux->Estados);
                    copiaSetParaSet(A2->EstadosIniciais, aux->EstadosIniciais);
                    copiaSetParaSet(A1->EstadosFinais, aux->EstadosFinais);

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

                    Estado *inicio = new Estado();
                    Estado *fim = new Estado();

                    aux->Estados.insert(inicio);
                    aux->EstadosIniciais.insert(inicio);
                    aux->EstadosFinais.insert(fim);

                    Transicao *t = new Transicao();
                    t->tipo = '~';
                    t->destino = fim;
                    inicio->transicoes.insert(t);

                    for (auto it = A1->EstadosFinais.begin(); it != A1->EstadosFinais.end(); it++)
                    {
                        linkEstadoParaSetEstados((*it), A1->EstadosIniciais, '~');
                    }
                    linkEstadoParaSetEstados(inicio, A1->EstadosIniciais, '~');
                    linkSetEstadosParaEstado(A1->EstadosFinais, fim, '~');
                    copiaSetParaSet(A1->Estados, aux->Estados);
                    copiaSetParaSet(A1->Alfabeto, aux->Alfabeto);

                    aux->Estados.insert(fim);

                    heap.push_back(aux);
                    A1->EstadosIniciais.clear();
                    A1->EstadosFinais.clear();
                    A1->Estados.clear();
                    A1->Alfabeto.clear();

                    delete A1;
                }
                break;
            default:
                // automato simples com uma transição
                Automato *aux = new Automato();
                Estado *E1 = new Estado();
                Estado *E2 = new Estado();
                // seta inicio e fim
                aux->EstadosIniciais.insert(E1);
                aux->EstadosFinais.insert(E2);
                // cria a transicao e o tipo dela
                Transicao *E1E2 = new Transicao();
                E1E2->destino = E2;
                E1E2->tipo = exp[i];
                aux->Alfabeto.insert(exp[i]);

                E1->transicoes.insert(E1E2);
                // adiciona ao automato criado
                aux->Estados.insert(E1);
                aux->Estados.insert(E2);
                // joga na heap
                heap.push_back(aux);
                break;
            }
        }
    }
    // alterando estados finais para o fecho lambda dos estados finais
    // ajuda daqui pra frente

    for (auto it = this->EstadosFinais.begin(); it != this->EstadosFinais.end(); it++)
    {
        set<Estado *> fecho;
        fechoLambda((*it), fecho);
        copiaSetParaSet(fecho, this->EstadosFinais);
        fecho.clear();
    }
    /// copia o automato auxiliar para o automato da classe por assim dizer
    copiaSetParaSet(heap[0]->Estados, this->Estados);
    copiaSetParaSet(heap[0]->EstadosIniciais, this->EstadosIniciais);
    copiaSetParaSet(heap[0]->EstadosFinais, this->EstadosFinais);
    copiaSetParaSet(heap[0]->Alfabeto, this->Alfabeto);

    this->reduzParaAFD(name_tag);
}
void imprimeset(set<Estado *> &s)
{

    cout << "{";
    for (auto it = s.begin(); it != s.end(); it++)
    {
        cout << " " << (*it)->id << " ";
    }
    cout << "}" << endl;
}
void Automato::imprimeSetFormal(set<Estado *> s)
{
    cout << "{";
    for (auto it = s.begin(); it != s.end(); it++)
    {
        cout << (*it)->id << " ";
    }
    cout << "}";
}
typedef struct ConjuntoEstados
{
    set<Estado *> conjunto;
    Estado *estadoCorrespondente;
} CE;

void Automato::reduzParaAFD(string name_tag)
{
    this->nomeiaEstados();

    // aux representa o novo automato
    Automato *aux = new Automato();
    // o conjunto de estados vai representar um estado equivalente no novo automato
    ConjuntoEstados *inicio = new ConjuntoEstados();
    // esse set representa o conjunto dos novos estados do novo automato
    list<ConjuntoEstados *> novosEstados;
    Estado *estadoInicial = new Estado();
    inicio->estadoCorrespondente = estadoInicial;
    // fecho lambda dos estados iniciais
    for (auto estado = this->EstadosIniciais.begin(); estado != this->EstadosIniciais.end(); estado++)
    {
        // cout << "Criando conjunto de estados iniciais" << endl;
        set<Estado *> fecho;
        fechoLambda((*estado), fecho);
        copiaSetParaSet(fecho, inicio->conjunto);
        // cout << "Imprimento conjunto inicial" << endl;
        // cout << inicio << endl;
        // imprimeset(inicio->conjunto);
    }
    //verificando se tambem é final;
    for (auto it = this->EstadosFinais.begin(); it != this->EstadosFinais.end(); it++)
    {
        for (auto it2 = inicio->conjunto.begin(); it2 != inicio->conjunto.end(); it2++)
        {
            // cout << (*it)->id << " e estado final! " << endl;
            if ((*it) == (*it2))
            {
                aux->EstadosFinais.insert(inicio->estadoCorrespondente);
                break;
            }
        }
    }
    aux->Estados.insert(estadoInicial);
    aux->EstadosIniciais.insert(estadoInicial);

    novosEstados.push_back(inicio);

    // iterator do set de novos estados

    // itera pelo conjunto de estados que vao sendo criados a medida que passamos pela tabela de transicao correspondente
    for (auto novosEstadosIt = novosEstados.begin(); novosEstadosIt != novosEstados.end(); novosEstadosIt++)
    {
        // cout << "Saindo do conjunto de estados ";
        // cout << (*novosEstadosIt) << endl;
        // imprimeset((*novosEstadosIt)->conjunto);
        // itera sobre o alfabeto do automato
        for (auto simbolo = this->Alfabeto.begin(); simbolo != this->Alfabeto.end(); simbolo++)
        {
            // estados alcançados pelo simbolo
            char simboloAtual = (*simbolo);
            // cout << "Com o simbolo " << simboloAtual << endl;
            // criando um novo conjunto de estados para representar os estados alcançados por casa simbolo e um novo estado que é correspondente a estes;
            ConjuntoEstados *estadosAlcancados = new ConjuntoEstados();
            Estado *novoEstado = new Estado();
            estadosAlcancados->estadoCorrespondente = novoEstado;
            //
            // iterando pelos estados de um conjunto presente nos novos estados
            for (auto estado = (*novosEstadosIt)->conjunto.begin(); estado != (*novosEstadosIt)->conjunto.end(); estado++)
            {
                // passa pelas transicoes de cada estado
                for (auto transicao = (*estado)->transicoes.begin(); transicao != (*estado)->transicoes.end(); transicao++)
                {
                    if ((*transicao)->tipo == simboloAtual)
                    {
                        Estado *estadoDestino = (*transicao)->destino;
                        estadosAlcancados->conjunto.insert(estadoDestino);
                    }
                }
            }
            ConjuntoEstados *estadosAlcancadosFecho = new ConjuntoEstados();
            Estado *ec = new Estado();
            estadosAlcancadosFecho->estadoCorrespondente = ec;

            for (auto it = estadosAlcancados->conjunto.begin(); it != estadosAlcancados->conjunto.end(); it++)
            {
                set<Estado *> fecho;
                fechoLambda((*it), fecho);
                copiaSetParaSet(fecho, estadosAlcancadosFecho->conjunto);
                fecho.clear();
            }
            // cout << "Estados alcancados: ";
            // cout << (estadosAlcancadosFecho) << endl;
            // imprimeset(estadosAlcancadosFecho->conjunto);

            // verifica se existe um set na lista totalmente igual ao set encontrado
            bool insert = true;
            for (auto estadoC = novosEstados.begin(); estadoC != novosEstados.end(); estadoC++)
            {
                if ((*estadoC)->conjunto == estadosAlcancadosFecho->conjunto)
                {
                    // cout << "Conjunto ja existe no set" << endl;
                    insert = false;
                    estadosAlcancadosFecho = (*estadoC);
                }
            }
            // iteraçãozinha pra descobrir estados finais e iniciais
            for (auto it = this->EstadosIniciais.begin(); it != this->EstadosIniciais.end(); it++)
            {
                for (auto it2 = estadosAlcancadosFecho->conjunto.begin(); it2 != estadosAlcancadosFecho->conjunto.end(); it2++)
                {
                    if ((*it) == (*it2))
                    {
                        // cout << (*it)->id << " e estado inicial! " << endl;
                        aux->EstadosIniciais.insert(estadosAlcancadosFecho->estadoCorrespondente);
                        break;
                    }
                }
            }
            for (auto it = this->EstadosFinais.begin(); it != this->EstadosFinais.end(); it++)
            {
                for (auto it2 = estadosAlcancadosFecho->conjunto.begin(); it2 != estadosAlcancadosFecho->conjunto.end(); it2++)
                {
                    // cout << (*it)->id << " e estado final! " << endl;
                    if ((*it) == (*it2))
                    {
                        aux->EstadosFinais.insert(estadosAlcancadosFecho->estadoCorrespondente);
                        break;
                    }
                }
            }

            // se o estado n leva a algum conjunto
            // ficaria dificil manter esse estado
            if (!estadosAlcancadosFecho->conjunto.empty()) // caso não encontre cojunto igual
            {
                if (insert)
                {
                    // cout << "Inserindo novo estado correspondente ao conjunto ";
                    // imprimeset(estadosAlcancadosFecho->conjunto);
                    // cout << endl;

                    novosEstados.push_back(estadosAlcancadosFecho);
                    aux->Estados.insert(estadosAlcancadosFecho->estadoCorrespondente);
                }
                Transicao *nova = new Transicao();
                nova->tipo = simboloAtual;
                nova->destino = estadosAlcancadosFecho->estadoCorrespondente;
                (*novosEstadosIt)->estadoCorrespondente->transicoes.insert(nova);
                // cout << "criando uma nova transicao do tipo " << simboloAtual << " de ";
                // imprimeset((*novosEstadosIt)->conjunto);
                // cout << " para ";
                // imprimeset((estadosAlcancadosFecho->conjunto));
                // cout << endl;
            }
        }
    }
    this->Alfabeto.clear();
    this->Estados.clear();
    this->EstadosIniciais.clear();
    this->EstadosFinais.clear();

    copiaSetParaSet(aux->Alfabeto , this->Alfabeto);
    copiaSetParaSet(aux->Estados , this->Estados);
    copiaSetParaSet(aux->EstadosIniciais , this->EstadosIniciais);
    copiaSetParaSet(aux->EstadosFinais , this->EstadosFinais);
    
    aux->criaVisualizacao(name_tag);
}
bool Automato::processaString(string data, int* posicao){
    //passar a string para todos os automatos em menoria. (1 exp => 1 automato)
    auto it = this->EstadosIniciais.begin();
    Estado* estadoAtual = (*it);
    int charAtual = 0;
    bool continua = true;
    bool aux = false;
    while (charAtual < data.size() && estadoAtual != nullptr)
    {
        //itera pelo estado atual
        Estado* proximoEstado = nullptr;
        for (auto it = estadoAtual->transicoes.begin(); it != estadoAtual->transicoes.end() ; it++)
        {
            if((*it)->tipo == data[charAtual]){
            //cout << "Saindo do estado " << estadoAtual->id << " para " << (*it)->destino->id << " com " << data[charAtual] << endl;
                proximoEstado = (*it)->destino;
                charAtual++;
                aux = true;
                //marca em que posicao da string o automato validou
                *posicao += 1;
                if(find(this->EstadosFinais.begin(), this->EstadosFinais.end(), estadoAtual) != this->EstadosFinais.end()){
                    return true;
                }
                break;
            }
        }
        estadoAtual = proximoEstado;
        if(!aux)
        {
            //cout << "Automato nao aceita a string." << endl;
            return false;
        }
    }
    return true;    
}
void Automato::nomeiaEstados()
{
    int n = 1;
    for (auto it = this->Estados.begin(); it != this->Estados.end(); it++)
    {
        (*it)->id = n;
        n++;
    }
}
void Automato::criaVisualizacao(string name)
{
    nomeiaEstados();

    ofstream output;

    output.open("../output/" + name + ".dot");

    output << "digraph " << name << " {" << endl;

    for (auto estado = this->EstadosIniciais.begin(); estado != this->EstadosIniciais.end(); estado++)
    {
        output << (*estado)->id << "[ color = blue ]" << endl;
    }

    for (auto estado = this->EstadosFinais.begin(); estado != this->EstadosFinais.end(); estado++)
    {
        output << (*estado)->id << "[ shape=doublecircle ]" << endl;
    }

    for (auto estado = this->Estados.begin(); estado != this->Estados.end(); estado++)
    {
#define V (*estado)->transicoes
        for (auto transicao = V.begin(); transicao != V.end(); transicao++)
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

    string command = ("dot -Tpng ../output/" + name + ".dot -o " + "../output/" + name + ".png");
    const char *c = command.c_str();
    system(c);
}