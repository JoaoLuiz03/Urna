#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CANDIDATOS 100
#define MAX_ELEITORES 1000

typedef struct {
    int numero;
    char nome[100];
    char partido[50];
} Candidato;

typedef struct {
    char cpf[12];
    char nome[100];
    int votouVereador;
    int votouPrefeito;
} Eleitor;

typedef struct {
    int numero;
    int votos;
} Resultado;

void carregarCandidatos(const char *arquivo, Candidato *candidatos, int *qtd) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        exit(1);
    }
    *qtd = 0;
    while (fscanf(fp, "%d;%[^;];%[^\n]\n", &candidatos[*qtd].numero, candidatos[*qtd].nome, candidatos[*qtd].partido) == 3) {
        (*qtd)++;
    }
    fclose(fp);
}

void carregarEleitores(Eleitor *eleitores, int *qtd) {
    FILE *fp = fopen("eleitores.txt", "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo de eleitores.\n");
        exit(1);
    }
    *qtd = 0;
    while (fscanf(fp, "%11s;%[^\n]\n", eleitores[*qtd].cpf, eleitores[*qtd].nome) == 2) {
        eleitores[*qtd].votouVereador = 0;
        eleitores[*qtd].votouPrefeito = 0;
        (*qtd)++;
    }
    fclose(fp);
}

int buscarEleitor(Eleitor *eleitores, int qtd, const char *cpf) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(eleitores[i].cpf, cpf) == 0) {
            return i;
        }
    }
    return -1;
}

void votar(Eleitor *eleitores, int qtdEleitores, const char *arquivoVotos, const char *cargo) {
    char cpf[12];
    int numero;
    printf("Digite o CPF do eleitor: ");
    scanf("%s", cpf);
    int idx = buscarEleitor(eleitores, qtdEleitores, cpf);
    if (idx == -1) {
        printf("Eleitor não encontrado.\n");
        return;
    }
    if (strcmp(cargo, "Vereador") == 0 && eleitores[idx].votouVereador) {
        printf("Eleitor já votou para Vereador.\n");
        return;
    }
    if (strcmp(cargo, "Prefeito") == 0 && eleitores[idx].votouPrefeito) {
        printf("Eleitor já votou para Prefeito.\n");
        return;
    }
    printf("Digite o número do candidato (0 para branco): ");
    scanf("%d", &numero);
    FILE *fp = fopen(arquivoVotos, "a");
    if (!fp) {
        printf("Erro ao abrir o arquivo de votos.\n");
        exit(1);
    }
    fprintf(fp, "%d\n", numero);
    fclose(fp);
    if (strcmp(cargo, "Vereador") == 0) {
        eleitores[idx].votouVereador = 1;
    } else {
        eleitores[idx].votouPrefeito = 1;
    }
    printf("Voto registrado com sucesso para %s.\n", cargo);
}

void apurar(const char *arquivoCandidatos, const char *arquivoVotos, const char *cargo) {
    Candidato candidatos[MAX_CANDIDATOS];
    int qtdCandidatos = 0;
    carregarCandidatos(arquivoCandidatos, candidatos, &qtdCandidatos);
    Resultado resultados[MAX_CANDIDATOS] = {0};
    for (int i = 0; i < qtdCandidatos; i++) {
        resultados[i].numero = candidatos[i].numero;
        resultados[i].votos = 0;
    }
    FILE *fp = fopen(arquivoVotos, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo de votos.\n");
        exit(1);
    }
    int numero, totalVotos = 0, brancos = 0, nulos = 0;
    while (fscanf(fp, "%d\n", &numero) == 1) {
        totalVotos++;
        if (numero == 0) {
            brancos++;
        } else {
            int encontrado = 0;
            for (int i = 0; i < qtdCandidatos; i++) {
                if (resultados[i].numero == numero) {
                    resultados[i].votos++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                nulos++;
            }
        }
    }
    fclose(fp);
    printf("\nResultado para %s:\n", cargo);
    int maxVotos = 0;
    for (int i = 0; i < qtdCandidatos; i++) {
        if (resultados[i].votos > maxVotos) {
            maxVotos = resultados[i].votos;
        }
    }
    for (int i = 0; i < qtdCandidatos; i++) {
        printf("Candidato %s (%d) - %d votos\n", candidatos[i].nome, candidatos[i].numero, resultados[i].votos);
    }
    printf("Brancos: %d\n", brancos);
    printf("Nulos: %d\n", nulos);
    printf("Total de votos: %d\n", totalVotos);
    printf("Vencedor(es):\n");
    for (int i = 0; i < qtdCandidatos; i++) {
        if (resultados[i].votos == maxVotos) {
            printf("%s (%d) com %d votos\n", candidatos[i].nome, candidatos[i].numero, resultados[i].votos);
        }
    }
}

void limparArquivos() {
    remove("votos_vereador.txt");
    remove("votos_prefeito.txt");
}

int main() {
    Eleitor eleitores[MAX_ELEITORES];
    int qtdEleitores = 0;
    carregarEleitores(eleitores, &qtdEleitores);
    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Votar para Vereador\n");
        printf("2. Votar para Prefeito\n");
        printf("3. Apurar Vereador\n");
        printf("4. Apurar Prefeito\n");
        printf("5. Limpar arquivos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                votar(eleitores, qtdEleitores, "votos_vereador.txt", "Vereador");
                break;
            case 2:
                votar(eleitores, qtdEleitores, "votos_prefeito.txt", "Prefeito");
                break;
            case 3:
                apurar("candidatos_vereador.txt", "votos_vereador.txt", "Vereador");
                break;
            case 4:
                apurar("candidatos_prefeito.txt", "votos_prefeito.txt", "Prefeito");
                break;
            case 5:
                limparArquivos();
                printf("Arquivos limpos com sucesso.\n");
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
    return 0;
}
