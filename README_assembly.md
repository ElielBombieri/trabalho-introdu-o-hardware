## README: Jogo "Guess the number " (Assembly MIPS)

Este é um programa simples implementado em Assembly MIPS que simula o jogo clássico de adivinhação de números. O objetivo é adivinhar um número secreto gerado entre 1 e 100.

---

### Visão Geral do Jogo

O jogo é um *loop* contínuo de adivinhação.

1.  **Início:** O programa solicita uma **semente** ao usuário.
2.  **Geração do Número Secreto:** Um número alvo entre 1 e 100 é gerado a partir desta semente.
3.  **Palpites:** O jogador insere um palpite e o programa fornece *feedback* ("Muito alto!" ou "Muito baixo!"). O número de tentativas é rastreado.
4.  **Vitória:** Ao acertar o número, o jogador vê o total de tentativas, e um novo jogo é iniciado imediatamente (solicitando uma nova semente).
5.  **Saída:** Se o jogador digitar `0` como palpite, o jogo é encerrado e o número secreto atual é revelado.

---

### Geração do Número Secreto

O número secreto é gerado de forma **pseudoaleatória** usando a semente fornecida pelo usuário, garantindo que a mesma semente sempre produza o mesmo número.

#### Detalhamento da Geração

A geração do número secreto (o `alvo`) segue a fórmula matemática abaixo, implementada através de operações MIPS de divisão e manipulação de registradores:

$$Número Secreto = (\text{Semente} \pmod{100}) + 1$$

O processo em MIPS:

1.  O valor da **Semente** (lida do usuário, em `$t0`) é dividido por 100.
    * **MIPS:** `div $t0, 100`
2.  O **resto** dessa divisão (que estará no intervalo $[0, 99]$) é recuperado do registrador *Hi* (`mfhi $t2`).
3.  É somado `1` ao resto, ajustando o intervalo final para $[1, 100]$.
    * **MIPS:** `addi $t2, $t2, 1`
4.  O valor resultante é salvo na variável `alvo`.

---

### Estrutura do Código (Assembly MIPS)

#### Seção `.data` (Dados)

Esta seção armazena todas as *strings* de texto utilizadas para interação com o usuário e as variáveis principais do jogo:

* **Mensagens (`.asciiz`):** Boas-vindas, solicitação de palpite, feedback (alto, baixo, acerto), início de novo jogo e fim de jogo.
* **Variáveis (`.word`):**
    * `tentativas`: Contador para o número de palpites (inicia em 0).
    * `alvo`: Armazena o número secreto gerado (entre 1 e 100).

#### Seção `.text` (Instruções)

Contém a lógica de execução do programa, começando na *label* `main`.

* **`main`:** Inicialização do jogo.
    * Solicita e lê a semente.
    * Executa a rotina de Geração do Número Secreto.
    * Inicializa `tentativas` para 0.
    * Salta para o `game_loop`.
* **`game_loop`:** Loop principal de adivinhação.
    * Solicita e lê o palpite.
    * Verifica se o palpite é **0** para sair (`exit_game`).
    * Incrementa e salva `tentativas`.
    * Compara o palpite com o `alvo` para saltar para `too_low`, `too_high` ou `correct_guess`.
* **`too_high` / `too_low`:** Rotinas de feedback que exibem a mensagem correspondente e retornam para o `game_loop`.
* **`correct_guess`:**
    * Exibe a mensagem de acerto e o total de tentativas.
    * Inicia um novo ciclo: solicita nova semente, gera novo `alvo` e zera `tentativas`.
    * Retorna para o `game_loop`.
* **`exit_game`:** Encerramento.
    * Exibe a mensagem final e revela o `alvo`.
    * Encerra o programa (`syscall 10`).

---
