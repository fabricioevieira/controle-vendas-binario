#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>

#define PRODUCTSIZE 21
#define KEYSIZE 54

typedef struct{
    int line; //ID
    float purchase_price; //Valor de compra
    float sales_price; //Valor de venda
    int storage_count; //Quantidade em estoque
    int sales_count; //Quantidade de vendas
    bool deleted; //Se está deletado
}Product;

typedef struct{
    int product_line; //FK
    char name[50]; //nome
}Key;

int countProducts(FILE* infos, Product product);
int countKeys(FILE* id);
void showMenu();
void sell(FILE* infos, FILE* id, int count_registers, int count_keys);
void insert(FILE* infos, FILE* id, int *count_registers, int *count_keys);
void delete(FILE* infos, FILE* id, int count_registers, int* count_keys);
void calculateProfit(FILE* infos, FILE*id, int count_registers, int count_keys);
void restock(FILE* infos, FILE* id, int count_registers, int count_keys);
void position(FILE *infos, int pos);
void positionID(FILE *id, int pos);
void show(FILE* infos, FILE* id, int count_registers, int count_keys);
bool exists(FILE* id, char *name, int count_registers);
int findPosition(FILE* infos, int count_registers);
void quickSort(int esq, int dir, FILE* id);
void partition(int esq, int dir, int *i, int *j, FILE* id);


int main(){
    int option, count_register = 0, count_keys = 0;
    Product product_infos;
    Key product_key;
    FILE* infos;
    FILE* keys;

    if(access("infos.bin", F_OK) == 0){
        infos = fopen("infos.bin", "r+b");
        count_register = countProducts(infos, product_infos);
    }
    else{
        infos = fopen("infos.bin", "w+b");
    }

    if(access("chaves.bin", F_OK) == 0){
        keys = fopen("chaves.bin", "r+b");
        count_keys = countKeys(keys);
    }
    else{
        keys = fopen("chaves.bin", "w+b");
    }

    do{
        showMenu();
        scanf("%d", &option);

        switch(option){
        case 1:
            sell(infos, keys, count_register, count_keys);
            break;
        case 2:
            insert(infos, keys, &count_register, &count_keys);
            break;
        case 3:
            delete(infos, keys, count_register, &count_keys);
            break;
        case 4:
            show(infos, keys, count_register, count_keys);
            break;
        case 5:
            calculateProfit(infos, keys, count_register, count_keys);
            break;
        case 6:
            restock(infos, keys, count_register, count_keys);
            break;
        case 0:
            printf("Encerrando programa...");
            break;
        default:
            printf("Opcao invalida");
            break;
        }
    }while(option != 0);

    fclose(keys);
    fclose(infos);
}

void showMenu(){
    printf("\n\t        MENU\n-----------------------------------\n");
    printf("[1] - Vender\n");
    printf("[2] - Cadastrar novo produto\n");
    printf("[3] - Remover produto do estoque\n");
    printf("[4] - Consultar estoque\n");
    printf("[5] - Consultar lucro\n");
    printf("[6] - Repor estoque\n");
    printf("[0] - Sair\n");
}

void position(FILE *infos, int pos){
    fseek(infos, (pos)*PRODUCTSIZE, SEEK_SET);
}

void positionID(FILE *id, int pos){
    fseek(id, (pos)*KEYSIZE, SEEK_SET);
}

bool exists(FILE* id, char *name, int count_registers){
    if(searchProduct(id, name, 0, count_registers) != -1)
        return true;
    else{
        return false;
    }
}

int countProducts(FILE* infos, Product product){
    int count;

    fseek(infos, 0, SEEK_END);
    count = ftell(infos);

    count /= PRODUCTSIZE;

    return count;
}

int countKeys(FILE* id){
    int count;

    fseek(id, 0, SEEK_END);
    count = ftell(id);

    count /= KEYSIZE;

    return count;
}

