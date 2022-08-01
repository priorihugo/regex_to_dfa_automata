#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <vector>
#include <set>
#include <list>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#pragma once
struct Transicao;

typedef struct Estado
{
    int id = -1;
    set<Transicao*> transicoes;
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
    void criaVisualizacao(string name);
    void processaString(string data);

private:
    void reduzParaAFD();
    void nomeiaEstados();

    set<Estado*> Estados;
    set<char> Alfabeto;
    set<Estado*> EstadosIniciais;
    set<Estado*> EstadosFinais;
};

#endif