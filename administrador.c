#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int Exit = 0;
char cpf_online[20];

// Função para verificar se o CPF contém apenas números
int VarAnalisarCPF(const char *cpf) {
    for (int i = 0; i < strlen(cpf); i++) {
        if (!isdigit(cpf[i])) {
            return 0;
        }
    }
    return 1;
}

// Função para verificar se o CPF e a senha estão corretos
int AnalisarLogin(const char *cpf, const char *senha) {
    FILE *arquivo = fopen("administradores.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }

    char linha[100], cpfArquivo[20], senhaArquivo[50];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "CPF: %s SENHA: %s ", cpfArquivo, senhaArquivo);
        if (strcmp(cpf, cpfArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

// Função para verificar se o CPF já está cadastrado
int VarVerCpfJaCadastrado(const char *cpf) {
    FILE *arquivo = fopen("administradores.txt", "r");
    if (arquivo == NULL) {
        return 0;
    }

    char linha[100], cpfArquivo[20];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "CPF: %s", cpfArquivo);
        if (strcmp(cpf, cpfArquivo) == 0) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

// Função para cadastrar um novo CPF e senha
void CadNovADM(const char *cpf, const char *senha) {
    FILE *arquivo = fopen("administradores.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fprintf(arquivo,
            "CPF: %s\tSENHA: %s\tREAL: %.2f\tBITCOIN: %.6f\tETHEREUM: "
            "%.6f\tRIPPLE: %.6f\n",
            cpf, senha, 0.0, 0.0, 0.0, 0.0);  // Valores iniciais das criptos
    fclose(arquivo);
}

// Função principal
int menu() {
    char cpf[20];
    char senha[50];
    int opcao;

    while (!Exit) {
        printf("Bem-vindo ao FEINANCE!\n");
        printf("Digite 1 para logar ou 2 para cadastrar um novo usuario (ou 0 para Exit): ");
        scanf("%d", &opcao);
        getchar();  // Limpa o buffer do enter

        if (opcao == 0) {
            printf("Saindo do sistema.\n");
            Exit = 1;
            break;
        }

        if (opcao == 1) {
            // Login
            while (1) {
                printf("Digite o CPF para login (somente numeros) (ou 0 para cancelar): ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                if (strcmp(cpf, "0") == 0) {
                    printf("Login cancelado.\n");
                    break;
                }

                if (!VarAnalisarCPF(cpf)) {
                    printf("CPF invalido! Digite apenas numeros.\n");
                    continue;
                }

                printf("Digite a senha: ");
                fgets(senha, sizeof(senha), stdin);
                senha[strcspn(senha, "\n")] = '\0';

                if (AnalisarLogin(cpf, senha)) {
                    printf("Login bem-sucedido! Bem-vindo!\n");
                    strcpy(cpf_online, cpf); // Armazena o CPF logado
                    break;
                } else {
                    printf("CPF ou senha incorretos. Tente novamente.\n");
                }
            }
        } 
        else if (opcao == 2) {
            // Cadastro de novo usuário
            while (1) {
                printf("Digite o CPF (somente numeros) do usuario que deseja cadastrar: ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                if (!VarAnalisarCPF(cpf)) {
                    printf("CPF invalido! Digite apenas numeros.\n");
                    continue;
                }

                if (VarVerCpfJaCadastrado(cpf)) {
                    printf("CPF já cadastrado! Tente outro CPF ou faça login.\n");
                } else {
                    printf("Digite a senha: ");
                    fgets(senha, sizeof(senha), stdin);
                    senha[strcspn(senha, "\n")] = '\0';

                    CadNovADM(cpf, senha);
                    printf("Cadastrado com sucesso!\n");
                    printf("Bem-vindo ao FEINANCE!\n");
                    break;
                }
            }
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}
