#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CANDIDATOS 200
#define MAX_ELEITORES 2000

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
        printf("Erro ao abrir %s\n", arquivo);
        exit(1);
    }
    *qtd = 0;
    while (fscanf(fp, "%d;%99[^;];%49[^\n]\n",
                  &candidatos[*qtd].numero,
                  candidatos[*qtd].nome,
                  candidatos[*qtd].partido) == 3) {
        (*qtd)++;
        if (*qtd >= MAX_CANDIDATOS) break;
    }
    fclose(fp);
}

void carregarEleitores(Eleitor *eleitores, int *qtd) {
    FILE *fp = fopen("eleitores.txt", "r");
    if (!fp) {
        printf("Arquivo eleitores.txt não encontrado.\n");
        exit(1);
    }
    *qtd = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), fp)) {

        linha[strcspn(linha, "\r\n")] = 0;
        char *p = linha;
        char *token;
        token = strtok(p, ";");
        if (!token) continue;
        strncpy(eleitores[*qtd].cpf, token, 11);
        eleitores[*qtd].cpf[11] = '\0';
        token = strtok(NULL, ";");
        if (token) strncpy(eleitores[*qtd].nome, token, 99);
        else eleitores[*qtd].nome[0] = '\0';
        token = strtok(NULL, ";");
        if (token) eleitores[*qtd].votouVereador = atoi(token);
        else eleitores[*qtd].votouVereador = 0;
        token = strtok(NULL, ";");
        if (token) eleitores[*qtd].votouPrefeito = atoi(token);
        else eleitores[*qtd].votouPrefeito = 0;
        (*qtd)++;
        if (*qtd >= MAX_ELEITORES) break;
    }
    fclose(fp);
}

void salvarEleitores(Eleitor *eleitores, int qtd) {
    FILE *fp = fopen("eleitores.txt", "w");
    if (!fp) {
        printf("Erro ao abrir eleitores.txt para escrita.\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        fprintf(fp, "%s;%s;%d;%d\n",
                eleitores[i].cpf,
                eleitores[i].nome,
                eleitores[i].votouVereador,
                eleitores[i].votouPrefeito);
    }
    fclose(fp);
}

int buscarEleitor(Eleitor *eleitores, int qtd, const char *cpf) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(eleitores[i].cpf, cpf) == 0) return i;
    }
    return -1;
}

void registrarVotoNoArquivo(const char *arquivoVotos, const char *cpf, int numero) {
    FILE *fp = fopen(arquivoVotos, "a");
    if (!fp) {
        printf("Erro ao abrir %s para gravar voto.\n", arquivoVotos);
        return;
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    fprintf(fp, "%s;%d;%s\n", cpf, numero, buf);
    fclose(fp);
}

void votar(Eleitor *eleitores, int qtdEleitores, const char *arquivoVotos, const char *cargo) {
    char cpf[12];
    int numero;
    printf("Digite o CPF do eleitor (somente dígitos): ");
    if (scanf("%11s", cpf) != 1) {
        printf("Entrada inválida.\n");
        while (getchar() != '\n'); 
        return;
    }
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
    if (scanf("%d", &numero) != 1) {
        printf("Entrada inválida.\n");
        while (getchar() != '\n');
        return;
    }
    registrarVotoNoArquivo(arquivoVotos, cpf, numero);
    if (strcmp(cargo, "Vereador") == 0) eleitores[idx].votouVereador = 1;
    else eleitores[idx].votouPrefeito = 1;
    salvarEleitores(eleitores, qtdEleitores);
    printf("Voto registrado com sucesso para %s.\n", cargo);
}

int compararResultados(const void *a, const void *b) {
    Resultado *ra = (Resultado *)a;
    Resultado *rb = (Resultado *)b;
    return rb->votos - ra->votos; 
}

void apurar(const char *arquivoCandidatos, const char *arquivoVotos, const char *cargo) {
    Candidato candidatos[MAX_CANDIDATOS];
    int qtdCandidatos = 0;
    carregarCandidatos(arquivoCandidatos, candidatos, &qtdCandidatos);
    if (qtdCandidatos == 0) {
        printf("Nenhum candidato carregado para %s.\n", cargo);
        return;
    }
    Resultado resultados[MAX_CANDIDATOS];
    for (int i = 0; i < qtdCandidatos; i++) {
        resultados[i].numero = candidatos[i].numero;
        resultados[i].votos = 0;
    }
    FILE *fp = fopen(arquivoVotos, "r");
    if (!fp) {
        printf("Arquivo de votos (%s) não encontrado. Zero votos.\n", arquivoVotos);
    } else {
        char linha[256];
        int totalVotos = 0, brancos = 0, nulos = 0;
        while (fgets(linha, sizeof(linha), fp)) {
            
            char *p = linha;
            char *cpf = strtok(p, ";");
            char *numStr = strtok(NULL, ";");
            
            if (!numStr) continue;
            int numero = atoi(numStr);
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
                if (!encontrado) nulos++;
            }
        }
        fclose(fp);

        
        qsort(resultados, qtdCandidatos, sizeof(Resultado), compararResultados);

        printf("\nResultado para %s:\n", cargo);
        for (int i = 0; i < qtdCandidatos; i++) {
            
            const char *nome = "Desconhecido";
            for (int j = 0; j < qtdCandidatos; j++) {
                if (candidatos[j].numero == resultados[i].numero) {
                    nome = candidatos[j].nome;
                    break;
                }
            }
            printf("%d votos - Candidato %s (%d)\n", resultados[i].votos, nome, resultados[i].numero);
        }
        printf("Brancos: %d\n", brancos);
        printf("Nulos: %d\n", nulos);
        printf("Total de votos: %d\n", totalVotos);

        if (qtdCandidatos > 0) {
            int maxVotos = resultados[0].votos;
            printf("Vencedor(es) com %d voto(s):\n", maxVotos);
            for (int i = 0; i < qtdCandidatos && resultados[i].votos == maxVotos; i++) {
                const char *nome = "Desconhecido";
                for (int j = 0; j < qtdCandidatos; j++) {
                    if (candidatos[j].numero == resultados[i].numero) {
                        nome = candidatos[j].nome;
                        break;
                    }
                }
                printf("%s (%d)\n", nome, resultados[i].numero);
            }
        }
    }
}

void limparArquivos() {
    remove("votos_vereador.txt");
    remove("votos_prefeito.txt");
    printf("Arquivos de votos removidos.\n");
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
        printf("5. Limpar arquivos de votos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
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
                break;
            case 0:
                printf("Encerrando.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
    return 0;
}
