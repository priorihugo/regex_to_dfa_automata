#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

#include "Automato.h"

using namespace std;

struct Tag
{
    string nome;
    string expressao;
    Automato automato;
};

struct Str_dividida
{
    vector<string> tags;
    string str;
};
// Ler arquivo.
vector<string> leArquivo(string arquivo)
{
    vector<string> aux;

    // Abre arquivo.
    ifstream input_file;
    input_file.open("./input/" + arquivo, ios::in);

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
    output_file.open("./output/" + arquivo, ios::out);

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

    cout << "[INFO] Tags salvas no arquivo " << arquivo << endl;

    // Fecha arquivo.
    output_file.close();
}

void escreveArquivoStrings(string arquivo, vector<Str_dividida> str)
{
    if (str.empty())
    {
        cout << "[INFO] Nenhuma string foi processada." << endl;
        return;
    }
    ofstream output_file;
    output_file.open("./output/" + arquivo, ios::out);

    // Verifica se arquivo foi aberto.
    if (!output_file.is_open())
    {
        cout << "[ERROR] Nao foi possivel abrir arquivo." << endl;
        exit(2);
    }

    vector<string>::iterator it1;
    vector<Str_dividida>::iterator it2;

    for (it2 = str.begin(); it2 != str.end(); it2++)
    {
        output_file << it2->str << endl;
        if (!it2->tags.empty())
        {
            for (it1 = it2->tags.begin(); it1 != it2->tags.end(); it1++)
            {
                output_file << *it1 << " ";
            }
            output_file << endl;
        }
        else
            output_file << "Nenhum automato em memoria processa a string " << endl;
        output_file << endl;
    }

    cout << "[INFO] Strings salvas no arquivo " << arquivo << endl;

    // Fecha arquivo.
    output_file.close();
}

vector<Tag> concatenaVetores(vector<Tag> tags, vector<Tag> tagsValidas)
{
    bool aux = false;
    for (int i = 0; i < tags.size(); i++)
    {
        // for (int j = 0; j < tagsValidas.size(); j++)
        // {
        //     if (tags[i].nome == tagsValidas[j].nome)
        //     {
        //         aux = true;
        //         break;
        //     }
        // }
        // if (!aux)
        tagsValidas.push_back(tags[i]);
    }
    return tagsValidas;
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
            case '\\':
                if (i < exp.length() - 1)
                {
                    // avança na sequencia de char
                    i++;
                    char c;
                    resultado = "";
                    switch (exp[i])
                    {
                    case 'n':
                        c = '\n';
                        resultado.push_back(c);
                        break;
                    case '\\':
                        c = '\\';
                        resultado.push_back(c);
                        break;
                        ;
                    case '*':
                        c = '\*';
                        resultado.push_back(c);
                        break;
                    case '.':
                        c = '\.';
                        resultado.push_back(c);
                        break;
                    case '+':
                        c = '\+';
                        resultado.push_back(c);
                        break;
                    case 'l':
                        c = '\l';
                        resultado.push_back(c);
                        break;
                    default:
                        return false;
                    }
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
            cout << "[WARNING] Caractere " << exp[i] << " invalido." << endl;
            return false;
        }
    }
    if (heap.size() == 1)
    {
        return true;
    }
    else
    {
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
    for (int i = 0; i < tagsValidas.size(); i++)
    {
        str_temp = tagsValidas[i].nome;
        transform(str_temp.begin(), str_temp.end(), str_temp.begin(), ::tolower);

        if (str_name == str_temp)
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
            if (!validaNomeTag(str_auxNome, tagsValidas))
            {
                // Valida expressao da tag.
                bool aux = validaExpressao(input_tags[i]);

                // Se expressao for valida inserimos ela no vetor tagsValidas.
                if (aux)
                {
                    Tag tag;
                    // Formata todos os nomes de tag em letras maiusculas.
                    transform(str_auxNome.begin(), str_auxNome.end(), str_auxNome.begin(), ::toupper);
                    tag.nome = str_auxNome;
                    tag.expressao = input_tags[i];
                    // Cria automato para expressao.
                    tag.automato.criaAutomato(tag.expressao, tag.nome);
                    // Parametro e nome do arquivo .dot de saida.
                    tag.automato.criaVisualizacao(tag.nome);
                    tagsValidas.push_back(tag);
                }
                else
                {
                    cout << "[WARNING] Expressao \'" << input_tags[i] << "\' da tag " << str_auxNome << " invalida." << endl;
                }
            }
        }
    }
    return tagsValidas;
}

