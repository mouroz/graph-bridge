CXX = g++
CXXFLAGS = -std=c++11 -Wall
INC = -I.

# Diretórios
TARJAN_DIR = tarjan
EULERIAN_DIR = eulerianPath
BUILD_DIR = build

# Arquivos fonte
TARJAN_SRCS = $(TARJAN_DIR)/graph.cpp $(TARJAN_DIR)/tarjan.cpp $(TARJAN_DIR)/graph_reader.cpp
EULERIAN_SRCS = $(EULERIAN_DIR)/eulerian.cpp

# Objetos
TARJAN_OBJS = $(TARJAN_SRCS:%.cpp=$(BUILD_DIR)/%.o)
EULERIAN_OBJS = $(EULERIAN_SRCS:%.cpp=$(BUILD_DIR)/%.o)
ALL_OBJS = $(TARJAN_OBJS) $(EULERIAN_OBJS)

# Executável
TARGET = $(EULERIAN_DIR)/eulerian

# Regra principal
all: directories $(TARGET)

# Criar diretórios necessários
directories:
	mkdir -p $(BUILD_DIR)/$(TARJAN_DIR)
	mkdir -p $(BUILD_DIR)/$(EULERIAN_DIR)

# Regra para o executável
$(TARGET): $(ALL_OBJS)
	$(CXX) $(ALL_OBJS) -o $@

# Regras para os objetos
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# Limpar arquivos gerados
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

# Executar o programa
run: all
	./$(TARGET)

.PHONY: all clean run directories 