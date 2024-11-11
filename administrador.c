#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int validarCPF(char cpf[]) {
    for (int i = 0; i < strlen(cpf); i++) {
        if (!isdigit(cpf[i])) {
            return 0;
        }
    }
    return 1;
}

int verificarCadastro(char cpf[], const char* arquivoNome) {
    FILE *arquivo;
    char cpfArquivo[12], senha[50];

    arquivo = fopen(arquivoNome, "r");
    if (arquivo == NULL) {
        return 0;
    }

    while (fscanf(arquivo, "%s %s", cpfArquivo, senha) != EOF) {
        if (strcmp(cpf, cpfArquivo) == 0) {
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

void cadastrarUsuario(const char* arquivoNome) {
    FILE *arquivo;
    char cpf[12], senha[50];

    arquivo = fopen(arquivoNome, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    do {
        printf("Digite o CPF (somente numeros): ");
        scanf("%11s", cpf);

        if (!validarCPF(cpf)) {
            printf("Apenas numeros sao permitidos no CPF.\n");
        } else if (verificarCadastro(cpf, arquivoNome)) {
            printf("CPF ja cadastrado. Tente outro CPF.\n");
        }
    } while (!validarCPF(cpf) || verificarCadastro(cpf, arquivoNome));

    printf("Digite a senha: ");
    scanf("%s", senha);

    fprintf(arquivo, "%s %s\n", cpf, senha);
    printf("Cadastro realizado com sucesso!\n");

    fclose(arquivo);
}

void CadastraInvestidor() {
    FILE *arquivo;
    char cpf[12], senha[50];

    arquivo = fopen("usuarios.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    do {
        printf("Digite o CPF (somente numeros): ");
        scanf("%11s", cpf);

        if (!validarCPF(cpf)) {
            printf("Apenas numeros sao permitidos no CPF.\n");
        } else if (verificarCadastro(cpf, "usuarios.txt")) {
            printf("CPF ja cadastrado. Tente outro CPF.\n");
        }
    } while (!validarCPF(cpf) || verificarCadastro(cpf, "usuarios.txt"));

    printf("Digite a senha: ");
    scanf("%s", senha);

    fprintf(arquivo, "CPF: %s  SENHA: %s\n", cpf, senha);
    printf("Investidor cadastrado com sucesso!\n");

    fclose(arquivo);
}

int login() {
    char cpf[12], senha[50], cpfArquivo[12], senhaArquivo[50];

    while (1) {
        printf("Digite o CPF (ou 0 para sair): ");
        scanf("%11s", cpf);

        if (strcmp(cpf, "0") == 0) {
            printf("Saindo do login...\n");
            return 0;
        }

        if (!validarCPF(cpf)) {
            printf("Apenas numeros sao permitidos no CPF.\n");
            continue;
        }

        if (verificarCadastro(cpf, "administradores.txt") == 0) {
            printf("CPF nao encontrado.\n");
            continue;
        }

        printf("Digite a senha: ");
        scanf("%s", senha);

        FILE *arquivo = fopen("administradores.txt", "r");
        if (arquivo == NULL) {
            printf("Erro ao acessar o arquivo.\n");
            return 0;
        }

        while (fscanf(arquivo, "%s %s", cpfArquivo, senhaArquivo) != EOF) {
            if (strcmp(cpf, cpfArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
                printf("Login bem-sucedido!\n");
                fclose(arquivo);
                return 1;
            }
        }

        printf("CPF ou senha incorretos. Tente novamente ou digite 0 para sair.\n");
        fclose(arquivo);
    }
}

void menuAdministrador() {
    int opcao;

    while (1) {
        printf("\nMenu de Administrador:\n");
        printf("1. Cadastrar Investidor\n");
        printf("2. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            CadastraInvestidor();
        } else if (opcao == 2) {
            printf("Saindo...\n");
            break;
        } else {
            printf("Digite uma opcao valida.\n");
        }
    }
}

int menu() {
    int opcao;

    while (1) {
        printf("Bem-vindo a Central de Administradores do Grupo FEInance\n\n");
        printf("Escolha uma opcao:\n");
        printf("1. Fazer Login\n");
        printf("2. Cadastrar-se\n");
        printf("Digite a opcao (1 ou 2): ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            if (login()) {
                printf("Acesso concedido!\n");
                menuAdministrador();
            } else {
                printf("Falha no login.\n");
            }
            break;
        } else if (opcao == 2) {
            cadastrarUsuario("administradores.txt");
            menuAdministrador();
            break;
        } else {
            printf("Digite uma opcao valida.\n");
        }
    }

    return 0;
}
