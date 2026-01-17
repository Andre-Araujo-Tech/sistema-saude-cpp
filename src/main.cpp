#include <iostream>
#include <cmath>
#include <string>

#define INF 1000
using namespace std;

/* ===========================
   ESTRUTURAS DE CIDADES
=========================== */
int qtdCidades = 0;
int numeroCidade[INF];
bool hospital[INF];
string nomeCidade[INF];
int grafo[INF][INF];

/* ===========================
   ESTRUTURAS DE PROFISSIONAIS
=========================== */
struct Coord {
    int x, y;
};

struct Profissional {
    string nome;
    string especialidade;
    int crm;
    Coord pos;
};

struct DataType {
    string key;
    Profissional prof;
};

/* ===========================
   ÁRVORES BINÁRIAS
=========================== */
struct Node {
    DataType data;
    Node *left, *right;
};

Node *raizNome = NULL;
Node *raizEsp = NULL;

/* ===========================
   FUNÇÕES DE ÁRVORE
=========================== */
void insert(Node *&curr, DataType data) {
    if (!curr) {
        curr = new Node{data, NULL, NULL};
    } else if (data.key >= curr->data.key) {
        insert(curr->right, data);
    } else {
        insert(curr->left, data);
    }
}

Node* search(Node* curr, string key) {
    if (!curr) return NULL;
    if (key == curr->data.key) return curr;
    if (key > curr->data.key) return search(curr->right, key);
    return search(curr->left, key);
}

bool crmExiste(Node* curr, int crm) {
    if (!curr) return false;
    if (curr->data.prof.crm == crm) return true;
    return crmExiste(curr->left, crm) || crmExiste(curr->right, crm);
}

void listarPorNome(Node* curr) {
    if (!curr) return;
    listarPorNome(curr->left);
    cout << "| Nome: " << curr->data.prof.nome << endl;
    cout << "| Especialidade: " << curr->data.prof.especialidade << endl;
    cout << "| CRM: " << curr->data.prof.crm << endl;
    cout << "| Coordenadas: (" << curr->data.prof.pos.x << ", "
         << curr->data.prof.pos.y << ")\n\n";
    listarPorNome(curr->right);
}

void listarPorEspecialidade(Node* curr) {
    if (!curr) return;
    listarPorEspecialidade(curr->left);
    cout << "| Especialidade: " << curr->data.prof.especialidade << endl;
    cout << "| Nome: " << curr->data.prof.nome << endl;
    cout << "| CRM: " << curr->data.prof.crm << endl;
    cout << "| Coordenadas: (" << curr->data.prof.pos.x << ", "
         << curr->data.prof.pos.y << ")\n\n";
    listarPorEspecialidade(curr->right);
}

// Liberação de memória (Pós-ordem)
void liberarArvore(Node* curr) {
    if (!curr) return;
    liberarArvore(curr->left);
    liberarArvore(curr->right);
    delete curr;
}

/* ===========================
        ESPECIALIDADES
=========================== */
void escolherEspecialidade(string &esp) {
    int op;
    string lista[8] = {
        "Clinico Geral", "Cardiologia", "Pediatria", "Ortopedia",
        "Neurologia", "Dermatologia", "Psiquiatria", "Ginecologia"
    };

    cout << "\n=== ESPECIALIDADES ===\n";
    for (int i = 0; i < 8; i++)
        cout << i + 1 << ". " << lista[i] << endl;

    do {
        cout << "Escolha: ";
        cin >> op;
        cin.ignore();
    } while (op < 1 || op > 8);

    esp = lista[op - 1];
}

/* ===========================
        CADASTROS
=========================== */
void cadastrarCidade() {
    if (qtdCidades >= INF - 1) {
        cout << "Limite de cidades atingido.\n";
        return;
    }
    qtdCidades++;
    numeroCidade[qtdCidades] = qtdCidades;

    cout << "Nome da cidade/bairro: ";
    getline(cin, nomeCidade[qtdCidades]);

    cout << "Possui hospital? (1-Sim 2-Nao): ";
    int op;
    cin >> op;
    cin.ignore();
    hospital[qtdCidades] = (op == 1);

    cout << "Cidade " << nomeCidade[qtdCidades] << " cadastrada com ID: " << qtdCidades << endl;
}

void cadastrarEstrada() {
    int o, d;
    cout << "ID Origem: ";
    cin >> o;
    cout << "ID Destino: ";
    cin >> d;
    cin.ignore();

    if (o < 1 || d < 1 || o > qtdCidades || d > qtdCidades) {
        cout << "Erro: IDs de cidades invalidos.\n";
        return;
    }

    grafo[o][d] = 1; // Orientado (mão única). Para mão dupla, adicione grafo[d][o] = 1;
    cout << "Via cadastrada entre " << nomeCidade[o] << " e " << nomeCidade[d] << "!\n";
}

