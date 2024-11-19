# FEINANCE - Exchange de Criptomoedas

## 👨‍💻 **Programadores**
**Nome:**  Igor Botini da Silva **RA:** 22.124.023-7 <br>
**Nome:**  Henrique Gaiardoni Sant Anna **RA:** 22.124.009-6 
## 📝 **Sobre o Projeto** 
Projeto se trata de um sistema de compra e venda de criptomoeda, gerenciado totalmente no terminal, utilizando a liguagem de programação: **C**<br>
Os sistemas (administrador.c e Main.c) diponiveis possuem as seguintes funções: <br><br>
    **administrador.c** = Cuida de toda a parte de gerenciamento de cadastro, exclusão de cadastro, cadastro de criptomoedas, mudança de valor entre outros. <br><br>
    **Main.c** = Cuida de toda a parte de investimentos, ou seja, o usuario final, sendo possivel comprar moedas 
que o seu gerente cadastrou e aplicou as devidas cotações.
## 🪄 **Funcionalidade do Sistema** 

- **Cadastrar Usuario:** Possibilidade de cadastrar até 10 usuarios no sistema

    🔍 **Código do Comando:**  `RUN -> 2 para cadastrar um novo usuário `

- **Depositar Fundos:** Sistema suporta depositos na moeda: Real, sem limite de deposito.

    🔍 **Código do Comando:**  `3. Depositar`

- **Sacar Fundos:** Caso necessite sacar o valor em real, tambem é possivel, pois o sistema possui uma função de saque, basta possuir o valor na moeda Real (R$).

    🔍 **Código do Comando:**  `4. Sacar`

- **Comprar Criptomoedas:** Sistema possui algumas moedas para comprar, basta apenas ter o valor em Real (R$) na conta, escolher a moeda e efetuar a compra e com as devidas taxas aplicadas o saldo será atualizado

    🔍 **Código do Comando:**  `5. Comprar criptomoedas`
    

- **Vender Criptomoedas:** Sendo possivel tambem vender as moedas compradas, escolhendo a moeda e a quantidade que deseja vender apos confirmar, o valor de venda sera convertido pra real (R$), com as devidas taxas aplicadas e enviado ao saldo de Reais (R$)

    🔍 **Código do Comando:**  `6. Vender criptomoedas`

 - **Deslogar da Conta:** Sai da conta e finaliza o sistema, evitando que qualquer pessoa consiga analisar sua conta

    🔍 **Código do Comando:**  `7. Atualizar Cotações`
     - **Deslogar da Conta:** Atualizar Cotações 


## ⌨️ **Como Usar o Sistema - ADM**

1. Abra o terminal e navegue até o diretório onde o arquivo `main.c` está localizado.

2. Compile o código usando o comando abaixo:
   ```bash
   gcc administrador.c 
   ```
3. Compile o código usando o comando abaixo:
   ```bash
   ./a.out
   ```
4. Selecionar 2 para se cadastrar    
    ```bash
    "Digite seu CPF(SOMENTE NUMERO): "
    "Digite sua senha: "
   ```
5. Ao acessar o sistema, ele da as seguintes opções
    ```bash
    Menu de Administrador:
    1. Cadastrar Investidor
    2. Excluir Investidor
    3. Cadastrar criptomoeda
    4. Excluir criptomoeda
    5. Consultar saldo
    6. Consultar extrato
    7. Atualizar cotacao
    8. Sair
    Escolha uma opcao:
   ```   
6. Seleciona a sua opção desejada e siga o fluxo de uso

## ⌨️ **Como Usar o Sistema - INVESTIDOR**




1. Abra o terminal e navegue até o diretório onde o arquivo `main.c` está localizado.

2. Compile o código usando o comando abaixo:
   ```bash
   gcc Main.c 
   ```
3. Selecionar 2 para se cadastrar    

4. Desposite um valor, utilizando o comando:
   ```bash
   3 - Depositar Valor
   ```
5. Atualize as cotações, utilizando o comando:
    ```bash
   7 - Atualizar Cotações
   ```
6. Escolha a moeda que deseja comprar, digite o valor e confirme com (S), utilizando o comando abaixo:

    ```bash
   5 - Comprar Criptomoedas
   ```

7. Atualize a cotação para verificar se houve ganho ou perde

8. Seleciona o Comando abaixo para vender um valor de criptomoeda:
    ```bash
   6 - Vender Criptomoeda
   ```
9. Apos confirmar a venda, o valor sera inserido na sua carteira real, utilizando o comando abaixo, o usuario verifica o saldo total de sua carteira.
    ```bash
   1 - Consultar Saldo
   ```
10. Agora o usuario pode sacar o valor em real, removendo o valor de sua carteira do sistema, utilizando o comando:
    ```bash
     4 - Sacar
    ```
