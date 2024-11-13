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


    while (fscanf(arquivo, "CPF: %s SENHA: %s", cpfArquivo, senha) != EOF) {
        if (strcmp(cpf, cpfArquivo) == 0) {
            fclose(arquivo);
            return 1; // CPF encontrado
        }
    }

    fclose(arquivo);
    return 0; // CPF não encontrado
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
        if (scanf("%11s", cpf) != 1) {
            printf("Erro na entrada do CPF.\n");
            return;
        }

        if (!validarCPF(cpf)) {
            printf("Apenas numeros sao permitidos no CPF.\n");
        } else if (verificarCadastro(cpf, arquivoNome)) {
            printf("CPF ja cadastrado. Tente outro CPF.\n");
        }
    } while (!validarCPF(cpf) || verificarCadastro(cpf, arquivoNome));

    printf("Digite a senha: ");
    if (scanf("%s", senha) != 1) {
        printf("Erro na entrada da senha.\n");
        fclose(arquivo);
        return;
    }

    fprintf(arquivo, "CPF: %s SENHA: %s\n", cpf, senha);
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
        if (scanf("%11s", cpf) != 1) {
            printf("Erro na entrada do CPF.\n");
            return;
        }

        if (!validarCPF(cpf)) {
            printf("Apenas numeros sao permitidos no CPF.\n");
        } else if (verificarCadastro(cpf, "usuarios.txt")) {
            printf("CPF ja cadastrado. Tente outro CPF.\n");
        }
    } while (!validarCPF(cpf) || verificarCadastro(cpf, "usuarios.txt"));

    printf("Digite a senha: ");
    if (scanf("%s", senha) != 1) {
        printf("Erro na entrada da senha.\n");
        fclose(arquivo);
        return;
    }

    fprintf(arquivo, "CPF: %s  SENHA: %s\n", cpf, senha);
    printf("Investidor cadastrado com sucesso!\n");

    fclose(arquivo);
}

int login() {
    char cpf[12], senha[50], cpfArquivo[12], senhaArquivo[50];

    while (1) {
        printf("Digite o CPF (ou 0 para sair): ");
        if (scanf("%11s", cpf) != 1) {
            printf("Erro na entrada do CPF.\n");
            return 0;
        }

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
        if (scanf("%s", senha) != 1) {
            printf("Erro na entrada da senha.\n");
            return 0;
        }

        FILE *arquivo = fopen("administradores.txt", "r");
        if (arquivo == NULL) {
            printf("Erro ao acessar o arquivo.\n");
            return 0;
        }

        // Agora, vamos ler corretamente o arquivo e comparar as informações.
        int loginValido = 0;
        while (fscanf(arquivo, "CPF: %s SENHA: %s", cpfArquivo, senhaArquivo) != EOF) {
            if (strcmp(cpf, cpfArquivo) == 0 && strcmp(senha, senhaArquivo) == 0) {
                loginValido = 1;
                break;  // Se o login for válido, sai do loop.
            }
        }

        fclose(arquivo);

        if (loginValido) {
            printf("Login bem-sucedido!\n");
            return 1;  // Login bem-sucedido
        } else {
            printf("CPF ou senha incorretos. Tente novamente ou digite 0 para sair.\n");
        }
    }
}

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

        if (opcao == 1) {
            CadastraInvestidor();
        } else if (opcao == 2) {
            break;  // Sai do menu administrador
        } else {
            printf("Digite uma opcao valida.\n");
        }
    }
}

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
            break;  // Sai do loop principal
        }
    }

    return 0;
}

