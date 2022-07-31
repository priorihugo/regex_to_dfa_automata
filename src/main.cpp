#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

struct Tag
{
    string nome;
    string expressao;
};
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

    cout << "[INFO] Arquivo carregado." << endl;

    // Fecha arquivo.
    input_file.close();
    return aux;
}

void escreveArquivoTags(string arquivo, vector<Tag> tags)
{
    ofstream output_file;
    output_file.open("../output/" + arquivo, ios::out);

    // Verifica se arquivo foi aberto.
    if (!output_file.is_open())
    {
        cout << "[ERROR] Nao foi possivel abrir arquivo." << endl;
        exit(2);
    }

    for (vector<Tag>::iterator it = tags.begin(); it != tags.end(); it++)
    {
        output_file << it->nome << ": " << it->expressao << endl;
    }

    cout << "[INFO] Tags salvas no arquivo." << arquivo << endl;

    // Fecha arquivo.
    output_file.close();
}

vector<Tag> concatenaVetores(vector<Tag> tags, vector<Tag> tagsValidas)
{
    for(int i = 0; i < tags.size(); i++)
    {
        tagsValidas.push_back(tags[i]);
    }
    return tagsValidas;
}
// Salva tags validas em arquivo.
void salvarTags(vector<Tag> tags, string output_file)
{
    if (!tags.empty())
    {
        escreveArquivoTags(output_file, tags);
    }
    else
    {
        cout << "[INFO] Nenhuma definicao de tag foi carregada. Execute o comando :c ou :e para carrega-las. " << endl;
    }
}

bool validaExpressao(string exp)
{
    vector<string> heap;
    for (int i = 0; i < exp.length(); i++)
    {
        string op1 = "", op2 = "", resultado = "";
        /// faixa de simbolos validos
        if (int(exp[i] >= 32 && int(exp[i]) <= 126))
        {
            switch (exp[i])
            {
            case ' ':
                continue;
                break;
            case '+':
                if (heap.size() >= 2)
                {
                    op1 = heap.back();
                    heap.pop_back();
                    op2 = heap.back();
                    heap.pop_back();

                    resultado.push_back('(');
                    resultado.append(op2);
                    resultado.push_back('+');
                    resultado.append(op1);
                    resultado.push_back(')');
                    heap.push_back(resultado);
                }
                else
                {
                    return false;
                }
                break;
            case '.':
                if (heap.size() >= 2)
                {
                    op1 = heap.back();
                    heap.pop_back();
                    op2 = heap.back();
                    heap.pop_back();

                    resultado.push_back('(');
                    resultado.append(op2);
                    resultado.push_back('.');
                    resultado.append(op1);
                    resultado.push_back(')');
                    heap.push_back(resultado);
                }
                else
                {
                    return false;
                }
                break;
            case '/':
            if (heap.size() >= 2)
                {
                    op1 = heap.back();
                    heap.pop_back();
                    op2 = heap.back();
                    heap.pop_back();

                    resultado.push_back('(');
                    resultado.append(op2);
                    resultado.push_back('/');
                    resultado.append(op1);
                    resultado.push_back(')');
                    heap.push_back(resultado);
                }
                else
                {
                    return false;
                }
                break;
            case '*':
                if (heap.size() >= 1)
                {
                    op1 = heap.back();
                    heap.pop_back();

                    resultado.append(op1);
                    resultado.push_back('*');
                    heap.push_back(resultado);
                }
                else
                {
                    return false;
                }
                break;
            default:
                resultado = "";
                resultado.push_back(exp[i]);
                heap.push_back(resultado);
                break;
            }
        }
        else
        {
            resultado = "";
            cout << "[INFO] Caractere " << exp[i] << " invalido." << endl;
            return false;
        }
    }
    if (heap.size() == 1)
    {
        return true;
    }
    else
    {
        cout << "[INFO] Expressao " << exp <<  " invalida." << endl;
        return false;
    }
}
// Verifica se já existe uma tag valida com o nome da tag que esta sendo lida.
bool validaNomeTag(string nomeTag, vector<Tag> tagsValidas)
{
    // Funcao faz copia das variaveis.
    string str_temp = "";
    string str_name = nomeTag;
    // Transforma temporariamente o nome da tag em letras minusculas.
    transform(str_name.begin(), str_name.end(), str_name.begin(), ::tolower);
    for(int i = 0; i < tagsValidas.size(); i++)
    {
        str_temp = tagsValidas[i].nome;
        transform(str_temp.begin(), str_temp.end(), str_temp.begin(), ::tolower);
        
        if(str_name == str_temp)
        {
            cout << "[WARNING] Tag \"" << nomeTag << "\" nao sera salva pois ja existe outra com mesmo nome." << endl;
            return true;
        }
    }
    return false;
}
// Função para dividir nome da tag e expressao.
vector<Tag> divideTag(vector<string> input_tags, vector<Tag> tagsValidas)
{
    for (int i = 0; i < input_tags.size(); i++)
    {
        size_t pos;
        string delimiter = ": ";
        string str_auxNome = "";

        while ((pos = input_tags[i].find(delimiter)) != string::npos)
        {
            // Separando nome da tag e sua expressao.
            str_auxNome = input_tags[i].substr(0, pos);
            input_tags[i].erase(0, pos + delimiter.length());

            // Verifica se nome da tag é valido. (true = o nome ja existe)
            if(!validaNomeTag(str_auxNome, tagsValidas))
            {
                // Valida expressao da tag que ficou na string.
                bool aux = validaExpressao(input_tags[i]);

                // Se expressao for valida inserimos ela no vetor tagsValidas.
                if (aux)
                {
                    Tag tag;
                    // Formata todos os nomes de tag em letras maiusculas.
                    transform(str_auxNome.begin(), str_auxNome.end(), str_auxNome.begin(), ::toupper);
                    tag.nome = str_auxNome;
                    tag.expressao = input_tags[i];
                    tagsValidas.push_back(tag);
                }
            }
        }
    }
    return tagsValidas;
}
//
void listaTags(vector<Tag> tags)
{
    if (!tags.empty())
    {
        cout << endl;
        for (vector<Tag>::iterator it = tags.begin(); it != tags.end(); it++)
        {
            cout << it->nome << ": " << it->expressao << endl;
        }
    }
    else
    {
        cout << "[INFO] Nenhuma definicao de tag foi carregada. Execute o comando :c ou :e para carrega-las. " << endl;
    }
}
// Ler tags via terminal.
vector<Tag> lerTagsTerminal(vector<Tag> tagsValidas)
{
    vector<string> tags;
    string temp;
    int quantTags = 0;

    cout << "Quantas tags serao listadas?" << endl;
    cin >> quantTags;

    if(quantTags < 0) 
        cout << "[WARNING] Valor invalido." << endl;

    cout << endl << "[INFO] Digite as definicoes de Tag abaixo." << endl;

    for(int i = 0; i <= quantTags; i++)
    {
        getline(cin, temp);
        tags.push_back(temp);
    }
    
    tagsValidas = divideTag(tags, tagsValidas);
    cout << "[INFO] Tags carregadas." << endl;
    return tagsValidas;
}