void partition(int esq, int dir, int *i, int *j, FILE* id){
    Key key, x, a, b;
    int meio;
    *i = esq;
    *j = dir;

    positionID(id, ((*i + *j)/2));
    fread(&x, KEYSIZE, 1, id);

    do{
        positionID(id, *i);
        fread(&key, KEYSIZE, 1, id);
        while(strcmp(x.name, key.name) > 0){
            (*i)++;
            positionID(id, *i);
            fread(&key, KEYSIZE, 1, id);
        }
        positionID(id, *j);
        fread(&key, KEYSIZE, 1, id);
        while(strcmp(x.name, key.name) < 0){
            (*j)--;
            positionID(id, *j);
            fread(&key, KEYSIZE, 1, id);    
        }

        if(*i <= *j){
            positionID(id, *i);
            fread(&a, KEYSIZE, 1, id);
            positionID(id, *j);
            fread(&b, KEYSIZE, 1, id);
            
            positionID(id, *j);
            fwrite(&a, KEYSIZE, 1, id);
            positionID(id, *i);
            fwrite(&b, KEYSIZE, 1, id);

            (*i)++;
            (*j)--;
        }
    }while(*i <= *j);
}

void quickSort(int esq, int dir, FILE* id){
    int i, j;
    partition(esq, dir, &i, &j, id);

    if(esq < j)
        quickSort(esq, j, id);
    if(dir > i)
        quickSort(i, dir, id);
}

int searchProduct(FILE *id, char* name, int inf, int sup){
    Key key;

    if (inf <= sup) {
        int meio = (inf + sup) / 2;
	
    int i = ftell(id);
    positionID(id, meio);
    fread(&key, KEYSIZE, 1, id);

    if (strcmp(name, key.name) < 0)
        return searchProduct(id, name, inf, meio - 1);
    else if (strcmp(name, key.name) > 0)
        return searchProduct(id, name, meio + 1, sup);
    else return meio;
   }

   return -1;
}

int findPosition(FILE* infos, int count_registers){
    Product product;

    for(int i = 0; i < count_registers; i++){
        position(infos, i);
        fread(&product, PRODUCTSIZE, 1, infos);

        if(product.deleted)
            return i;
    }
    return count_registers;
}

void insert(FILE* infos, FILE* id, int *count_registers, int *count_keys){
    int pos;
    Key product_key;
    Product product_info;

    fflush(stdin);
    printf("Digite o nome do produto:");
    gets(product_key.name);


    pos = *count_registers;
    if(*count_registers > 0){
        if(exists(id, product_key.name, *count_registers)){
            printf("Produto ja existe na base de dados!\n");
            return;
        }
        pos = findPosition(infos, *count_registers);
    }

    printf("Digite o valor de compra: ");
    scanf("%f", &product_info.purchase_price);
    printf("Digite o valor de venda: ");
    scanf("%f", &product_info.sales_price);
    printf("Digite a quantidade: ");
    scanf("%d", &product_info.storage_count);

    product_info.deleted = false;
    product_info.line = pos;
    product_key.product_line = pos;
    product_info.sales_count = 0;

    position(infos, pos);
    fseek(id, 0, SEEK_END);

    if(fwrite(&product_info, PRODUCTSIZE, 1, infos)){
        fwrite(&product_key, KEYSIZE, 1, id);
        quickSort(0, *count_keys, id);
        (*count_keys)++;
        printf("\n\nProduto adicionado com sucesso!\n\n");
    }
    else{
        printf("\n\nErro ao adicionar produto. Tente novamente.\n\n");
    }

    if(pos >= *count_registers)
        *count_registers = *count_registers + 1;
}

void show(FILE* infos, FILE* id, int count_registers, int count_keys){
    Product product_infos;
    Key product_key;
    int count = 0, line;
    float total = 0;

    positionID(id, count);
    printf("\n");
    for(int i = 0; i < count_keys; i++){
        positionID(id, i);
        fread(&product_key, KEYSIZE, 1, id);
        if(product_key.product_line >= 0){
            printf("Produto = %s\n", product_key.name);
            line = product_key.product_line;

            position(infos, line);
            fread(&product_infos, PRODUCTSIZE, 1, infos);
            printf("Quantidade em estoque = %d\n", product_infos.storage_count);
            printf("Valor em estoque = R$%.2f\n", (product_infos.sales_price * product_infos.storage_count));
            total += (product_infos.sales_price * product_infos.storage_count);
        }
    }

    printf("\n\nValor total em estoque = R$%.2f\n", total);
}

