#include <iostream>
#include <string.h>
#include <stdio.h>

struct team {
    char nome[50];
    int anoFundacao;
    char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void excluir(FILE *);

int main() {
    int op;
    FILE *arq;

    if ((arq = fopen("times.dat", "rb+")) == NULL) {
        if ((arq = fopen("times.dat", "wb+")) == NULL) {
            printf("Falha ao abrir o arquivo!\n");
            return 1;
        }
    }

    do {
        printf("\n======= GESTAO DE TIMES ======= \n");
        printf("1. Cadastrar time\n");
        printf("2. Consultar time por código\n");
        printf("3. Excluir time\n");
        printf("4. Sair\n");
        printf("===========Times cadastrados: %d\n", tamanho(arq));
        printf("Opção: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                cadastrar(arq);
                break;
            case 2:
                consultar(arq);
                break;
            case 3:
                excluir(arq);
                break;
            case 4:
                fclose(arq);
                break;
        }
    } while (op != 4);

    return 0;
}

void cadastrar(FILE *arq) {
    team novoTime;
    char confirma;

    novoTime.status = ' ';

    printf("Cadastrando novo time:\n");
    printf("Nome do time: ");
    getchar();
    gets(novoTime.nome);
    printf("Ano de fundação: ");
    scanf("%d", &novoTime.anoFundacao);
    printf("\nConfirma cadastro? <s/n>: ");
    getchar();
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S') {
        fseek(arq, 0, SEEK_END);
        fwrite(&novoTime, sizeof(team), 1, arq);
        printf("\nTime cadastrado com sucesso!\n");
    } else {
        printf("\nCadastro cancelado.\n");
    }
}

void consultar(FILE *arq) {
    team consultTime;
    int cod;

    printf("\nConsultar time por código\n");
    printf("Informe o código do time: ");
    scanf("%d", &cod);

    if ((cod <= tamanho(arq)) && (cod > 0)) {
        fseek(arq, (cod - 1) * sizeof(team), SEEK_SET);
        fread(&consultTime, sizeof(team), 1, arq);

        if (consultTime.status == ' ') {
            printf("\nNome do time: %s\n", consultTime.nome);
            printf("Ano de fundação: %d\n", consultTime.anoFundacao);
        } else {
            printf("\nTime não existe ou foi excluído!\n");
        }
    } else {
        printf("\nCódigo de time inválido!\n");
    }
}

void excluir(FILE *arq) {
    team delTime;
    char confirma;
    int cod;

    printf("\nExcluir time\n");
    printf("Informe o código do time a ser excluído: ");
    scanf("%d", &cod);

    if ((cod <= tamanho(arq)) && (cod > 0)) {
        fseek(arq, (cod - 1) * sizeof(team), SEEK_SET);
        fread(&delTime, sizeof(team), 1, arq);

        if (delTime.status == ' ') {
            printf("\nNome do time: %s\n", delTime.nome);
            printf("Ano de fundação: %d\n", delTime.anoFundacao);
            printf("\nConfirma exclusão? <s/n>: ");
            getchar();
            scanf("%c", &confirma);

            if (toupper(confirma) == 'S') {
                fseek(arq, (cod - 1) * sizeof(team), SEEK_SET);
                delTime.status = '*';
                fwrite(&delTime, sizeof(team), 1, arq);
                printf("\nTime excluído com sucesso!\n");
            }
        } else {
            printf("Time inexistente!\n");
        }
    } else {
        printf("\nCódigo de time inválido!\n");
    }
}

int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(team);
}

