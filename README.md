# graph-bridge
Análise do uso de códigos de localização de ponte para aplicação em outros algoritmos


## Como compilar (cmake)

### Requisitos
- Tenha cmake instalado

### Passos
- Na pasta principal do repositório, execute `setup.sh` (macOS e Linux) ou `setup.bat` (Windows)

- O executável estará disposto como `code/MyProject` ou `code/MyProject.exe`

- Para compilações subsequentes, apenas é necessário usar `cmake .` na pasta principal do repositório


Qualquer arquivo novo deve ser incluido em CMakeLists.txt
 
O destino do executável ainda está em discussão


## Como compilar (Linux gcc)

### Passos
- Na pasta principal, imprima: 

```(cd code && g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iutils -Igraph main.cpp eulerian.cpp naive.cpp tarjan.cpp utils/randomizer.cpp graph/graph_reader.cpp graph/graph.cpp -o MyProject)```


## Como usar Debug (VsCode Linux)
- Em qualquer .cpp, selecione Debug in C++, e logo em seguida a opção `Debug MyProject (Linux)`