void delete(FILE* infos, FILE* id, int count_registers, int* count_keys){
    Key product_key;
    Product product;
    int pos, line;
    char c[] = {127,254,'\0'};

    fflush(stdin);
    printf("Digite o nome do produto:");
    gets(product_key.name);

    if(count_registers <= 0){
        printf("Nao ha produtos na base de dados!");
        return;
    }
    else{
        if(!exists(id, product_key.name, count_registers)){
            printf("Produto inexistente na base de dados!\n");
            return;
        }
    }

    pos = searchProduct(id, product_key.name, 0, *count_keys);

    positionID(id, pos);
    fread(&product_key, KEYSIZE, 1, id);
    line = product_key.product_line;

    product_key.product_line = -1;
    strcpy(product_key.name, c);
    positionID(id, pos);
    fwrite(&product_key, KEYSIZE, 1, id);


    position(infos, line);
    fread(&product, PRODUCTSIZE, 1, infos);
    product.deleted = true;

    position(infos, line);
    fwrite(&product, PRODUCTSIZE, 1, infos);

    (*count_keys)--;
    quickSort(0, *count_keys, id);

    printf("\n\nProduto removido com sucesso!\n\n");
}

void sell(FILE* infos, FILE* id, int count_registers, int count_keys){
    Key product_key;
    Product product;
    int pos, line, qtde;

    fflush(stdin);
    printf("Digite o nome do produto:");
    gets(product_key.name);

    if(count_registers <= 0){
        printf("Nao ha produtos na base de dados!");
        return;
    }
    else{
        if(!exists(id, product_key.name, count_registers)){
            printf("Produto inexistente na base de dados!\n");
            return;
        }
    }
    pos = searchProduct(id, product_key.name, 0, count_keys);

    positionID(id, pos);
    fread(&product_key, KEYSIZE, 1, id);
    line = product_key.product_line;

    position(infos, line);
    fread(&product, PRODUCTSIZE, 1, infos);

    printf("\nPreco: R$%.2f\n", product.sales_price);
    printf("\nInforme a quantidade que deseja comprar: ");
    scanf("%d", &qtde);

    if(qtde > product.storage_count){
        printf("\nNao ha estoque disponivel para atender a venda");
        return;
    }

    product.storage_count -= qtde;
    product.sales_count += qtde;

    position(infos, line);
    fwrite(&product, PRODUCTSIZE, 1, infos);
    printf("\nValor total da compra: R$%.2f", qtde*product.sales_price);
    printf("\nCompra efetuada com sucesso!\n");
}

void restock(FILE* infos, FILE* id, int count_registers, int count_keys){
    Key product_key;
    Product product;
    int pos, line, qtde;

    fflush(stdin);
    printf("Digite o nome do produto:");
    gets(product_key.name);

    if(count_registers <= 0){
        printf("Nao ha produtos na base de dados!");
        return;
    }
    else{
        if(!exists(id, product_key.name, count_registers)){
            printf("Produto inexistente na base de dados!\n");
            return;
        }
    }
    pos = searchProduct(id, product_key.name, 0, count_keys);

    positionID(id, pos);
    fread(&product_key, KEYSIZE, 1, id);
    line = product_key.product_line;

    do{
    printf("\nInforme a quantidade a ser reposta: ");
    scanf("%d", &qtde);
    }while(qtde <= 0);

    position(infos, line);
    fread(&product, PRODUCTSIZE, 1, infos);

    product.storage_count += qtde;

    position(infos, line);
    fwrite(&product, PRODUCTSIZE, 1, infos);

    printf("Estoque atualizado com sucesso!\nNova quantidade em estoque: %d", product.storage_count);
}

void calculateProfit(FILE* infos, FILE*id, int count_registers, int count_keys){
    Product product_infos;
    Key product_key;
    int count = 0, line;
    float total = 0, lucro = 0;

    positionID(id, count);
    for(int i = 0; i < count_keys; i++){
        positionID(id, i);
        fread(&product_key, KEYSIZE, 1, id);
        if(product_key.product_line >= 0){
            printf("Produto = %s\n", product_key.name);
            line = product_key.product_line;

            position(infos, line);
            fread(&product_infos, PRODUCTSIZE, 1, infos);
            lucro = product_infos.sales_price - product_infos.purchase_price;
            printf("Lucro = R$%.2f\n", lucro*product_infos.sales_count);

            total += lucro * product_infos.sales_count;
        }
    }

    printf("\n\nLucro total = R$%.2f\n", total);
}