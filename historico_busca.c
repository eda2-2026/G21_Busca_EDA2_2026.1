#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 101
#define TAM_STR 100

typedef struct busca{
    char termo[TAM_STR];
    int frequencia;

    struct busca *prox_hash;
    struct busca *prox_hist;

}Busca;

Busca *tabela[TAM];
Busca *historico = NULL;


void inicializar(){

    for(int i=0;i<TAM;i++)
        tabela[i] = NULL;

    historico = NULL;
}


unsigned int funcao_hash(char *str){

    unsigned int h = 0;

    while(*str){
        h = h * 31 + *str;
        str++;
    }

    return h % TAM;
}


void mover_para_inicio_historico(Busca *node){

    if(historico == node)
        return;

    Busca *ant = NULL;
    Busca *atual = historico;

    while(atual && atual != node){

        ant = atual;
        atual = atual->prox_hist;
    }

    if(atual){

        ant->prox_hist = atual->prox_hist;

        atual->prox_hist = historico;
        historico = atual;
    }
}


void registrar_busca(char *termo){

    int indice = funcao_hash(termo);

    Busca *atual = tabela[indice];

    while(atual){

        if(strcmp(atual->termo,termo)==0){

            atual->frequencia++;

            mover_para_inicio_historico(atual);

            printf("Busca atualizada: %s (%d)\n",
                   atual->termo,
                   atual->frequencia);

            return;
        }

        atual = atual->prox_hash;
    }

    Busca *novo = malloc(sizeof(Busca));

    strcpy(novo->termo,termo);
    novo->frequencia = 1;

    novo->prox_hash = tabela[indice];
    tabela[indice] = novo;

    novo->prox_hist = historico;
    historico = novo;

    printf("Nova busca registrada: %s\n", termo);
}


void buscar_termo(char *termo){

    int indice = funcao_hash(termo);

    Busca *atual = tabela[indice];

    while(atual){

        if(strcmp(atual->termo,termo)==0){

            mover_para_inicio_historico(atual);

            printf("Encontrado: %s (%d vezes)\n",
                   atual->termo,
                   atual->frequencia);

            return;
        }

        atual = atual->prox_hash;
    }

    printf("Termo nao encontrado\n");
}


void mostrar_historico(){

    printf("\n===== HISTORICO DE BUSCA =====\n");

    Busca *aux = historico;

    while(aux){

        printf("%s (%d)\n",
               aux->termo,
               aux->frequencia);

        aux = aux->prox_hist;
    }
}

void mostrar_hash(){

    printf("\n===== TABELA HASH =====\n");

    for(int i=0;i<TAM;i++){

        Busca *aux = tabela[i];

        if(aux){

            printf("[%d] -> ",i);

            while(aux){

                printf("%s(%d) -> ",
                       aux->termo,
                       aux->frequencia);

                aux = aux->prox_hash;
            }

            printf("NULL\n");
        }
    }
}


void apagar_historico(){

    for(int i=0;i<TAM;i++){

        Busca *atual = tabela[i];

        while(atual){

            Busca *temp = atual;
            atual = atual->prox_hash;

            free(temp);
        }

        tabela[i] = NULL;
    }

    historico = NULL;

    printf("Historico apagado com sucesso!\n");
}

void carregar_arquivo(){

    FILE *f = fopen("dados_busca.txt","r");

    if(!f){

        printf("Erro ao abrir arquivo\n");
        return;
    }

    char termo[TAM_STR];

    while(fgets(termo,TAM_STR,f)){

        termo[strcspn(termo,"\n")] = 0;

        registrar_busca(termo);
    }

    fclose(f);

    printf("Dados carregados!\n");
}


int main(){

    inicializar();

    int opcao;
    char termo[TAM_STR];

    while(1){

        printf("\n===== MENU =====\n");
        printf("1 - Registrar busca\n");
        printf("2 - Buscar termo\n");
        printf("3 - Mostrar historico\n");
        printf("4 - Mostrar tabela hash\n");
        printf("5 - Carregar dados do arquivo\n");
        printf("6 - Apagar historico\n");
        printf("7 - Sair\n");

        scanf("%d",&opcao);
        getchar();

        if(opcao == 1){

            printf("Digite o termo: ");
            fgets(termo,TAM_STR,stdin);

            termo[strcspn(termo,"\n")] = 0;

            registrar_busca(termo);
        }

        else if(opcao == 2){

            printf("Digite o termo: ");
            fgets(termo,TAM_STR,stdin);

            termo[strcspn(termo,"\n")] = 0;

            buscar_termo(termo);
        }

        else if(opcao == 3)
            mostrar_historico();

        else if(opcao == 4)
            mostrar_hash();

        else if(opcao == 5)
            carregar_arquivo();

        else if(opcao == 6)
            apagar_historico();

        else if(opcao == 7)
            break;
    }

    return 0;
}