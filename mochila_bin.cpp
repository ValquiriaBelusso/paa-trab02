#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

struct item
{
    int idx;
    int custo;
    int beneficio;
    float custo_beneficio;
};

void populateBenefits(vector<item> &itens, ifstream &myFile, int tamanho)
{
    string aux;
    for (int i = 0; i < tamanho; i++)
    {
        itens.push_back(item());
        getline(myFile, aux, '\t');
        itens[i].idx = i + 1;
        itens[i].beneficio = stoi(aux);
    }
}

void populateCostBenefit(vector<item> &itens, ifstream &myFile, int tamanho)
{
    string aux;
    for (int i = 0; i < tamanho; i++)
    {
        getline(myFile, aux, '\t');
        itens[i].custo = stoi(aux);
        itens[i].custo_beneficio = static_cast<float>(itens[i].beneficio) / itens[i].custo;
    }
}

int findBest(vector<item> itens)
{
    item largest = itens[0];
    int j = 0;
    for (int i = 1; i < itens.size(); i++)
    {
        if (itens[i].custo_beneficio > largest.custo_beneficio)
        {
            largest = itens[i];
            j = i;
        }
    }
    return j;
}

void quickSort(vector<item> &vet, int left, int right)
{
    int i = left, j = right;
    item aux;
    float pivo;
    pivo = (vet[left].custo_beneficio + vet[i + j / 2].custo_beneficio + vet[right].custo_beneficio) / 3;

    while (i <= j)
    {
        while (vet[i].custo_beneficio < pivo && i < right)
            i++;
        while (vet[j].custo_beneficio > pivo && j > left)
            j--;
        if (i <= j)
        {
            aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
            i++;
            j--;
        }
    }
    if (j > left)
        quickSort(vet, left, j);
    if (i < right)
        quickSort(vet, i, right);
}

vector<item> mochilaBinaria(vector<item> itens, int capacidade)
{
    vector<item> itens_selecionados;
    item bestItem;
    int peso = 0;
    int aux;
    while ((peso < capacidade) && (itens.size() > 0))
    {
        aux = findBest(itens);
        bestItem = itens[aux];

        int a = min(bestItem.custo, capacidade - peso);
        if (a == bestItem.custo)
        {
            peso += a;
            itens_selecionados.push_back(bestItem);
            itens.erase(itens.begin() + aux);
        }
        else
        {
            itens.erase(itens.begin() + aux);
        }
    }

    return itens_selecionados;
}

vector<item> mochilaBinariaOrdenada(vector<item> itens, int capacidade)
{
    vector<item> itens_selecionados;
    item bestItem;
    int peso = 0;

    quickSort(itens, 0, itens.size() - 1);

    while ((peso < capacidade) && (itens.size() > 0))
    {
        bestItem = itens.back();
        int a = min(bestItem.custo, capacidade - peso);
        if (a == bestItem.custo)
        {
            peso += a;
            itens_selecionados.push_back(bestItem);
            itens.pop_back();
        }
        else itens.pop_back();

    }

    return itens_selecionados;
}

int main()
{
    int sizes[15] = {10, 50, 100, 200, 300, 500, 750, 1000, 1250, 1500, 2000, 2500, 3000, 4000, 5000};
    for (int size : sizes)
    {
        string filename = "./paa02/Mochila" + to_string(size) + ".txt";
        ifstream myFile(filename);
        if (!myFile.is_open())
        {
            cout << "Erro ao abrir arquivo" << endl;
            return 0;
        }

        vector<item> itens;
        string aux = "";
        int capacidade;
        getline(myFile, aux);
        capacidade = stoi(aux);

        populateBenefits(itens, myFile, size);
        getline(myFile, aux);
        populateCostBenefit(itens, myFile, size);

        auto start = chrono::steady_clock::now();

        vector<item> mochila = mochilaBinaria(itens, capacidade);

        ofstream bin("./resultados/mochilaBinaria.txt", ios_base::app);

        if (!bin.is_open())
        {
            cout << "Erro ao abrir arquivo" << endl;
            return 0;
        }

        bin << endl << endl << "RESULTADO MOCHILA " << size << " ITENS:" << endl;
        int b = 0, n = 0, p = 0;

        for (item i : mochila)
        {
            n++;
            b += i.beneficio;
            p += i.custo;
        }
        bin << endl
            << "Capacidade final: " << p << " de " << capacidade << endl
            << "Benefício total: " << b << endl
            << "Número de itens: " << n << endl;
    
        auto end = chrono::steady_clock::now();
        bin << "Tempo de execução: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns" << endl;

        bin.close();

        auto startOrd = chrono::steady_clock::now();
        vector<item> mochilaOrdenada = mochilaBinariaOrdenada(itens, capacidade);

        ofstream binOrd("./resultados/mochilaBinariaOrdenada.txt", ios_base::app);

        binOrd << endl << endl << "RESULTADO MOCHILA ORDENADA " << size << " ITENS:" << endl;
        b = 0, n = 0, p = 0;

        for (item i : mochilaOrdenada)
        {
            n++;
            b += i.beneficio;
            p += i.custo;
        }
        binOrd << endl
               << "Capacidade final: " << p << " de " << capacidade << endl
               << "Benefício total: " << b << endl
               << "Número de itens: " << n << endl;

        auto endOrd = chrono::steady_clock::now();
        binOrd << "Tempo de execução: " << chrono::duration_cast<chrono::nanoseconds>(endOrd - startOrd).count() << " ns" << endl;
        binOrd.close();

        myFile.close();
    }
}
