# Nome do executável
TARGET = compilador

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Diretórios de código-fonte e binário
SRC_DIR = src
BUILD_DIR = build

# Arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Arquivos objeto (convertendo .cpp para .o)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Regra padrão
all: $(BUILD_DIR) $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar os arquivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cria o diretório de build, se não existir
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Limpa os arquivos gerados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
