#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_LINHA 256
#define MAX_USUARIOS 100
#define MAX_TRANSACOES 100

// Cotações das criptomoedas
float cotacao_bitcoin = 250000.0; // Cotação para Bitcoin
float cotacao_ethereum = 15000.0; // Cotação para Ethereum
float cotacao_ripple = 5.0;       // Cotação para Ripple

// Função para gerar uma variação aleatória de -5% a +5%
float gerarVariacao() { return (float)(rand() % 11 - 5) / 100; }

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
        return 0; // Arquivo não encontrado
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
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para excluir um investidor
void excluirInvestidor(const char *arquivoNome) {
    char cpf[12], cpfArquivo[12], senha[50];
    int encontrado = 0;

    printf("Digite o CPF do investidor a ser excluido: ");
    if (scanf("%11s", cpf) != 1) {
        printf("Erro na entrada do CPF.\n");
        return;
    }
    limparBuffer();

    if (!validarCPF(cpf)) {
        printf("CPF invalido. Apenas numeros sao permitidos.\n");
        return;
    }

    FILE *arquivoOriginal = fopen(arquivoNome, "r+");
    if (arquivoOriginal == NULL) {
        printf("Erro ao abrir o arquivo de usuarios.\n");
        return;
    }

    char linha[TAM_LINHA];
    char usuarios_com_cpf[MAX_USUARIOS][TAM_LINHA];
    char usuarios_sem_cpf[MAX_USUARIOS][TAM_LINHA];
    int total_usuarios_com_cpf = 0;
    int total_usuarios_sem_cpf = 0;

    // Lê todos os usuários e separa em duas listas
    while (fgets(linha, sizeof(linha), arquivoOriginal) != NULL) {
        if (strstr(linha, cpf) != NULL) {
            // Encontrou o CPF, substituímos a linha
            if (total_usuarios_com_cpf < MAX_USUARIOS) {
                snprintf(usuarios_com_cpf[total_usuarios_com_cpf], TAM_LINHA, "CPF: EXCLUIDO SENHA: EXCLUIDO\n");
                total_usuarios_com_cpf++;
            }
        } else {
            if (total_usuarios_sem_cpf < MAX_USUARIOS) {
                strncpy(usuarios_sem_cpf[total_usuarios_sem_cpf], linha, TAM_LINHA - 1);
                usuarios_sem_cpf[total_usuarios_sem_cpf][TAM_LINHA - 1] = '\0'; // Garantir que a string seja nula-terminada
                total_usuarios_sem_cpf++;
            }
        }
    }
    fclose(arquivoOriginal);

    // Se não houver usuários encontrados para o CPF
    if (total_usuarios_com_cpf == 0) {
        printf("Nenhum investidor encontrado com o CPF: %s\n", cpf);
        return;
    }

    // Reescreve o arquivo sem o usuário excluído
    FILE *arquivoSaida = fopen(arquivoNome, "w");
    if (arquivoSaida == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Reescreve todos os usuários sem o CPF excluído
    for (int i = 0; i < total_usuarios_sem_cpf; i++) {
        fputs(usuarios_sem_cpf[i], arquivoSaida);
    }

    // Escreve os usuários com o CPF substituído
    for (int i = 0; i < total_usuarios_com_cpf; i++) {
        fputs(usuarios_com_cpf[i], arquivoSaida);
    }

    fclose(arquivoSaida);

    // Registrar no historico.txt
    FILE *historicoArquivo = fopen("historico.txt", "a");
    if (historicoArquivo == NULL) {
        printf("Erro ao abrir o arquivo de historico para escrita! Um novo será criado.\n");
        historicoArquivo = fopen("historico.txt", "w");
        if (historicoArquivo == NULL) {
            printf("Não foi possível criar o arquivo de historico!\n");
            return;
        }
    }

    // Registra a exclusão no histórico
    fprintf(historicoArquivo, "CPF: %s\tEXCLUIDO: %s\n", cpf, cpf); // Registrando o CPF que foi excluído
    fclose(historicoArquivo);

    printf("Investidor com CPF %s excluído com sucesso!\n", cpf);
}

// Função para cadastrar um novo usuário
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
            fclose(arquivo);
            return;
        }
        limparBuffer();

        if (!validarCPF(cpf)) {
            printf("Apenas numeros sao permitidos no CPF.\n");
        } else if (verificarCadastro(cpf, arquivoNome)) {
            printf("CPF ja cadastrado. Tente outro CPF.\n");
        }
    } while (!validarCPF(cpf) || verificarCadastro(cpf, arquivoNome));

    printf("Digite a senha: ");
    if (scanf("%49s", senha) != 1) {
        printf("Erro na entrada da senha.\n");
        fclose(arquivo);
        return;
    }
    limparBuffer();

    fprintf(arquivo, "CPF: %s SENHA: %s SALDO: 0.00\n", cpf, senha);
    printf("Cadastro realizado com sucesso!\n");
    fclose(arquivo);
}

