#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <vector>
#include <string>
#include <fstream>

using namespace std;

#pragma once
struct Transicao;

typedef struct Estado
{
    int id = -1;
    bool inicio = false;
    bool fim = false;
    vector<Transicao*> transicoes;
}Estado;

typedef struct Transicao
{
    char tipo;
    Estado* destino;

}Transicao;

class Automato
{
public:
    Automato();
    ~Automato();

    void criaAutomato(string Expressao);
    void criaVisualizacao( string name);

private:

    void reduzParaAFN();
    void reduzParaAFD();
    void nomeiaEstados();

    vector<Estado*> Estados;
    vector<Estado*> EstadosIniciais;
    vector<Estado*> EstadosFinais;

};

#endif