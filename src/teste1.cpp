#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "./Automato.h"

using namespace std;

int main(){

    Automato a;
    //ofstream out;
    //out.open("TESTE.dot");

    a.criaAutomato("ab*+");
    a.criaVisualizacao("TESTEFINAL");

    return 0;

}