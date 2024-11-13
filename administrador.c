#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função para validar CPF (só números)
int validarCPF(char cpf[]) {
    for (int i = 0; i < strlen(cpf); i++) {
        if (!isdigit(cpf[i])) {
            return 0;
        }
    }
    return 1;
}

// Função para verificar se o CPF já está cadastrado no arquivo
int verificarCadastro(char cpf[], const char* arquivoNome) {
    FILE *arquivo;
    char cpfArquivo[12], senha[50];

    arquivo = fopen(arquivoNome, "r");
    if (arquivo == NULL) {
        return 0;  // Se o arquivo não puder ser aberto, assume que não há cadastro
    }

    while (fscanf(arquivo, "CPF: %s SENHA: %s", cpfArquivo, senha) != EOF) {
        if (strcmp(cpf, cpfArquivo) == 0) {
            fclose(arquivo);
            return 1; // CPF encontrado no arquivo
        }
    }

    fclose(arquivo);
    return 0; // CPF não encontrado no arquivo
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);  // Limpa o buffer até a nova linha ou fim de arquivo
}

// Função para cadastrar um novo usuário
void cadastrarUsuario(const char* arquivoNome) {
    FILE *arquivo;
    char cpf[12], senha[50];

    arquivo = fopen(arquivoNome, "a");  // Abrindo o arquivo no modo append (adicionar)
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    do {
        printf("Digite o CPF (somente numeros): ");
        if (scanf("%11s", cpf) != 1) {
            printf("Erro na entrada do CPF.\n");
            fclose(arquivo);
            return;
        }
        limparBuffer();  // Limpar o buffer após a entrada

        if (!validarCPF(cpf)) {
            printf("Apenas numeros sao permitidos no CPF.\n");
        } else if (verificarCadastro(cpf, arquivoNome)) {
            printf("CPF ja cadastrado. Tente outro CPF.\n");
        }
    } while (!validarCPF(cpf) || verificarCadastro(cpf, arquivoNome));

    printf("Digite a senha: ");
    if (scanf("%49s", senha) != 1) {  // Usando %49s para garantir que o buffer não seja excedido
        printf("Erro na entrada da senha.\n");
        fclose(arquivo);
        return;
    }
    limparBuffer();  // Limpar o buffer após a entrada

    // Escreve os dados no arquivo
    fprintf(arquivo, "CPF: %s SENHA: %s\n", cpf, senha);
    printf("Cadastro realizado com sucesso!\n");

    fclose(arquivo);
}

// Função de login para verificar as credenciais de um administrador
int login() {
    char cpf[12], senha[50], cpfArquivo[12], senhaArquivo[50];

    while (1) {
        printf("Digite o CPF (ou 0 para sair): ");
        if (scanf("%11s", cpf) != 1) {
            printf("Erro na entrada do CPF.\n");
            return 0;
        }
        limparBuffer();  // Limpar o buffer após a entrada

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
        if (scanf("%49s", senha) != 1) {
            printf("Erro na entrada da senha.\n");
            return 0;
        }
        limparBuffer();  // Limpar o buffer após a entrada

        FILE *arquivo = fopen("administradores.txt", "r");
        if (arquivo == NULL) {
            printf("Erro ao acessar o arquivo.\n");
            return 0;
        }

        int loginValido = 0;
        while (fscanf(arquivo, "CPF: %s SENHA: %s", cpfArquivo, senhaArquivo) != EOF) {
            if (strcmp(cpf, cpfArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
                loginValido = 1;
                break;
            }
        }

        fclose(arquivo);

        if (loginValido) {
            printf("Login bem-sucedido!\n");
            return 1;
        } else {
            printf("CPF ou senha incorretos. Tente novamente ou digite 0 para sair.\n");
        }
    }
}

// Menu do Administrador
void MenuADM() {
    int opcao;

    while (1) {
        printf("\nMenu de Administrador:\n");
        printf("1. Cadastrar Investidor\n");
        printf("2. Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Erro na entrada da opcao.\n");
            return;
        }
        limparBuffer();  // Limpar o buffer após a entrada

        if (opcao == 1) {
            cadastrarUsuario("usuarios.txt");  // Chama a função de cadastro
        } else if (opcao == 2) {
            break;  // Sai do menu administrador
        } else {
            printf("Digite uma opcao valida.\n");
        }
    }
}

// Função principal
int main() {
    int opcao;
    int sair = 0;

    while (!sair) {
        printf("Bem-vindo a Central de Administradores do Grupo FEInance\n\n");
        printf("Escolha uma opcao:\n");
        printf("1. Fazer Login\n");
        printf("2. Cadastrar-se\n");
        printf("Digite a opcao (1 ou 2): ");
        if (scanf("%d", &opcao) != 1) {
            printf("Erro na entrada da opcao.\n");
            return 0;
        }
        limparBuffer();  // Limpar o buffer após a entrada

        if (opcao == 1) {
            if (login()) {
                MenuADM();  // Chama o menu do administrador
            } else {
                printf("Falha no login.\n");
            }
        } else if (opcao == 2) {
            cadastrarUsuario("administradores.txt");
            MenuADM();  
        } else {
            printf("Digite uma opcao valida.\n");
        }

        printf("Deseja sair? (1 - Sim, 0 - Não): ");
        if (scanf("%d", &sair) != 1 || sair == 1) {
            printf("Saindo do sistema...\n");
            break;
        }
        limparBuffer();  // Limpar o buffer após a entrada
    }

    return 0;
}
