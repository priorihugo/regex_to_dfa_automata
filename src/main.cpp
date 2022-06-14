#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

// Ler arquivo.
vector<string> leArquivo(string arquivo)
{
    vector<string> aux;

    // Abre arquivo.
    ifstream input_file;
    input_file.open("../input/" + arquivo, ios::in);

    // Verifica se arquivo foi aberto.
    if (!input_file.is_open())
    {
        cout << "[ERROR] Nao foi possivel abrir arquivo." << endl;
        exit(2);
    }

    // Separa cada linha do arquivo em uma string.
    string line;
    while (getline(input_file, line))
    {
        aux.push_back(line);
    }

    // Verifica se arquivo esta vazio.
    if (aux.empty())
    {
        cout << "[WARNING] Nunhuma string foi carregada do arquivo." << endl;
    }

    // Imprimindo strings para teste.
    // cout << endl;
    // for (vector<string>::iterator it = aux.begin(); it != aux.end(); it++)
    // {
    //     cout << *it << endl;
    // }
    cout << "[INFO] Arquivo carregado." << endl;

    // Fecha arquivo.
    input_file.close();
    return aux;
}

void menu()
{
    cout << endl;
    cout << ":d -> Dividir em tags a string do arquivo informado" << endl;
    cout << ":c -> Carregar um arquivo com definicoes de tags" << endl;
    cout << ":o -> Especificar o caminho do arquivo de saida para a divisao em tags" << endl;
    cout << ":p -> Realizar a divisao em tags da entrada informada" << endl;
    cout << ":a -> Listar as definicoes formais dos automatos em memoria" << endl;
    cout << ":l -> Listar as definicoes de tag validas" << endl;
    cout << ":s -> Salvar as tags" << endl;
    cout << ":q -> Sair do programa" << endl;
}

int main()
{
    string opcao;

    // Posicao 0 do vetor aux armazena funcionalidade.
    // Posicao 1 (quando existe) armazena nome de arquivo ou string.
    vector<string> aux;
    string delimiter = " ";

    // Vetor de Tags quer armazena tags validas.
    //vector<Tag> tagsValidas;

    vector<string> input_string;
    vector<string> input_tags;

    do
    {
        menu();
        getline(cin, opcao);

        // Dividindo a string opcao.
        size_t pos;

        while ((pos = opcao.find(delimiter)) != string::npos)
        {
            aux.push_back(opcao.substr(0, pos));
            opcao.erase(0, pos + delimiter.length());
        }
        aux.push_back(opcao);
        opcao = "";

        // Tratando erros de entrada.
        // Verifica se opcao foi passada por parametro no formato ":x".
        if (aux[0].size() != 2 && aux[0].at(0) != ':')
        {
            cout << "[ERROR] Erro no comando de entrada." << endl;
            exit(1);
        }

        // Verifica se usuario passou 2 parametro de entrada.
        // Algumas opcoes sao excecao por terem apenas 1 parametro.
        if (aux[0] != ":q" && aux[0] != ":l" && aux[0] != ":a")
        {
            if (aux.size() != 2)
            {
                cout << "[ERROR] Dados de entrada incorretos." << endl;
                exit(1);
            }
        }

        switch (aux[0].at(1))
        {
        case 'd':
            input_string = leArquivo(aux[1]);
            cout << "[WARNING] Funcionalidade de dividir string nao implementada." << endl;
            // FAZER ----> Dividir a string em tags.
            break;
        case 'c':
            input_tags = leArquivo(aux[1]);
            //tagsValidas = divideTag(input_tags, tagsValidas);
            break;
        case 'o':
            cout << "[WARNING] Funcionalidade nao implementada." << endl;
            break;
        case 'p':
            cout << "[WARNING] Funcionalidade nao implementada." << endl;
            break;
        case 'a':
            cout << "[WARNING] Funcionalidade nao implementada." << endl;
            break;
        case 'l':
            // Lista todas as definicoes das tags validas.
            //listaTags(tagsValidas);
            break;
        case 's':
            //salvarTags(tagsValidas, aux[1]);
            break;
        case 'q':
            cout << "[INFO] Encerrando Programa." << endl;
            break;
        default:
            cout << "Opcao Invalida" << endl;
            break;
        }
        aux.clear();

    } while (aux[0] != ":q");
}