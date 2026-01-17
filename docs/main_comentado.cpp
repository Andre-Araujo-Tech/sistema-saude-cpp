#include <iostream>
#include <cmath>
#include <string>

// Define um limite máximo para cidades/profissionais.
// Usado para evitar estouro de memória e simplificar o controle de índices.
#define INF 1000 
using namespace std;

/* ===========================================================
   ESTRUTURAS DE CIDADES (GRAFOS)
   Aqui usamos Matriz de Adjacência para representar o mapa.
   =========================================================== */
int qtdCidades = 0;              // Contador global de cidades cadastradas
int numeroCidade[INF];           // Armazena os IDs das cidades
bool hospital[INF];              // Vetor booleano: true se a cidade tem hospital
string nomeCidade[INF];          // Armazena o nome correspondente ao ID
int grafo[INF][INF];             // Matriz de Adjacência (representa as estradas)

/* ===========================================================
   ESTRUTURAS DE PROFISSIONAIS
   =========================================================== */
struct Coord {
    int x, y; // Localização cartesianas para cálculo de distância
};

struct Profissional {
    string nome;
    string especialidade;
    int crm;
    Coord pos;
};

// Estrutura auxiliar para permitir que a BST use diferentes chaves (Nome ou Especialidade)
struct DataType {
    string key;      // A chave de ordenação da árvore
    Profissional prof;
};

/* ===========================================================
   ÁRVORES BINÁRIAS DE BUSCA (BST)
   Utilizadas para organizar profissionais de forma que a busca 
   seja O(log n) em média.
   =========================================================== */
struct Node {
    DataType data;
    Node *left, *right;
};

// Raízes globais para as duas árvores de indexação
Node *raizNome = NULL; // Organizada por ordem alfabética de Nome
Node *raizEsp = NULL;  // Organizada por Especialidade

/* ===========================================================
   FUNÇÕES DE ÁRVORE
   =========================================================== */

// Inserção em BST: O(log n) em média, O(n) no pior caso
// Insere um novo profissional na árvore respeitando a ordem da 'key'
void insert(Node *&curr, DataType data) {
    if (!curr) {
        curr = new Node{data, NULL, NULL};
    } else if (data.key >= curr->data.key) {
        insert(curr->right, data); // Maiores ou iguais vão para a direita
    } else {
        insert(curr->left, data);  // Menores vão para a esquerda
    }
}

// Busca um nó específico na árvore através de uma string (nome ou especialidade)
Node* search(Node* curr, string key) {
    if (!curr) return NULL;
    if (key == curr->data.key) return curr;
    if (key > curr->data.key) return search(curr->right, key);
    return search(curr->left, key);
}

// Verifica se um CRM já existe percorrendo toda a árvore (Busca exaustiva)
bool crmExiste(Node* curr, int crm) {
    if (!curr) return false;
    if (curr->data.prof.crm == crm) return true;
    return crmExiste(curr->left, crm) || crmExiste(curr->right, crm);
}

// Percurso In-Order (Esquerda - Raiz - Direita) para listar em ordem alfabética
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

// Similar ao listarPorNome, mas focado na árvore indexada por especialidade
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

// Limpa os nós da memória usando Pós-ordem (Filhos antes do Pai) para evitar memory leak
void liberarArvore(Node* curr) {
    if (!curr) return;
    liberarArvore(curr->left);
    liberarArvore(curr->right);
    delete curr;
}

/* ===========================================================
   FUNÇÕES DE APOIO E CADASTRO
   =========================================================== */

// Garante que o usuário escolha apenas especialidades válidas do sistema
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

// Cria um "Vértice" no nosso Grafo de cidades
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

// Cria uma "Aresta" direcionada entre dois vértices do Grafo
void cadastrarEstrada() {
    int o, d;
    cout << "ID Origem: "; cin >> o;
    cout << "ID Destino: "; cin >> d;
    cin.ignore();

    if (o < 1 || d < 1 || o > qtdCidades || d > qtdCidades) {
        cout << "Erro: IDs de cidades invalidos.\n";
        return;
    }

    grafo[o][d] = 1; // 1 indica que existe conexão
    cout << "Via cadastrada entre " << nomeCidade[o] << " e " << nomeCidade[d] << "!\n";
}

// Captura dados e insere o profissional nas duas árvores de indexação (Nome e Especialidade)
void cadastrarProfissional() {
    Profissional p;
    DataType dNome, dEsp;

    cout << "Nome do profissional: ";
    getline(cin, p.nome);
    // Verificação de duplicidade de nome usando a BST (muito rápido)
    while (search(raizNome, p.nome)) {
        cout << "Nome ja existe. Digite outro: ";
        getline(cin, p.nome);
    }

    escolherEspecialidade(p.especialidade);

    cout << "CRM: ";
    cin >> p.crm;
    // Verificação de duplicidade de CRM
    while (crmExiste(raizNome, p.crm)) {
        cout << "CRM ja cadastrado. Digite outro: ";
        cin >> p.crm;
    }

    cout << "Coordenada X: "; cin >> p.pos.x;
    cout << "Coordenada Y: "; cin >> p.pos.y;
    cin.ignore();

    dNome.key = p.nome;
    dNome.prof = p;
    insert(raizNome, dNome); // Insere na árvore de nomes

    dEsp.key = p.especialidade;
    dEsp.prof = p;
    insert(raizEsp, dEsp);   // Insere na árvore de especialidades

    cout << "Profissional cadastrado com sucesso!\n";
}

/* ===========================================================
   ALGORITMOS DE BUSCA ESPACIAL E EM GRAFO
   =========================================================== */

// Busca em Largura (BFS): Encontra o hospital mais próximo em termos de conexões (estradas)
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

    // A BFS garante o caminho com menor número de saltos entre cidades
    
    while (frente < tras) {
        int atual = fila[frente++];
        
        if (hospital[atual]) {
            cout << "\n>>> HOSPITAL ENCONTRADO! <<<\n";
            cout << "Cidade: " << nomeCidade[atual] << " (ID: " << atual << ")\n";
            return;
        }

        for (int i = 1; i <= qtdCidades; i++) {
            // Se houver estrada e a cidade não foi visitada
            if (grafo[atual][i] == 1 && !visitado[i]) {
                visitado[i] = true;
                fila[tras++] = i;
            }
        }
    }
    cout << "Nenhum hospital acessivel a partir desta localidade.\n";
}

// Calcula a distância euclidiana entre dois pontos (Geometria)
double dist(Coord a, Coord b) {
    // Fórmula: sqrt((x2-x1)^2 + (y2-y1)^2)
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Percorre a árvore e verifica quais profissionais estão dentro do raio de 100 unidades
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

/* ===========================================================
   INTERFACE DE USUÁRIO
   =========================================================== */
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
    // Inicializa a matriz do grafo com -1 (indicando ausência de estradas)
    for(int i = 0; i < INF; i++) {
        for(int j = 0; j < INF; j++) {
            grafo[i][j] = -1;
        }
    }

    int op;
    do {
        menu();
        if(!(cin >> op)) { // Proteção contra entradas que não sejam números
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
