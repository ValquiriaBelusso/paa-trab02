#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>
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
            // break (Se item nao couber, o que fazer?)
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

    cout << endl
         << "Vetor ordenado" << endl;
    for (item i : itens)
    {
        cout << i.idx << " " << i.beneficio << " " << i.custo << " " << i.custo_beneficio << endl;
    }

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
        else
        {
            itens.pop_back();
            // break (Se item nao couber, o que fazer?)
        }
    }
    cout << "RESULTADO" << endl;
    int b = 0;
    for (item i : itens_selecionados)
    {
        cout << i.idx << " " << i.beneficio << " " << i.custo << " " << i.custo_beneficio << endl;
        b += i.beneficio;
    }
    cout << "Peso ocupado da mochila: " << peso << endl;
    cout << "Benefício total: " << b << endl;
    return itens_selecionados;
}

int main()
{
    string filename = "./paa02/Mochila2000.txt";
    ifstream myFile(filename);
    if (!myFile.is_open())
    {
        cout << "Erro ao abrir arquivo";
        return 0;
    }
    vector<item> itens;
    string aux = "";
    int capacidade;
    getline(myFile, aux);
    capacidade = stoi(aux);
    cout << capacidade << endl;

    populateBenefits(itens, myFile, 2000);
    getline(myFile, aux);
    populateCostBenefit(itens, myFile, 2000);

    vector<item> mochila = mochilaBinaria(itens, capacidade);

    cout << "RESULTADO" << endl;
    int b = 0;
    for (item i : mochila)
    {
        // cout << i.idx << " ";
        b += i.beneficio;
    }
    cout << endl
         << "Benefício total: " << b << endl;

    vector<item> mochilaOrdenada = mochilaBinariaOrdenada(itens, capacidade);
    b = 0;
    cout << "RESULTADO" << endl;
    for (item i : mochilaOrdenada)
    {
        // cout << i.idx << " ";
        b += i.beneficio;
    }
    cout << endl
         << "Benefício total: " << b << endl;
}