// Função para garantir a entrada de um número float válido
float solicitar_float(const char *mensagem) {
    float valor;
    char buffer[50];
    int valido;

    do {
        printf("%s", mensagem);
        if (fgets(buffer, sizeof(buffer), stdin)) {
            // Tenta converter o valor lido para float
            valido = sscanf(buffer, "%f", &valor);
            if (valido == 1) {
                return valor;
            } else {
                printf("Digite apenas numeros.\n");
            }
        } else {
            printf("Erro na leitura da entrada. Tente novamente.\n");
        }
    } while (1);
}

//Funçao para cadastrar criptomoeda
void cadastrar_criptomoeda() {
    char nome[50];
    float cotacao_inicial, taxa_compra, taxa_venda;

    // Solicita os dados da criptomoeda
    printf("Digite o nome da criptomoeda: ");
    scanf("%49s", nome); // Lê o nome da criptomoeda
    getchar(); // Limpa o buffer de entrada

    cotacao_inicial = solicitar_float("Digite a cotação inicial da criptomoeda: ");
    taxa_compra = solicitar_float("Digite a taxa de compra (em %): ");
    taxa_venda = solicitar_float("Digite a taxa de venda (em %): ");

    // Abre o arquivo para escrita em modo de append
    FILE *arquivo = fopen("criptomoedas.txt", "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    // Escreve os dados no arquivo
    if (fprintf(arquivo, "NOME: %s | COTAÇÃO: %.2f | TAXA COMPRA: %.2f | TAXA VENDA: %.2f\n",
                nome, cotacao_inicial, taxa_compra, taxa_venda) < 0) {
        perror("Erro ao escrever no arquivo");
        fclose(arquivo);
        exit(1);
    }

    // Fecha o arquivo
    fclose(arquivo);

    printf("Criptomoeda cadastrada com sucesso!\n");
}

// Função para excluir uma criptomoeda
void excluir_criptomoeda() {
    char nome[50], linha[200], nome_arquivo_temp[] = "temp.txt";
    int encontrada = 0;

    printf("Digite o nome da criptomoeda que deseja excluir: ");
    scanf("%49s", nome);
    getchar(); // Limpa o buffer de entrada

    FILE *arquivo = fopen("criptomoedas.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    FILE *temp = fopen(nome_arquivo_temp, "w");
    if (temp == NULL) {
        perror("Erro ao criar arquivo temporário");
        fclose(arquivo);
        exit(1);
    }

    // Percorre o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Verifica se a linha contém o nome da criptomoeda
        if (strstr(linha, nome) != NULL) {
            printf("\nCriptomoeda encontrada:\n%s", linha);
            printf("Deseja realmente excluir esta criptomoeda? (s/n): ");
            char confirmacao = getchar();
            getchar(); // Limpa o buffer de entrada

            if (confirmacao == 's' || confirmacao == 'S') {
                printf("Criptomoeda excluída com sucesso!\n");
                encontrada = 1;
                continue; // Ignora a escrita dessa linha no arquivo temporário
            } else {
                printf("Exclusão cancelada.\n");
            }
        }

        // Copia a linha para o arquivo temporário
        fprintf(temp, "%s", linha);
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrada) {
        // Substitui o arquivo original pelo temporário
        if (remove("criptomoedas.txt") != 0 || rename(nome_arquivo_temp, "criptomoedas.txt") != 0) {
            perror("Erro ao atualizar o arquivo");
            exit(1);
        }
    } else {
        printf("Criptomoeda não encontrada no arquivo.\n");
        remove(nome_arquivo_temp); // Remove o arquivo temporário, pois não foi usado
    }
}

// Função para consultar saldo de um investidor pelo CPF
void consultar_saldo_investidor() {
    char cpf[15]; // CPF informado pelo usuário
    char linha[200]; // Buffer para leitura de cada linha do arquivo
    int encontrado = 0;

    // Solicita o CPF do investidor
    printf("Digite o CPF do investidor (somente números): ");
    scanf("%14s", cpf); // Lê o CPF informado
    getchar(); // Limpa o buffer de entrada

    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo usuarios.txt");
        exit(1);
    }

    // Percorre o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Verifica se a linha contém o CPF informado
        if (strstr(linha, cpf) != NULL) {
            printf("\nInvestidor encontrado:\n%s", linha);
            encontrado = 1;
            break; // Sai do loop após encontrar o CPF
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("\nCPF nao encontrado no sistema.\n");
    }
}

