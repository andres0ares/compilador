# Para rodar 
### O Docker precisa está rodando

No terminal:

cd docker  
docker-compose run --rm --build compilador bash

# No container

## cria o compilador

make

# Atividade 5

## executa a analise sintatica

./compilador ./input/atv5/i1.ci

# Outras atividades

## Atividade 2 

### executa o compilador

./compilador ./input/entrada_default.ci

### compila o codigo assembly resultante

as --64 -o ./output/compilado.o ./output/modelo.s  
ld -o compilado ./output/compilado.o

### executa linguagem compilada

./compilado

## Atividade 3 

### executa o compilador

./compilador ./input/atv3/Parte1_1.txt
./compilador ./input/atv3/Parte1_2.txt
./compilador ./input/atv3/Parte1_3.txt
./compilador ./input/atv3/Parte2.txt
./compilador ./input/atv3/Parte3.txt

## Atividade 4

### executa a analise lexica

// com erro:

./compilador ./input/atv4/i1.ci

// sem erro:

./compilador ./input/atv4/i2.ci


