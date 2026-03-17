# 🏃‍♂️ Ranking de Atletas: Estruturas de Dados & Performance

Este projeto foi desenvolvido para a disciplina de Estrutura de Dados na **UFCA**. O objetivo é gerenciar um ranking de competições de atletismo, comparando o desempenho de diversos algoritmos de ordenação aplicados sobre listas **Encadeadas Estáticas** (vetores) e **Dinâmicas** (ponteiros).

## 📄 Artigo Técnico
Acesse a análise completa: **[PDF: Análise de Desempenho](https://github.com/lfariazzz/data-structures-ranking/blob/main/docs/Artigo%20Final.pdf)**

Este estudo fundamenta o projeto e identifica o **Merge Sort** como o algoritmo ideal para rankings ao vivo, garantindo previsibilidade $O(n \log n)$ e estabilidade nos resultados.

## ⚠️ Recomendação de Ambiente (VS Code + Windows)

Para garantir que o projeto compile sem erros de linkagem ou bibliotecas ausentes, **recomenda-se fortemente o uso do MSYS2** como ambiente de desenvolvimento integrado ao VS Code.

## 🛠️ Guia de Execução

Utilize o terminal (preferencialmente via MSYS2 no Windows) para executar os comandos abaixo na raiz do projeto:

### 1. Compilar o projeto
O projeto utiliza um **Makefile** para gerenciar a compilação modular. Os arquivos de objeto (`.o`) serão gerados automaticamente na pasta `obj/` para otimizar builds futuros.
```bash
make
```

### 2. Rodar a demonstração do ranking
Para visualizar os rankings estático e dinâmico com os dados de teste carregados no main.c, confirmando que as estruturas estão sincronizadas:
```bash
./ranking
```

### 3. Rodar os benchmarks e salvar dados para os gráficos
Para executar a bateria completa de testes de desempenho (Bubble, Selection, Insertion, Quick e Merge Sort) e exportar os resultados para análise quantitativa:
```bash
# Executa os testes e redireciona a saída formatada para o CSV
./ranking --benchmark 100 > data/resultados.csv
```

### 4. Gerar os gráficos de análise
Com o arquivo CSV populado, utilize o script Python para criar as visualizações SVG na pasta graficos/:
```bash
# Executa o script de geração de imagens
python graficos.py
```

### 5. Limpar arquivos temporários
Para remover os binários, arquivos de objeto e o CSV de resultados, garantindo uma compilação totalmente "limpa" e livre de resíduos de versões antigas:
```bash
make clean
``` 

---

### Por que usar o MSYS2?
* **Suporte a Nanosegundos**: O compilador GCC fornecido pelo MSYS2 suporta nativamente as bibliotecas de alta precisão necessárias para medições de tempo em nanosegundos (`tempo_ns`), essenciais para distinguir a performance de algoritmos eficientes.
* **Incompatibilidade de Compiladores**: Versões antigas de compiladores (como as integradas no Code::Blocks padrão ou Dev-C++) costumam falhar ao processar cabeçalhos modernos ou ao linkar múltiplos arquivos `.c`.
* **Ferramentas POSIX**: O MSYS2 traz utilitários como `make` e `git` de forma nativa para o terminal do Windows, padronizando o fluxo de trabalho para todos os membros do grupo.

## 📂 Estrutura do Projeto

A arquitetura do projeto segue o padrão de separação de responsabilidades para facilitar a manutenção técnica entre os colaboradores:

* **`src/`**: Implementações lógicas dos algoritmos, estruturas e gerenciamento de memória.
* **`include/`**: Cabeçalhos (`.h`) contendo os contratos das funções e as structs do sistema.
* **`obj/`**: Pasta de binários temporários gerados durante a compilação.
* **`data/`**: Local onde as amostras de entrada e os resultados CSV são armazenados.
* **`graficos/`**: Pasta de saída para as análises visuais de complexidade.

---
### 👥 Equipe do Projeto
| Desenvolvedor | Perfil GitHub | Função |
| :--- | :--- | :--- |
| **Levi Farias** | [@lfariazzz](https://github.com/lfariazzz) | Líder Técnico, Headers & Estruturas de Dados|
| **Henrique Coimbra** | [@HenriqueCoimbra12](https://github.com/HenriqueCoimbra12) | Algoritmos de Ordenação, Benchmarks & Documentação |
| **Malaquias** | [@malaquiaso841-cyber](https://github.com/malaquiaso841-cyber) | Algoritmos de Ordenação, Benchmarks & Documentação |
| **Wesley** | [@awesleyy](https://github.com/awesleyy) | Algoritmos de Ordenação, Benchmarks & Documentação |

### **Instituição**: Universidade Federal do Cariri (UFCA)
