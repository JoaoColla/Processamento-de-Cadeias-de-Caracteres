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
    int pos_inserir = indice->qnt;

    for (int i=0; i < indice->qnt; i++){

        int cmp = strcmp(indice->vocabulario[i].palavra, palavra);
        
        if (cmp == 0){
            int qnt_pos = indice->vocabulario[i].qnt;
            indice->vocabulario[i].pos = realloc(indice->vocabulario[i].pos, (qnt_pos+1)*sizeof(int));

            indice->vocabulario[i].pos[qnt_pos] = inicio;
            indice->vocabulario[i].qnt += 1;
            return;
        }

        if (cmp > 0){
            pos_inserir = i;
            break;
        }
    }

    indice->vocabulario = realloc(indice->vocabulario, (indice->qnt +1)*sizeof(Token));

    for (int i = indice->qnt; i>pos_inserir; i--){
        indice->vocabulario[i] = indice->vocabulario[i-1];
    }

    strcpy(indice->vocabulario[pos_inserir].palavra, palavra);
    indice->vocabulario[pos_inserir].pos = malloc(sizeof(int));
    indice->vocabulario[pos_inserir].pos[0] = inicio;
    indice->vocabulario[pos_inserir].qnt = 1;

    indice->qnt += 1;
}

void LerArq(IndInvert *indice){
    char caminho[300];
    printf("Digite o caminho do arquivo:\n");
    scanf(" %[^\n]", caminho);

    FILE *arq = fopen(caminho, "r");

    if (arq == NULL){
        printf("Erro abrindo o arquivo.");
        return;
    }
    if (indice->txt_orig != NULL){
        free(indice->txt_orig);
        indice->txt_orig = NULL;
    }

    if (indice->vocabulario != NULL){
        for (int i = 0; i < indice->qnt; i++){
            free(indice->vocabulario[i].pos);
        }
        free(indice->vocabulario); 
        indice->vocabulario = NULL;
        indice->qnt = 0;
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
    int erro = 0;

    for (int i =0; i <= tamanho; i++){
        unsigned char c = (unsigned char)indice->txt_orig[i];

        if (isalnum(c)){
            if (pos<49){
                palavra[pos] = tolower(c);
                pos++;
            }
            else if(erro == 0){
                printf("Palavra muito grande na posicao %d, truncando para 50 caracteres.\n", i - pos);
                palavra[49] = '\0';
                erro = 1;
            }
        }
        else if (pos>0){
            palavra[pos] = '\0';
            int ini_palavra = i - pos;
            InserirToken(indice, palavra, ini_palavra);
            pos = 0;
            erro = 0;
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

int idx_palavra = -1;
int ocorr_atual = 0;

int main(){
    int opc_menu = 0;
    IndInvert indice = {NULL, NULL, 0, 0};

    do{
        printf(
        "\n Escolha uma opcao pelo numero:\n"
        "[ 1 ] - Ler e processar arquivo texto\n"
        "[ 2 ] - Apresentar o arquivo invertido\n"
        "[ 3 ] - Procurar uma palavra\n"
        "[ 4 ] - Proxima ocorrencia da busca\n"
        "[ 5 ] - Sair do sistema\n\n");

        scanf("%d", &opc_menu);
        if (opc_menu == 1){
            LerArq(&indice);
            idx_palavra = -1;

        }
        else if (opc_menu == 2){
            exibirIndice(&indice);
        }
        else if (opc_menu == 3){
            int opc_menu2 = 0;
            int comp = 0; 

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
            
            char palavra[100];
            printf("Digite a palavra que deseja buscar: ");
            scanf("%s", palavra);
            for (int i = 0; palavra[i] != '\0'; i++){
                palavra[i] = tolower((unsigned char)palavra[i]);
            }

            if(opc_menu2 == 1){
                int baixo=0, alto = indice.qnt-1, achou=-1;

                while (baixo <= alto){
                    int meio = baixo + (alto-baixo)/2;
                    int cmp = strcmp(palavra, indice.vocabulario[meio].palavra);


                    comp++;
                    if (cmp == 0){
                        achou = indice.vocabulario[meio].pos[0];
                        idx_palavra = meio;
                        ocorr_atual = 0;

                        int ini_contex = achou-40;
                        if(ini_contex < 0){
                            ini_contex = 0;
                        }

                        printf("\nPalavra encontrada!\n"
                                "Palavra: %s\nPosicao: %d\nOcorrencias: %d\nComparacoes: %d\n", palavra, achou,indice.vocabulario[meio].qnt, comp);

                        printf("Contexto: %.100s\n\n", &indice.txt_orig[ini_contex]);

                        break;
                    }

                    if (cmp>0){
                        baixo = meio+1;
                    }else if(cmp<0){
                        alto = meio-1;
                    }
                }

                if (achou == -1){
                    printf("Sinto muito, a palavra nao foi encontrada.\n");
                    idx_palavra = -1;
                }
            }
            else if(opc_menu2 == 2){
                int achou = -1;

                for (int i=0; i<indice.qnt; i++){
                    comp++;
                    int cmp = strcmp(palavra, indice.vocabulario[i].palavra);

                    if (cmp == 0){
                        achou = indice.vocabulario[i].pos[0];
                        idx_palavra = i;
                        ocorr_atual = 0;

                        int ini_contex = achou-40;
                        if(ini_contex < 0){
                            ini_contex = 0;
                        }

                        printf("\nPalavra encontrada!\n"
                                "Palavra: %s\nPosicao: %d\nOcorrencias: %d\nComparacoes: %d\n", palavra, achou,indice.vocabulario[i].qnt, comp);

                        printf("Contexto: %.100s\n\n", &indice.txt_orig[ini_contex]);

                        break;
                    }
                }

                if (achou == -1){
                    printf("Sinto muito, a palavra nao foi encontrada.\n");
                    idx_palavra = -1;
                }

            }
        }
        else if (opc_menu == 4){
            if (idx_palavra == -1){
                printf("Nenhuma palavra foi buscada ainda, utilize a opcao [ 3 ] antes.");
            }
            else if (ocorr_atual + 1 < indice.vocabulario[idx_palavra].qnt){
                ocorr_atual++;

                int posicao = indice.vocabulario[idx_palavra].pos[ocorr_atual];
                int ini_contex = posicao - 40;
                if (ini_contex < 0){
                    ini_contex = 0;
                }

                printf("Ocorrencia %d de %d da palavra %s, na posicao %d\nContexto: %.100s\n\n", ocorr_atual+1, indice.vocabulario[idx_palavra].qnt, indice.vocabulario[idx_palavra].palavra, indice.vocabulario[idx_palavra].pos[ocorr_atual],&indice.txt_orig[ini_contex]);
            }else{
                printf("\n\nTodas as ocorrencias da palavra -> %s <- ja foram visitadas\n\n", indice.vocabulario[idx_palavra].palavra);
            }
        }
    }while(opc_menu != 5);

    if (indice.txt_orig != NULL){
    free(indice.txt_orig);
    }
    if (indice.vocabulario != NULL){
        for (int i = 0; i < indice.qnt; i++){
            free(indice.vocabulario[i].pos);
        }
        free(indice.vocabulario);
    }
}