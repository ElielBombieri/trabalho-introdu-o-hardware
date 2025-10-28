.data
mensagem_boas_vindas: .asciiz "Bem-vindo ao Jogo: Adivinhe o numero!\nDigite a semente: "
mensagem_palpite:     .asciiz "Digite o palpite: "
mensagem_alto:        .asciiz "Resposta: Muito alto!\n"
mensagem_baixo:       .asciiz "Resposta: Muito baixo!\n"
mensagem_acerto:      .asciiz "Resposta: Acertou! Tentativas: "
mensagem_novo_jogo:   .asciiz "\nNovo jogo iniciado!\n"
mensagem_fim:         .asciiz "Jogo encerrado. Numero secreto era: "
nova_linha:           .asciiz "\n"
tentativas:           .word 0
alvo:                 .word 0

.text
main:
    li $v0, 4                   # print msg inicial
    la $a0, mensagem_boas_vindas
    syscall

    li $v0, 5                   # lê semente
    syscall
    move $t0, $v0

    li $t1, 100                 # gera número entre 1 e 100
    div $t0, $t1
    mfhi $t2
    addi $t2, $t2, 1
    sw $t2, alvo

    sw $zero, tentativas        # zera tentativas

game_loop:
    li $v0, 4                   # pede palpite
    la $a0, mensagem_palpite
    syscall

    li $v0, 5                   # lê palpite
    syscall
    move $t3, $v0

    beq $t3, $zero, exit_game   # se 0, sai do jogo

    lw $t4, tentativas          # soma tentativa
    addi $t4, $t4, 1
    sw $t4, tentativas

    lw $t2, alvo                # compara palpite com alvo
    beq $t3, $t2, correct_guess
    slt $t5, $t3, $t2
    bne $t5, $zero, too_low

too_high:
    li $v0, 4                   # msg muito alto
    la $a0, mensagem_alto
    syscall
    j game_loop

too_low:
    li $v0, 4                   # msg muito baixo
    la $a0, mensagem_baixo
    syscall
    j game_loop

correct_guess:
    li $v0, 4                   # msg de acerto
    la $a0, mensagem_acerto
    syscall
    li $v0, 1                   # mostra tentativas
    lw $a0, tentativas
    syscall
    li $v0, 4
    la $a0, nova_linha
    syscall
    li $v0, 4
    la $a0, mensagem_novo_jogo
    syscall

    li $v0, 4                   # pede nova semente
    la $a0, mensagem_boas_vindas
    syscall
    li $v0, 5
    syscall
    move $t0, $v0

    li $t1, 100                 # novo número
    div $t0, $t1
    mfhi $t2
    addi $t2, $t2, 1
    sw $t2, alvo
    sw $zero, tentativas
    j game_loop

exit_game:
    li $v0, 4                   # msg final
    la $a0, mensagem_fim
    syscall
    li $v0, 1                   # mostra o número secreto
    lw $a0, alvo
    syscall
    li $v0, 4
    la $a0, nova_linha
    syscall
    li $v0, 10                  # encerra
    syscall
