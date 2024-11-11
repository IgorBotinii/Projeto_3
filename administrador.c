#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função para verificar se o CPF contém apenas números
int validarCPF(char cpf[]) {
    for (int i = 0; i < strlen(cpf); i++) {
        if (!isdigit(cpf[i])) {
            return 0; // Se encontrar um caractere não numérico, retorna falso
        }
    }
    return 1; // Se todos os caracteres forem numéricos, retorna verdadeiro
}

// Função para verificar se o CPF já está cadastrado
int verificarCadastro(char cpf[]) {
    FILE *arquivo;
    char cpfArquivo[12], senha[50];

    arquivo = fopen("administradores.txt", "r");
    if (arquivo == NULL) {
        return 0; // Se o arquivo não existir ou não puder ser aberto, considera como não cadastrado
    }

    // Percorre o arquivo verificando se o CPF já está cadastrado
    while (fscanf(arquivo, "%s %s", cpfArquivo, senha) != EOF) {
        if (strcmp(cpf, cpfArquivo) == 0) {
            fclose(arquivo);
            return 1; // CPF já cadastrado
        }
    }

    fclose(arquivo);
    return 0; // CPF não encontrado
}

// Função para login
int login() {
    char cpf[12], senha[50], cpfArquivo[12], senhaArquivo[50];

    while (1) {
        printf("Digite o CPF (ou 0 para sair): ");
        scanf("%11s", cpf);  // Limita a leitura a 11 caracteres

        // Se o usuário digitar "0", sai do login
        if (strcmp(cpf, "0") == 0) {
            printf("Saindo do login...\n");
            return 0;
        }

        // Verifica se o CPF é válido
        if (!validarCPF(cpf)) {
            printf("Apenas números são permitidos no CPF.\n");
            continue; // Pede para digitar novamente
        }

        // Verifica se o CPF está cadastrado
        if (verificarCadastro(cpf) == 0) {
            printf("CPF não encontrado.\n");
            continue; // Pede para digitar novamente
        }

        // Solicita a senha
        printf("Digite a senha: ");
        scanf("%s", senha);

        FILE *arquivo = fopen("administradores.txt", "r");
        if (arquivo == NULL) {
            printf("Erro ao acessar o arquivo.\n");
            return 0;
        }

        // Verifica se a combinação CPF e senha existe
        while (fscanf(arquivo, "%s %s", cpfArquivo, senhaArquivo) != EOF) {
            if (strcmp(cpf, cpfArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
                printf("Login bem-sucedido!\n");
                fclose(arquivo);
                return 1; // Login realizado com sucesso
            }
        }

        printf("CPF ou senha incorretos. Tente novamente ou digite 0 para sair.\n");
        fclose(arquivo);
    }
}

int main() {
    int opcao;

    // Loop até o usuário escolher uma opção válida
    while (1) {
        printf("Bem-vindo à Central de Administradores do Grupo FEInance\n\n");
        printf("Escolha uma opção:\n");
        printf("1. Fazer Login\n");
        printf("2. Cadastrar-se\n");
        printf("Digite a opção (1 ou 2): ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            // Tenta fazer o login
            if (login()) {
                printf("Acesso concedido!\n");
            } else {
                printf("Falha no login.\n");
            }
            break; // Sai do loop após o login ou falha
        } else if (opcao == 2) {
            // Cadastro de novo usuário
            FILE *arquivo;
            char cpf[12], senha[50];

            // Abre o arquivo administradores.txt em modo de append (acrescentar)
            arquivo = fopen("administradores.txt", "a");

            // Verifica se o arquivo foi aberto corretamente
            if (arquivo == NULL) {
                printf("Erro ao abrir o arquivo.\n");
                return 1;
            }

            // Solicita o CPF do usuário e valida
            do {
                printf("Digite o CPF (somente números): ");
                scanf("%11s", cpf);  // Limita a leitura a 11 caracteres

                if (!validarCPF(cpf)) {
                    printf("Apenas números são permitidos no CPF.\n");
                } else if (verificarCadastro(cpf)) {
                    printf("CPF já cadastrado. Tente outro CPF.\n");
                }
            } while (!validarCPF(cpf) || verificarCadastro(cpf));

            // Solicita a senha do usuário
            printf("Digite a senha: ");
            scanf("%s", senha);

            // Grava o CPF e a senha no arquivo
            fprintf(arquivo, "%s %s\n", cpf, senha);

            // Confirma o cadastro
            printf("Cadastro realizado com sucesso!\n");

            // Fecha o arquivo
            fclose(arquivo);
            break; // Sai do loop após o cadastro
        } else {
            printf("Digite uma opção válida.\n");
        }
    }

    return 0;
}