void cadastrarProfissional() {
    Profissional p;
    DataType dNome, dEsp;

    cout << "Nome do profissional: ";
    getline(cin, p.nome);
    while (search(raizNome, p.nome)) {
        cout << "Nome ja existe. Digite outro: ";
        getline(cin, p.nome);
    }

    escolherEspecialidade(p.especialidade);

    cout << "CRM: ";
    cin >> p.crm;
    while (crmExiste(raizNome, p.crm)) {
        cout << "CRM ja cadastrado. Digite outro: ";
        cin >> p.crm;
    }

    cout << "Coordenada X: ";
    cin >> p.pos.x;
    cout << "Coordenada Y: ";
    cin >> p.pos.y;
    cin.ignore();

    dNome.key = p.nome;
    dNome.prof = p;
    insert(raizNome, dNome);

    dEsp.key = p.especialidade;
    dEsp.prof = p;
    insert(raizEsp, dEsp);

    cout << "Profissional cadastrado com sucesso!\n";
}

/* ===========================
   BUSCA DE HOSPITAL (BFS)
=========================== */
void buscarHospital() {
    int inicio;
    cout << "ID da cidade onde voce esta: ";
    cin >> inicio;
    cin.ignore();

    if (inicio < 1 || inicio > qtdCidades) {
        cout << "Cidade invalida.\n";
        return;
    }

    bool visitado[INF] = {false};
    int fila[INF], frente = 0, tras = 0;

    fila[tras++] = inicio;
    visitado[inicio] = true;

    while (frente < tras) {
        int atual = fila[frente++];
        if (hospital[atual]) {
            cout << "\n>>> HOSPITAL ENCONTRADO! <<<\n";
            cout << "Cidade: " << nomeCidade[atual] << " (ID: " << atual << ")\n";
            return;
        }
        for (int i = 1; i <= qtdCidades; i++) {
            if (grafo[atual][i] != -1 && !visitado[i]) {
                visitado[i] = true;
                fila[tras++] = i;
            }
        }
    }
    cout << "Nenhum hospital acessivel a partir desta localidade.\n";
}

/* ===========================
   PROFISSIONAIS PRÓXIMOS
=========================== */
double dist(Coord a, Coord b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void buscarProximos(Node* curr, Coord c, int &cont) {
    if (!curr) return;
    buscarProximos(curr->left, c, cont);

    double d = dist(curr->data.prof.pos, c);
    if (d <= 100) {
        cont++;
        cout << "- " << curr->data.prof.nome 
             << " [" << curr->data.prof.especialidade 
             << "] Distancia: " << d << " unidades\n";
    }

    buscarProximos(curr->right, c, cont);
}

/* ===========================
            MENU
=========================== */
void menu() {
    cout << "\n==============================\n";
    cout << "      SISTEMA DE SAUDE       \n";
    cout << "==============================\n";
    cout << "1. Cadastrar cidade\n";
    cout << "2. Cadastrar via (estrada)\n";
    cout << "3. Buscar hospital mais proximo (Grafo)\n";
    cout << "4. Cadastrar profissional\n";
    cout << "5. Listar profissionais por nome (BST)\n";
    cout << "6. Listar por especialidade (BST)\n";
    cout << "7. Buscar profissionais proximos (Coord)\n";
    cout << "0. Sair\n";
    cout << "Escolha uma opcao: ";
}

int main() {
    // Inicializa a matriz do grafo com -1 (sem conexões)
    for(int i = 0; i < INF; i++) {
        for(int j = 0; j < INF; j++) {
            grafo[i][j] = -1;
        }
    }

    int op;
    do {
        menu();
        if(!(cin >> op)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entrada invalida. Digite um numero.\n";
            continue;
        }
        cin.ignore();

        switch (op) {
            case 1: cadastrarCidade(); break;
            case 2: cadastrarEstrada(); break;
            case 3: buscarHospital(); break;
            case 4: cadastrarProfissional(); break;
            case 5: 
                cout << "\n--- LISTA POR NOME ---\n";
                listarPorNome(raizNome); 
                break;
            case 6: 
                cout << "\n--- LISTA POR ESPECIALIDADE ---\n";
                listarPorEspecialidade(raizEsp); 
                break;
            case 7: {
                Coord c;
                cout << "Sua coordenada X: "; cin >> c.x;
                cout << "Sua coordenada Y: "; cin >> c.y;
                cin.ignore();
                int cont = 0;
                cout << "\nBuscando profissionais em um raio de 100 unidades...\n";
                buscarProximos(raizNome, c, cont);
                cout << "Total: " << cont << " profissionais encontrados.\n";
                break;
            }
            case 0:
                cout << "Encerrando e liberando memoria...\n";
                liberarArvore(raizNome);
                liberarArvore(raizEsp);
                break;
            default:
                cout << "Opcao inexistente.\n";
        }
    } while (op != 0);

    return 0;
}
