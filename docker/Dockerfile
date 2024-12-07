FROM ubuntu:latest

# Atualiza o sistema e instala as dependências necessárias
RUN apt-get update && apt-get install -y \
    build-essential \       
    nasm \                 
    python3 \               
    python3-pip \           
    gdb \                  
    vim \                   
    wget \                  
    git \                  
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Define o diretório de trabalho
WORKDIR /compiler

COPY . /compiler

# Define o entrypoint como o interpretador bash para interatividade
CMD ["/bin/bash"]