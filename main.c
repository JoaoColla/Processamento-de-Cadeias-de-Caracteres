# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

typedef struct Token{

    char palavra[50];
    int *pos;
    int qnt;
    int tam;

} Token;

typedef struct IndInvert{
    
    Token *vocabulario;
    char *txt_orig;
    int qnt;
    int tam;
} IndInvert;

void InserirToken(IndInvert *indice, char *palavra, int inicio){
    for (int i=0; i < indice->qnt; i++){
        if (strcmp(indice->vocabulario[i].palavra, palavra) == 0){
            int qnt_pos = indice->vocabulario[i].qnt;
            indice->vocabulario[i].pos = realloc(indice->vocabulario[i].pos, (qnt_pos+1)*sizeof(int));

            indice->vocabulario[i].pos[qnt_pos] = inicio;
            indice->vocabulario[i].qnt += 1;
            return;
        }
    }

    int fim = indice->qnt;
    indice->vocabulario = realloc(indice->vocabulario, (indice->qnt +1)*sizeof(Token));
    strcpy(indice->vocabulario[fim].palavra, palavra);
    indice->vocabulario[fim].pos = malloc(sizeof(int));
    indice->vocabulario[fim].pos[0] = inicio;
    indice->vocabulario[fim].qnt = 1;

    indice->qnt += 1;
}

void LerArq(IndInvert *indice){
    char caminho[100];
    printf("Digite o caminho do arquivo:\n");
    scanf("%s", caminho);

    FILE *arq = fopen(caminho, "r");

    if (arq == NULL){
        printf("Erro abrindo o arquivo.");
        return;
    }
    if (indice->txt_orig != NULL){
        free(indice->txt_orig);
        indice->txt_orig = NULL;
    }

    char linha[1024];
    int tam_txt = 0;

    indice->txt_orig = malloc(1);
    indice->txt_orig[0] = '\0';

    while (fgets(linha, sizeof(linha), arq) != NULL){
        tam_txt += strlen(linha);
        char *temp = realloc(indice->txt_orig, tam_txt + 1);
        if (temp != NULL){
            indice->txt_orig = temp;
            strcat(indice->txt_orig, linha);
        }
    }
    fclose(arq);

    char palavra[50];
    int pos =0;
    int tamanho = strlen(indice->txt_orig);

    for (int i =0; i <= tamanho; i++){
        unsigned char c = (unsigned char)indice->txt_orig[i];

        if (isalnum(c)){
            if (pos<49){
                palavra[pos] = tolower(c);
                pos++;
            }
        }
        else if (pos>0){
            palavra[pos] = '\0';
            int ini_palavra = i - pos;
            InserirToken(indice, palavra, ini_palavra);
            pos = 0;
        }
    }
}


void exibirIndice(IndInvert *indice){
    for(int i = 0; i < indice->qnt; i++){
        printf("%s (%d) ", indice->vocabulario[i].palavra, indice->vocabulario[i].qnt);

        for (int j=0; j < indice->vocabulario[i].qnt; j++){
            printf("%d ", indice->vocabulario[i].pos[j]);
        }
        printf("\n");
    }
}

int main(){
    int opc_menu = 0;
    IndInvert indice = {NULL, NULL, 0, 0};

    do{
        printf(
        "\n Escolha uma opcao pelo numero:\n"
        "[ 1 ] - Ler e processar arquivo texto\n"
        "[ 2 ] - Apresentar o arquivo invertido\n"
        "[ 3 ] - Procurar uma palavra\n"
        "[ 4 ] - Proximma ocorrencia da busca\n"
        "[ 5 ] - Sair do sistema\n\n");

        scanf("%d", &opc_menu);
        if (opc_menu == 1){

            LerArq(&indice);

        }
        else if (opc_menu == 2){
            exibirIndice(&indice);
        }
        else if (opc_menu == 3){
            int opc_menu2 = 0;

            printf(
            "\n Escolha uma opcao pelo numero:\n"
            "[ 1 ] - Busca binaria\n"
            "[ 2 ] - Busca sequencial\n"
            "[ 3 ] - Retornar\n\n");

            scanf("%d", &opc_menu2);


            if (opc_menu2 == 3){
                opc_menu = 0;
                continue;
            }
            else if(opc_menu2 == 1){

            }
            else if(opc_menu2 == 2){

            }
        }
        else if (opc_menu == 4){

        }
    }while(opc_menu != 5);
}