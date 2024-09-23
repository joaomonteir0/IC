#include <fstream>
#include <iostream>

using namespace std;

int main() {
    ifstream infile;
    infile.open("teste.txt", ios::binary); 

    if (infile.fail()) {
        cout << "Error opening the file" << endl;
        return 1;
    }

    infile.seekg(0, ios::end); // ir para o fim do ficheiro
    streamsize size = infile.tellg();
    infile.seekg(0, ios::beg); // voltar ao inicio do ficheiro
    
    char* data = new char[size];

    // ler para o array c/size 
    infile.read(data, size);

    cout << "Size: " << size << " bytes" << endl;
    cout << data << endl;

    free(data);

    return 0;
}