// Função para o extrato ter no máximo 100 linhas
void manterUltimasTransacoes() {
  FILE *extratoFile = fopen("extrato.txt", "r");
  if (extratoFile == NULL) {
    printf("Erro ao abrir o arquivo de extrato.\n");
    return;
  }

  // Contar o número de linhas no arquivo
  char line[TAM_LINHA];
  int numLinhas = 0;

  // Contar o número de linhas no arquivo
  while (fgets(line, sizeof(line), extratoFile) != NULL) {
    numLinhas++;
  }
  fclose(extratoFile);

  // Se houver mais de 100 linhas, remover as linhas excedentes
  if (numLinhas > MAX_TRANSACOES) {
    extratoFile = fopen("extrato.txt", "r");
    FILE *tempFile = fopen("extrato_temp.txt", "w");

    if (tempFile == NULL) {
      printf("Erro ao abrir o arquivo temporario.\n");
      fclose(extratoFile);
      return;
    }

    // Ignorar as primeiras (numLinhas - 100) linhas
    int linhasIgnoradas = numLinhas - MAX_TRANSACOES;
    int linhaAtual = 0;

    // Copiar as últimas 100 linhas para o arquivo temporário
    while (fgets(line, sizeof(line), extratoFile) != NULL) {
      if (linhaAtual >= linhasIgnoradas) {
        fputs(line, tempFile);
      }
      linhaAtual++;
    }

    fclose(extratoFile);
    fclose(tempFile);

    // Substituir o arquivo original pelo temporário
    remove("extrato.txt");
    rename("extrato_temp.txt", "extrato.txt");
  }
}

// Função para consultar o extrato
void consultarExtrato() {
  char cpf[20]; // CPF de até 20 caracteres
  char line[TAM_LINHA];
  int cpfEncontrado = 0;
  char cpfFormatado[20]; // Para armazenar "CPF: X"

  // Solicita o CPF do usuário
  printf("Digite o CPF do investidor: ");
  scanf("%19s", cpf); // Limita a entrada para até 19 caracteres
  printf("--------------------------------\n");

  // Formata o CPF para a busca, no formato "CPF: %s"
  snprintf(cpfFormatado, sizeof(cpfFormatado), "CPF: %s", cpf);

  // Verifica se o CPF existe no arquivo usuarios.txt
  FILE *usuariosFile = fopen("usuarios.txt", "r");
  if (usuariosFile == NULL) {
    printf("Erro ao abrir o arquivo de usuarios.\n");
    return;
  }

  while (fgets(line, sizeof(line), usuariosFile) != NULL) {
    if (strncmp(line, cpfFormatado, strlen(cpfFormatado)) == 0) {
      cpfEncontrado = 1;
      break;
    }
  }
  fclose(usuariosFile);

  if (!cpfEncontrado) {
    printf("CPF não encontrado no arquivo de usuarios.\n");
    return;
  }

  // Exibe o extrato para o CPF encontrado
  FILE *extratoFile = fopen("extrato.txt", "r");
  if (extratoFile == NULL) {
    printf("Erro ao abrir o arquivo de extrato.\n");
    return;
  }

  printf("Extrato para o CPF %s:\n", cpf);
  while (fgets(line, sizeof(line), extratoFile) != NULL) {

    // Verifica se a linha começa com o CPF completo no formato "CPF: X"
    if (strncmp(line, cpfFormatado, strlen(cpfFormatado)) == 0) {
      printf("%s", line); // Exibe a linha com a transação
    }
  }
  fclose(extratoFile);

  // Depois de consultar o extrato, garante que o arquivo tenha no máximo 100
  // transações
  manterUltimasTransacoes();
}

// Função para atualizar cotações
void atualizarCotacoes() {
  cotacao_bitcoin += cotacao_bitcoin * gerarVariacao();
  cotacao_ethereum += cotacao_ethereum * gerarVariacao();
  cotacao_ripple += cotacao_ripple * gerarVariacao();

  printf("Cotacoes atualizadas:\n");
  printf("Bitcoin: R$%.2f\n", cotacao_bitcoin);
  printf("Ethereum: R$%.2f\n", cotacao_ethereum);
  printf("Ripple: R$%.2f\n", cotacao_ripple);

  return;
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
        limparBuffer();

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
        limparBuffer();

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
        printf("2. Excluir Investidor\n");
        printf("3. Cadastrar criptomoeda\n");
        printf("4. Excluir criptomoeda\n");
        printf("5. Consultar saldo\n");
        printf("6. Consultar extrato\n");
        printf("7. Atualizar cotacao\n");
        printf("8. Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Erro na entrada da opcao.\n");
            return;
        }
        limparBuffer();

        if (opcao == 1) {
            cadastrarUsuario("usuarios.txt");
        } else if (opcao == 2) {
            excluirInvestidor("usuarios.txt");
        } else if (opcao == 3) {
            cadastrar_criptomoeda("criptomoedas.txt");
        } else if (opcao == 4) {
            excluir_criptomoeda("criptomoedas.txt");
        } else if (opcao == 5) {
            consultar_saldo_investidor("usuarios.txt");
        } else if (opcao == 6) {
            consultarExtrato();
        } else if (opcao == 7) {
            atualizarCotacoes();
        } else if (opcao == 8) {
            break;
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
        limparBuffer();

        if (opcao == 1) {
            if (login()) {
                MenuADM();
            } else {
                printf("Falha no login.\n");
            }
        } else if (opcao == 2) {
            cadastrarUsuario("administradores.txt");
        } else {
            printf("Digite uma opcao valida.\n");
        }

        printf("Deseja sair? (1 - Sim, 0 - Não): ");
        if (scanf("%d", &sair) != 1 || sair == 1) {
            printf("Saindo do sistema...\n");
            break;
        }
        limparBuffer();
    }

    return 0;
}