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

    a.criaAutomato("ab+c+x+*", "TESTE");
    a.criaVisualizacao("TESTEFINAL");

    a.processaString("aabcaa", 0);

    return 0;

}