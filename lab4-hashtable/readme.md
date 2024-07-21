# Lab 4 - HashTable

## Função de hash e resolução de conflitos

### Função `insertHash()`
Para calcular o hash, utilizamos o método mais simples, que consiste em calcular o módulo do Id do jogador pelo tamanho da tabela hash definido:

```cpp
int key = auxPlayer.sofifa_id % sizeHash;
hashtable[key].push_back(auxPlayer);
```

### Resolução de conflitos

Como definimos a HashTable como um vetor de vetores de jogadores (`vector<vector<Player>> hashtable(sizeHash);`), a resolução de conflitos consistiu em percorrer o vetor interno até encontrar o Id desejado. Para encontrar a posição no vetor externo é simplesmente calculado o hash do jogador e é feito um acesso direto na posição do vetor retornada pelo cálculo.

```cpp
while ((i < hashtable[key].size()) && hashtable[key][i].sofifa_id != fifa_id) i++;
```

Caso o loop acima percorra todo o vetor da posição escolhida e encontre o jogador desejado (isso é checado pela comparação abaixo):

```cpp
if (hashtable[key][i].sofifa_id == fifa_id) {...}
```

então, a função de busca retorna as informações do jogador. Senão, uma instancia de jogador com o Id passado na busca, o nome "NAO ENCONTRADO" e uma string vazia como posições é retornada.