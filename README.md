# Trabalho 2 - Controle de Vendas #
Disciplina: Algoritmos e Técnicas de Programação II

---

### Descrição: ###
O proprietário de uma cantina necessita de um sistema que o ajude a controlar a venda de seus produtos. Ao executar o sistema pela primeira vez, o usuário deverá informar o estoque de cada produto e o seu valor. O sistema deve exibir o seguinte menu:    

1 - Vender  
2 - Cadastrar novo produto  
3 - Remover produto do estoque  
4 - Consultar estoque  
5 - Consultar lucro  
6 - Repor estoque  
0 - Sair

---

### Regras ###

Os itens do menu devem seguir as seguintes regras:  

##### 1 - Vender: #####
Em Venda, o cliente deve escolher qual produto irá comprar pelo **NOME**. O sistema irá exibir o valor do produto e solicitará que o usuário informe a quantidade que deseja comprar. O sistema irá verificar se o estoque atual atende àquela venda e, caso atenda, mostrar o valor total da compra e baixar o estoque do produto, caso contrário, exibe a mensagem _“Não há estoque disponível para atender a venda”_.

#### 2 - Cadastrar novo produto ####
Possibilite que o usuário entre com um novo produto em estoque. Os campos que o usuário deve cadastrar são: 
1. Nome do Produto; 
2. Valor de compra; 
3. Valor de Venda; 
4. Quantidade em estoque; 
5. Quantidade vendida. 

Como teste inicial, Considere os seguintes produtos:

    Id - Nome

    01 - suco  
    02 - refrigerante  
    03 - salgadinho frito  
    04 - salgadinho assado  
    05 - doce   

Sinta-se à vontade para adicionar qualquer valor aos outros campos. A chave primária será o NOME DO PRODUTO (string de até 50 caracteres). Salve a chave primária em um arquivo binário (tabela) separado, chamado *chaves.bin*, e o restante das informações em um arquivo binário (tabela) chamado *infs.bin*. Cada linha (registro) em *chaves.bin* terá o nome do produto e um inteiro informando a linha onde o restante das informações do respectivo produto está em *infs.bin*. Mantenha os registros de chaves.bin ORDENADOS pelo nome do produto (ordem crescente). Utilize o Quick Sort para isso. Quando um registro novo for inserido, você terá que inserir a chave primária no arquivo *chaves.bin* e o restante das informações em *infs.bin*. Para isso, antes de inserir, verifique se existe algum registro excluído no arquivo *infs.bin* para re-aproveitar o espaço. Veja o *item 3* para mais detalhes.

Repare que os arquivos são binários, então você utilizará ESTRUTURAS como abstração para inserir e recuperar os itens. Considere registros de tamanho fixo, ou seja, limite o tamanho de cada campo, por ex, 50 caracteres para o nome, 4 bytes para o código do produto, etc. Dessa maneira, registros previamente deletados poderão ser reaproveitados sem problemas em relação a diferenças no tamanho.

#### 3 - Remover produto do estoque  ####
Para remover um produto do estoque, remova a chave do arquivo *chaves.bin* e reordene o arquivo inteiro. Entretanto, não remova o registro de *infs.bin*, simplesmente adicione um campo binário especial em cada registro chamado deletado, e, quando um registro for deletado, modifique o valor do campo para TRUE. Quando você for inserir um novo produto, NÃO INSIRA O REGISTRO NO FINAL DO ARQUIVO, percorra o arquivo para procurar registros previamente deletados, e escreva por cima.

#### 4 - Consultar estoque ####
O sistema irá mostrar cada produto, a sua quantidade em estoque, o valor total de cada produto em estoque e o valor total do estoque. Os produtos devem ser exibidos em ordem alfabética (repare que *chaves.bin* já está em ordem alfabética).

#### 5 - Consultar lucro ####
O sistema mostrará o quanto (em R$) foi vendido de cada produto.

#### 6 - Repor estoque  #####
O usuário deverá escolher qual produto irá repor informar a quantidade a ser reposta e então mostrar o estoque novo do produto. Não devem ser aceitas quantidades menores ou iguais à zero. UTILIZE BUSCA BINÁRIA PARA ENCONTRAR O REGISTRO EM *chaves.bin*

#### 0 - Sair ####
Encerra a aplicação e mostra o total de vendas efetuadas, a quantidade vendida de cada produto, o quanto ficou de cada produto no estoque, seu valor e o seu valor total em estoque e o lucro total do dia.