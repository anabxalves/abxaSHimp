#include "mySHfunc.h"

int main()
{
    /*
        if batch:
            - verificar se o arquivo existe (e fazer consequentes tratamentos)
            - a cada comando executado, printar o que está sendo executado
            - não será impresso o prompt
        
        if interativo:
            prompt => 'abxa (tipo de acesso)> '
                pode ser Sequencial, chamado com o comando "style sequential"
                    prompt deverá ser: abxa seq>

                pode tambem ser Paralelo, chamado com o comando "style parallel"
                    prompt deverá ser: abxa par>

                tipo de acesso default: Sequencial

            - poder ter mais de um comando por linha (separados por ;), e sua execução dependerá do tipo de acesso

                if seq:
                    - uma thread de execução (mesmo processo filho)
                    - o proximo processo somente será executado quando o seu antecessor tiver completado
                
                if par:
                    - criação de várias threads de execução (processos filhos)
                    - os processos ocorrerão ao mesmo tempo, não havendo limites de threads
            
            - o prompt só deve ficar disponível quando todos os comandos tiverem sido executados

            - deve usar fork(), exec(), wait(), dup2(), pipe(), pthread_create() e pthread_join().

            passo a passo:
            1. receber a string de entrada dos comandos
            2. separar os comandos (por ;)
            3. verificar se é batch (args == 2) ou interativo
            4. verificar se tipo seq ou par
            5. verificar se comando é nativo ou não (se é um dos comandos shell ou se chama execução externa (utilizar o execvp())
            6. printar o resultado no terminal
            7. whilw não EOF ou exit

        finalizar com exit();
    */

    return 0;
}