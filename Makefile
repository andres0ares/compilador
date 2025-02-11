# Nome do executável
TARGET = compilador

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Diretórios de código-fonte e binário
SRC_DIR = src
BUILD_DIR = build

# Busca recursiva por todos os arquivos .cpp dentro de SRC_DIR
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")

# Gera a lista de arquivos objeto, preservando estrutura de diretórios
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Regra padrão
all: $(BUILD_DIR) $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar os arquivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)  # Cria o diretório do objeto, se necessário
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cria o diretório de build, se não existir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Limpa os arquivos gerados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