void imprimeExpAceitas(vector<string> exp_aceitas)
{
    vector<string>::iterator it;
    for (it = exp_aceitas.begin(); it != exp_aceitas.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
}

vector<Str_dividida> processaString(vector<Tag> tagsValidas, vector<string> input_string, vector<Str_dividida> strs_div)
{
    if (tagsValidas.empty())
    {
        cout << "[INFO] Nenhuma definicao de tag foi carregada. Execute o comando :c ou :e para carrega-las. " << endl;
        return strs_div;
    }
    if (input_string.empty())
    {
        cout << "[INFO] Nunhuma string foi carrega do arquivo." << endl;
        return strs_div;
    }

    // Retorna ultima posicao da string validada pelo automato.
    int posicao_str = 0;
    bool str_isValida;
    // Vetor armazena todos os automatos que validou a string.
    vector<string> tags_aceitas;
    // Armazena tamanho da substring processada por um automato em cada posicao.
    vector<int> tamSubstring;

    // Percorre o vetor com todas as strings a serem processadas.
    cout << endl;
    for (int i = 0; i < input_string.size(); i++)
    {
        Str_dividida str;
        str.str = input_string[i];
        cout << "String: " << str.str << endl;
        // Percorre a string.
        while (input_string[i].size() > 0)
        {
            for (int j = 0; j < tagsValidas.size(); j++)
            {
                posicao_str = tagsValidas[j].automato.processaString(input_string[i]);
                // Armazena o tamanho da sustring processada.
                tamSubstring.push_back(posicao_str);
            }
            // Verificar se todos os elementos do vetor sao iguais a -1.
            str_isValida = false;
            for (int j = 0; j < tamSubstring.size(); j++)
            {
                if (tamSubstring[j] != -1)
                {
                    str_isValida = true;
                    break;
                }
            }
            if (!str_isValida)
            {
                cout << "[WARNING] Nenhum automato em memoria processa a string " << str.str << endl;
                tags_aceitas.clear();
                break;
            }
            else
            {
                //  Verificar qual foi a maior substring processada.
                int maior = 0;
                for (int j = 1; j < tamSubstring.size(); j++)
                {
                    if (tamSubstring[j] > tamSubstring[maior])
                        maior = j;
                }
                // Variavel recebe tamanho da maior substring processada.
                posicao_str = tamSubstring[maior];
                tamSubstring.clear();
                // Para remover parte da string ja processada(quando automato processa apenas um caracter).
                if (posicao_str == 0)
                    posicao_str++;
                // Salva apenas a tag que processou uma maior substring.
                tags_aceitas.push_back(tagsValidas[maior].nome);
                // Remove da string parte ja validada pelo automato.
                input_string[i].erase(0, posicao_str);
                posicao_str = 0;
            }
        }
        // Salva a string e suas tags apos o processamento.
        str.tags = tags_aceitas;
        strs_div.push_back(str);

        // Imprime apenas se string foi processada completamente.
        if (!tags_aceitas.empty())
        {
            imprimeExpAceitas(tags_aceitas);
        }
        tags_aceitas.clear();
    }
    return strs_div;
}

vector<char> encontrarAlfabeto(string exp)
{
    vector<char> alfabeto;

    for (int i = 0; i < exp.size(); i++)
    {
        if (exp[i] != '+' && exp[i] != '.' && exp[i] != '*')
            alfabeto.push_back(exp[i]);
    }
    return alfabeto;
}

void listarAutomatos(vector<Tag> tags)
{
    if (tags.empty())
    {
        cout << "[INFO] Nenhum automato em memoria." << endl;
        return;
    }

    cout << "[INFO] Funcao transicao esta sendo representada pelo arquivo de saida, no diretorio output, da seguinte forma : \"nome da tag.png ou .dot\""
         << endl;

    for (int i = 0; i < tags.size(); i++)
    {
        cout << tags[i].nome << "=(";

        // Imprime estados do automato.
        tags[i].automato.imprimeSetFormal(tags[i].automato.getEstados());
        cout << ", ";
        // Imprime alfabeto.
        vector<char> alfabeto = encontrarAlfabeto(tags[i].expressao);
        cout << "{";
        for (int j = 0; j < alfabeto.size(); j++)
        {
            cout << alfabeto[j] << " ";
        }
        cout << "}, ";
        // Imprime estado inicial.
        tags[i].automato.imprimeSetFormal(tags[i].automato.getEstadosIniciais());
        cout << ", ";
        // Imprime estados finais.
        tags[i].automato.imprimeSetFormal(tags[i].automato.getEstadosFinais());
        cout << ")" << endl;
    }
}
//
void listaTags(vector<Tag> tags)
{
    if (!tags.empty())
    {
        cout << endl;
        for (vector<Tag>::iterator it = tags.begin(); it != tags.end(); it++)
            cout << it->nome << ": " << it->expressao << endl;
    }
    else
        cout << "[INFO] Nenhuma definicao de tag foi carregada. Execute o comando :c ou :e para carrega-las. " << endl;
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

vector<string> trataDadosEntrada(vector<string> aux, string input, int *funcao)
{
    // Dividindo a string opcao.
    string delimiter = " ";
    size_t pos;

    while ((pos = input.find(delimiter)) != string::npos)
    {
        aux.push_back(input.substr(0, pos));
        input.erase(0, pos + delimiter.length());
    }
    aux.push_back(input);

    // Retornar 1 se for tag e 0 funcionalidade.
    if (aux[0].size() == 2 && aux[0].at(0) == ':')
        *funcao = 0;
    else if (aux[0].back() == ':')
        *funcao = 1;
    else
    {
        cout << "[ERROR] Comando invalido." << endl;
        exit(1);
    }
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

    // CASO1: Posicao 0 do vetor aux armazena funcionalidade.
    // Posicao 1 (quando existe) armazena nome de arquivo ou string.
    // CASO2: Posicao 0 do vetor aux armazena nome da tag.
    // Posicao 1 armazana expressao.
    vector<string> aux;
    int tipo_func = 0;

    // Vetor de Tags quer armazena tags validas.
    vector<Tag> tagsValidas;

    vector<string> input_string;
    vector<string> input_tags;

    // Vetor armazena todas as string divididas.
    vector<Str_dividida> strs_dividida;

    // Vetor temporario para receber retorno de funcao
    // quando tag é lida via terminal.
    vector<Tag> temp;

    menu();

    do
    {
        cout << endl;
        getline(cin, opcao);

        // Se for 1 e tag, se for 0 funcionalidade.
        aux = trataDadosEntrada(aux, opcao, &tipo_func);

        if (tipo_func == 1)
        {
            input_tags.push_back(opcao);
            tagsValidas = divideTag(input_tags, tagsValidas);
            input_tags.clear();
        }
        else
        {
            // Verifica se usuario passou 2 parametro de entrada.
            // Algumas opcoes sao excecao por terem apenas 1 parametro.
            if (aux[0] == ":d" || aux[0] == ":c" || aux[0] == ":o" || aux[0] == ":p" || aux[0] == ":s")
            {
                if (aux.size() != 2)
                {
                    cout << "[ERROR] Dados de entrada incorretos, espera-se 2 argumentos." << endl;
                    exit(1);
                }
            }

            switch (aux[0].at(1))
            {
            case 'd':
                input_string = leArquivo(aux[1]);
                strs_dividida = processaString(tagsValidas, input_string, strs_dividida);
                input_string.clear();
                break;
            case 'c':
                input_tags = leArquivo(aux[1]);
                // Funcao de dividir chama a de validar a tag.
                tagsValidas = divideTag(input_tags, tagsValidas);
                input_tags.clear();
                break;
            case 'o':
                escreveArquivoStrings(aux[1], strs_dividida);
                break;
            case 'p':
                input_string.push_back(aux[1]);
                strs_dividida = processaString(tagsValidas, input_string, strs_dividida);
                input_string.clear();
                break;
            case 'a':
                listarAutomatos(tagsValidas);
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
                cout << "[ERROR] Opcao Invalida." << endl;
                break;
            }
        }
        opcao = "";
        aux.clear();
    } while (aux[0] != ":q");
}