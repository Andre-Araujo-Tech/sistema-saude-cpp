# 🏥 Sistema de Gestão de Saúde Urbana

Este projeto é uma aplicação em **C++** desenvolvida para gerenciar a infraestrutura de saúde de uma região. Ele utiliza algoritmos de **Grafos** para mapear cidades e encontrar hospitais, e **Árvores Binárias de Busca (BST)** para organizar e pesquisar profissionais de saúde.

## 🕹️ Funcionalidades

### 📍 Gestão de Localidades (Grafos)
* **Cadastro de Cidades:** Registro de nomes e identificação de presença de hospitais.
* **Mapeamento de Vias:** Criação de conexões (estradas) entre cidades usando uma **Matriz de Adjacência**.
* **Busca por Hospital:** Implementação do algoritmo **BFS (Busca em Largura)** para encontrar o hospital mais próximo em termos de conexões.

### 👩‍⚕️ Gestão de Profissionais (Árvores Binárias)
* **Indexação por Nome e Especialidade:** Utiliza duas árvores distintas para buscas rápidas.
* **Busca por Proximidade:** Localiza profissionais em um raio de 100 unidades de distância com base em coordenadas cartesianas ($x, y$).
* **Validação de CRM:** Impede o cadastro duplicado de profissionais.

---

## 🛠️ Tecnologias e Estruturas de Dados

* **Linguagem:** C++
* **Grafo:** Representado por Matriz de Adjacência para conexões entre cidades.

* **Busca em Largura (BFS):** Utilizada para navegar no grafo e encontrar o hospital mais próximo de forma eficiente.

* **Árvore Binária de Busca (BST):** Para armazenamento eficiente e listagem ordenada de profissionais.

* **Geometria Analítica:** Cálculo de distância euclidiana para busca por proximidade.

---

## 📋 Como Executar

1.  Certifique-se de ter um compilador C++ instalado (GCC/G++).

2.  Clone o repositório:
    ```bash
    git clone https://github.com/Andre-Araujo-Tech/sistema-saude-cpp
    ```

3.  Acesse a pasta do projeto:
    ```bash
    cd sistema-saude-cpp
    ```

4.  Compile o código:
    ```bash
    g++ -o sistema_saude main.cpp
    ```
5.  Execute o programa:
    ```bash
    ./sistema_saude
    ```

---

## 📖 Exemplo de Uso

1.  **Cadastre cidades:** Identifique quais possuem hospitais (ex: ID 1, ID 2).
2.  **Crie estradas:** Conecte as cidades pelos seus IDs (ex: Origem 2 -> Destino 1).
3.  **Localize emergências:** Use a opção de busca para saber qual cidade com hospital é acessível a partir da sua localização atual através do grafo.
4.  **Gerencie especialistas:** Cadastre médicos e liste-os por especialidade ou ordem alfabética.

---

## 📂 Organização do Projeto

- `src/main.cpp`  
  Código principal do sistema, mantendo foco na funcionalidade e clareza.

- `docs/main_comentado.cpp`  
  Versão didática do código, com comentários detalhados explicando
  estruturas de dados, algoritmos e fluxo do sistema.

---