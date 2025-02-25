# graph-bridge
Análise do uso de códigos de localização de ponte para aplicação em outros algoritmos


## Como compilar (cmake)

### Requisitos
- Tenha cmake instalado

### Passos
- Na pasta principal do repositório, use `cmake --build .` na pasta principal do repositório

- O executável está disposto na posta code, com nome MyProject


Qualquer arquivo novo deve ser incluido em CMakeLists.txt
 
O destino do executável ainda está em discussão


## Como compilar (gcc)

### Passos
- Na pasta principal, imprima: 

```(cd code && g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iutils -Igraph main.cpp eulerian.cpp naive.cpp tarjan.cpp utils/randomizer.cpp graph/graph_reader.cpp graph/graph.cpp -o MyProject)```

