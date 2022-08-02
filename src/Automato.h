#ifndef AUTOMATO_H
#define AUTOMATO_H

#include <vector>
#include <set>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

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

    void criaAutomato(string Expressao, string name_tag);
    void criaVisualizacao(string name);
    bool processaString(string data, int* posicao);
    void imprimeSetFormal(set<Estado *> s);

    set<Estado*> getEstados() {return this->Estados;};
    set<Estado*> getEstadosIniciais() {return this->EstadosIniciais;};
    set<Estado*> getEstadosFinais() {return this->EstadosFinais;};

private:
    void reduzParaAFD(string name_tag);
    void nomeiaEstados();

    set<Estado*> Estados;
    set<char> Alfabeto;
    set<Estado*> EstadosIniciais;
    set<Estado*> EstadosFinais;
};

#endif