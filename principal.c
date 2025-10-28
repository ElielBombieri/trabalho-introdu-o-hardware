/*
 * Jogo: Adivinhe o Número (Guess the Number) - Implementação com Arduino e Teclado 4x4
 *
 * Descrição: Este programa implementa um jogo de adivinhação de número secreto
 * (entre 1 e 100) utilizando um teclado matricial 4x4 e comunicação serial
 * para feedback. A detecção da tecla pressionada é feita por varredura (scanning).
 *
 * Funcionalidades Especiais (Teclas):
 * - '*' (Asterisco): Limpa o palpite atual.
 * - '#' (Cerquilha): Envia o palpite para verificação.
 * - '0' seguido de '#': Encerra o jogo atual, revela o número secreto e inicia um novo.
 */

//Variáveis Globais


// Variável que armazena o número secreto a ser adivinhado (entre 1 e 100).
int target;
// Contador para o número de tentativas feitas pelo jogador.
int tentativas = 0;
// Variável que armazena o palpite (número) digitado pelo jogador no teclado.
int palpite = 0;
// Variável temporária para armazenar o valor do dígito lido (0 a 9).
int valor = 0;
// Variável não utilizada neste código.
int lendo = 0;


// Função setup() - Configuração Inicial

void setup() {
  // Configuração do Teclado Matricial 4x4 
  
  // Configuração dos pinos das LINHAS (Entrada com Pull-up interno: Linhas Ativas em LOW)
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  // Configuração dos pinos das COLUNAS (Saída para Varredura: Colunas Ativas em LOW)
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

  // Configuração Serial e Inicialização do Jogo 
  
  // Inicializa a comunicação serial a 9600 bits por segundo (para feedback).
  Serial.begin(9600);

  // Inicializa o gerador de números aleatórios com uma semente do pino A0.
  randomSeed(analogRead(A0));
  // Gera o número secreto (target) entre 1 e 100 (o '101' é exclusivo).
  target = random(1, 101);

  // Mensagens de boas-vindas e instruções.
  Serial.println("Bem-vindo ao Jogo: Adivinhe o numero!");
  Serial.println("Digite o palpite e pressione #");
  Serial.println("Use * para limpar o campo");
  Serial.println("Para encerrar o jogo e revelar o numero, digite 0 e pressione #\n");
}

//  Função loop() - Lógica Principal do Programa

void loop() {
  int lin, col;
  // Variável para armazenar a tecla lida. Inicializa com caractere nulo ('\0').
  char tecla = '\0';

  
  // Itera sobre as colunas (pinos 8 a 5).
  for (col = 8; col >= 5; col--) {
    // Garante que todas as colunas estão em HIGH (estado de repouso).
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);

    // Coloca a coluna atual em LOW (ativa-a para varredura).
    digitalWrite(col, LOW);

    // Itera sobre as linhas (pinos 12 a 9) para checar se há tecla pressionada.
    for (lin = 12; lin >= 9; lin--) {
      // Leitura da linha: LOW indica tecla pressionada na intersecção (lin, col).
      if (digitalRead(lin) == LOW) {
        
        // --- Mapeamento das Teclas 4x4 ---
        if (lin == 12 && col == 8) tecla = '1';
        else if (lin == 12 && col == 7) tecla = '2';
        else if (lin == 12 && col == 6) tecla = '3';
        else if (lin == 12 && col == 5) tecla = 'A'; 

        else if (lin == 11 && col == 8) tecla = '4';
        else if (lin == 11 && col == 7) tecla = '5';
        else if (lin == 11 && col == 6) tecla = '6';
        else if (lin == 11 && col == 5) tecla = 'B';

        else if (lin == 10 && col == 8) tecla = '7';
        else if (lin == 10 && col == 7) tecla = '8';
        else if (lin == 10 && col == 6) tecla = '9';
        else if (lin == 10 && col == 5) tecla = 'C';

        else if (lin == 9 && col == 8) tecla = '*'; // Limpar
        else if (lin == 9 && col == 7) tecla = '0'; // Dígito 0 / Encerrar
        else if (lin == 9 && col == 6) tecla = '#'; // Enviar Palpite
        else if (lin == 9 && col == 5) tecla = 'D';
        
        // Sai do loop de varredura assim que a primeira tecla é detectada.
        goto fim_varredura; 
      }
    }
    digitalWrite(col, HIGH); // Desativa a coluna atual (retorna ao repouso)
  }
  
  fim_varredura:; // Ponto de continuação após detecção da tecla (via goto)

  // Processamento da Tecla Pressionada
  if (tecla != '\0') {
    // Pequeno atraso (debounce) para estabilizar a leitura.
    delay(200);

    // Caso: DÍGITO (0 a 9) 
    if (tecla >= '0' && tecla <= '9') {
      // Converte o caractere do dígito para seu valor numérico.
      valor = tecla - '0';
      
      // Checa o limite máximo (100). O palpite só pode ter 3 dígitos se for '100'.
      if (palpite < 100) {
        // Acumula o dígito (ex: 12 * 10 + 3 = 123).
        palpite = palpite * 10 + valor;
        Serial.print(valor); // Exibe o dígito digitado.
      } else {
        Serial.println("\nPalpite muito grande! Max: 100.");
        // O palpite é mantido, mas não aceita mais dígitos até ser enviado ou limpo.
      }
    }
    // Caso: '*' (Limpar Palpite) 
    else if (tecla == '*') {
      palpite = 0; 
      Serial.println("\nCampo limpo!");
    }
    // Caso: '#' (Enviar Palpite / Ação) 
    else if (tecla == '#') {
      Serial.print("\nPalpite: ");
      Serial.println(palpite);

      // Verificação de Encerramento (Palpite == 0) 
      if (palpite == 0) {
        Serial.print("Jogo encerrado. O numero secreto era: ");
        Serial.println(target);
        Serial.println("Reiniciando para um novo jogo...\n");
        
        // Reinicia o gerador, sorteia novo 'target' e zera 'tentativas'.
        randomSeed(analogRead(A0));
        target = random(1, 101);
        tentativas = 0;
      }
      // Lógica Normal do Jogo (Palpites 1 a 100) 
      else {
        tentativas++; 

        if (palpite == target) {
          // ACERTOU: Fim de Jogo e Reinício
          Serial.print("Resposta: Acertou! Tentativas: ");
          Serial.println(tentativas);
          Serial.println("\nNovo jogo iniciado!\n");

          // Reinicia o jogo.
          randomSeed(analogRead(A0));
          target = random(1, 101);
          tentativas = 0;
        }
        else if (palpite > target) {
          // Erro: Palpite Alto
          Serial.println("Resposta: Muito alto!");
        }
        else {
          // Erro: Palpite Baixo
          Serial.println("Resposta: Muito baixo!");
        }
      }
      // Limpa o palpite para a próxima rodada (Palpite 0 se for erro, ou 0 no reinício).
      palpite = 0;
    }
    // Caso: Teclas A, B, C, D 
    // Estas teclas são ignoradas, nenhuma ação é executada.
  }
}