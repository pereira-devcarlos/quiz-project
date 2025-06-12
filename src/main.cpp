#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <limits>

using namespace std;

struct jogador{
    string name;
    int pontos;
};

// Função para ler jogadores do arquivo
int lerJogadores(ifstream& arquivoJ, jogador player[], int maxPlayers) {
    int quantPlayers = 0;
    string pontoString;
    while (quantPlayers < maxPlayers && getline(arquivoJ, player[quantPlayers].name)) {
        if (!getline(arquivoJ, pontoString)) break;
        player[quantPlayers].pontos = stoi(pontoString);
        quantPlayers++;
    }
    return quantPlayers;
}

int main(){
    const int TAM=10;
    int menu = 2, quantPlayers = 0;

    // Abrir arquivos
    ifstream arquivoP("../../data/perguntas.txt");
    ifstream arquivoJ("../../data/jogadores.txt");
    if (!arquivoP.is_open() || !arquivoJ.is_open()) {
        cout << "Erro: algum dos arquivos nao foi aberto corretamente!" << endl;
        return 1;
    }

    jogador player[50];
    string perguntas[TAM];
    string opcoes[TAM][4];
    string correta[TAM];
    string resposta[TAM];

    // Lê jogadores existentes no início
    quantPlayers = lerJogadores(arquivoJ, player, 50);
    arquivoJ.close(); // Fecha para evitar conflito com escrita

    while (menu != 3) {
        cout << "\n\tQuiz de Programacao" << endl;
        cout << "[1] Jogar" << endl << "[2] Ranking" << endl << "[3] Sair" << endl;
        cout << "Digite a opcao que voce deseja: ";
        cin >> menu;

        while (menu < 1 || menu > 3) {
            cout << "\nErro: opcao invalida!!!" << endl;
            cout << "Digite a opcao que voce deseja: ";
            cin >> menu;
        }

        switch (menu) {
            case 1: {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Digite seu nome: ";
                getline(cin, player[quantPlayers].name);
                while (player[quantPlayers].name.empty() || player[quantPlayers].name.find_first_not_of(' ') == string::npos) {
                    cout << "Nome inválido! Digite seu nome: ";
                    getline(cin, player[quantPlayers].name);
                }
                player[quantPlayers].pontos = 0;

                // Reposiciona ponteiro do arquivo de perguntas
                arquivoP.clear();
                arquivoP.seekg(0);

                // Coletando os dados das perguntas
                for (int i = 0; i < TAM; i++) {
                    getline(arquivoP, perguntas[i]);
                    for (int j = 0; j < 4; j++) {
                        getline(arquivoP, opcoes[i][j]);
                    }
                    getline(arquivoP, correta[i]);
                }

                // Verificando as perguntas e alternativas
                for (int i = 0; i < TAM; i++) {
                    cout << endl << i + 1 << "-" << perguntas[i] << endl;
                    for (int j = 0; j < 4; j++) {
                        cout << opcoes[i][j] << endl;
                    }
                    cout << "R: ";
                    cin >> resposta[i];
                    resposta[i][0] = toupper(resposta[i][0]);
                    if (resposta[i] == correta[i]) {
                        player[quantPlayers].pontos += 10;
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cout << player[quantPlayers].name << " obteve " << player[quantPlayers].pontos << endl;

                // Salva novo jogador no arquivo
                ofstream arquivoE("../../data/jogadores.txt", ios::app);
                if (arquivoE.is_open()) {
                    arquivoE << player[quantPlayers].name << endl;
                    arquivoE << player[quantPlayers].pontos << endl;
                    arquivoE.close();
                } else {
                    cout << "Erro ao salvar jogador!" << endl;
                }
                quantPlayers++; // Adiciona novo jogador ao vetor
                break;
            }
            case 2: {
                // Reabre arquivo para garantir leitura atualizada
                ifstream arquivoJr("../../data/jogadores.txt");
                if (!arquivoJr.is_open()) {
                    cout << "Erro ao abrir jogadores.txt para leitura do ranking!" << endl;
                    break;
                }
                quantPlayers = lerJogadores(arquivoJr, player, 50);
                arquivoJr.close();

                // Ordenar o vetor para que fique do maior para o menor
                for (int i = 0; i < quantPlayers-1; i++){
                    for (int j = 0; j < quantPlayers-i-1; j++){
                        if (player[j].pontos < player[j+1].pontos){
                            swap(player[j], player[j+1]);
                        }
                    }
                }

                // Exibir Ranking
                cout << "\n     Ranking Quiz C++" << endl;
                cout << "\n==========================" << endl;
                for (int i = 0; i < 3 && i < quantPlayers; i++){
                    cout << "\t" << i+1 << " Lugar" << endl;
                    cout << "   " << player[i].name << " " << player[i].pontos << "pts" << endl;
                    cout << "==========================" << endl;
                }

                cout << "\nDeseja continuar no jogo [1]-Sim ou [2]-Nao: ";
                int op;
                cin >> op;
                if (op == 2) {
                    menu = 3;
                    cout << "\nObrigado por acessar meu jogo, volte sempre!!!" << endl;
                }
                break;
            }
            case 3:
                cout << "\nVolte sempre, basta executar o jogo novamente!" << endl;
                break;
        }
    }

    return 0;
}