void menu()
{
    cout << endl;
    cout << ":d -> Dividir em tags a string do arquivo informado" << endl;
    cout << ":c -> Carregar um arquivo com definicoes de tags" << endl;
    cout << ":e -> Definir tags" << endl;
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
    vector<Tag> tagsValidas;

    vector<string> input_string;
    vector<string> input_tags;

    // Vetor temporario para receber retorno de funcao 
    // quando tag é lida via terminal.
    vector<Tag> temp;

    menu();

    do
    {
        cout << endl;
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
            cout << "[ERROR] Erro no formato do comando de entrada." << endl;
            exit(1);
        }

        // Verifica se usuario passou 2 parametro de entrada.
        // Algumas opcoes sao excecao por terem apenas 1 parametro.
        if (aux[0] != ":q" && aux[0] != ":l" && aux[0] != ":a" && aux[0] != ":e")
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
            // Funcao de dividir chama a de validar a tag.
            temp.clear();
            temp = divideTag(input_tags, tagsValidas);
            tagsValidas = concatenaVetores(temp, tagsValidas);
            break;
        case 'e':
            temp.clear();
            temp = lerTagsTerminal(tagsValidas);
            tagsValidas = concatenaVetores(temp, tagsValidas);
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
            listaTags(tagsValidas);
            break;
        case 's':
            salvarTags(tagsValidas, aux[1]);
            break;
        case 'q':
            cout << "[INFO] Encerrando Programa." << endl;
            break;
        default:
            cout << "[WARNING] Opcao Invalida." << endl;
            break;
        }
        aux.clear();

    } while (aux[0] != ":q");
}