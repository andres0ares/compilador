# Para rodar 
### O Docker precisa está rodando

No terminal:

cd docker  
docker-compose run --rm --build compilador bash

# No container

## cria o compilador

make

# Atividade 4

## executa a analise lexica

./compilador ./input/entrada_ec1.ci


# Outras atividades

## Atividade 2 

### executa o compilador

./compilador ./input/entrada_default.ci

### compila o codigo assembly resultante

as --64 -o ./output/compilado.o ./output/modelo.s  
ld -o compilado ./output/compilado.o

### executa linguagem compilada

./compilado



