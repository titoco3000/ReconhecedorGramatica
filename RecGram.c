/*
Reconhecedor de Gramatica

Tito Guidotti - 32120613
Mateus Yoon - 32159609

Reconhecece a linguagem definida pela seguinte gramática:

<programa> ::= <declarações> <comandos>
<declarações> ::= <declaração> | <declaração> <declarações>
<declaração> ::= int <variavel>;
<comandos> ::= <comandos> ; <atribuição> | <atribuição>
<atribuição> ::= <variavel> = <expr>
<expr> ::= <expr> + <termo> | <expr> - <termo> | + <termo> | - <termo> | <termo>
<termo> ::= <termo> * <fator> | <termo> / <fator> | <fator>
<fator> ::= <numero> | (<expr>)
<variável> ::= <letra> <variável> | <letra>
<numero> ::= <digito> <número> | <digito>
<letra> ::= a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z
<digito> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

Depois de extendida para levar em conta comentarios e intervalos e fatorada, fica:

<programa> ::= ( <intervalo> | λ) <declarações> (<intervalo> | λ) <comandos>(<intervalo> | λ) EOF
<declarações> ::= <declaração> { (<intervalo> | λ) <declaração> }
<declaração> ::= int <intervalo> <variavel> (<intervalo> | λ) ;
<comandos> ::= <atribuição> { (<intervalo> | λ) ; (<intervalo> | λ) <atribuição>}
<atribuição> ::= <variavel> (<intervalo> | λ)= (<intervalo> | λ) <expr>
<expr> ::= ( ( + | - ) (<intervalo> | λ) | λ) <termo> { (<intervalo> | λ) (+|-) (<intervalo> | λ) <termo> }
<termo> ::= <fator> { (<intervalo> | λ) ( * | / ) (<intervalo> | λ) <fator> }
<fator> ::= <numero> | '(' (<intervalo> | λ) <expr> (<intervalo> | λ) ')'
<variável> ::= <letra> ( <variável> | λ )
<numero> ::= <digito> ( <número> | λ )
<letra> ::= a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z
<digito> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
<intervalo> ::= ( // {qualquer exceto \n} \n | <espaço> | <tab> | \n ) (<intervalo> | λ)

Argumentos: nome do arquivo a ser analisado
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char *buffer;
int pos = 0;
int linha_atual = 1;

bool programa();
bool declaracoes();
bool declaracao();
bool comandos();
bool atribuicao();
bool expr();
bool termo();
bool fator();
bool variavel();
bool numero();
bool letra();
bool digito();
bool digito();
bool intervalo();

// <programa> ::= ( <intervalo> | λ) <declarações> (<intervalo> | λ) <comandos>(<intervalo> | λ) EOF
bool programa(){
    intervalo();
    if(declaracoes()){
        intervalo();
        if(comandos()){
            intervalo();
            if(buffer[pos++] == '\0')
                return true;
        }
    }
    return false;
}

// <declarações> ::= <declaração> { (<intervalo> | λ) <declaração> }
bool declaracoes(){
    if(declaracao()){
        while (true)
        {
            int i = pos;
            int l = linha_atual;
            intervalo();
            if(!declaracao()){
                pos = i;
                linha_atual = l;
                break;
            }
        }
        return true;
    }
    return false;
}

// <declaração> ::= int <intervalo> <variavel> (<intervalo> | λ) ;
bool declaracao(){
    if(buffer[pos++] == 'i' &&buffer[pos++] == 'n' && buffer[pos++] == 't' && intervalo() && variavel() && buffer[pos++] == ';'){
        return true;
    }
    return false;
}

// <comandos> ::= <atribuição> { (<intervalo> | λ) ; (<intervalo> | λ) <atribuição>}
bool comandos(){
    if(atribuicao()){
        while (true)
        {
            int i = pos;
            int l = linha_atual;
            intervalo();
            if(buffer[pos++]==';'){
                intervalo();
                if(!atribuicao()){
                    return false;
                }
            }
            else{
                pos = i;
                l = linha_atual;
                break;
            }
        }
        return true;
    }
    return false;
}

// <atribuição> ::= <variavel> (<intervalo> | λ)= (<intervalo> | λ) <expr>
bool atribuicao(){
    if(variavel()){
        intervalo();
        if(buffer[pos++]=='='){
            intervalo();
            if(expr()){
                return true;
            }
        }
    }
    return false;
}

// <expr> ::= ( ( + | - ) (<intervalo> | λ) | λ) <termo> { (<intervalo> | λ) (+|-) (<intervalo> | λ) <termo> }
bool expr(){
    char s = buffer[pos++];
    if(s == '+' || s =='-'){
        intervalo();
    }
    else{
        pos--;
    }
    if(termo()){
        while (true)
        {
            int i = pos;
            int l = linha_atual;
            intervalo();
            s = buffer[pos++];
            if(s == '+' || s =='-'){
                intervalo();
                if(!termo()){
                    pos = i;
                    break;
                }
            }
            else{
                pos = i;
                linha_atual = l;
                break;
            }
        }
        return true;
    }
    return false;
}

// <termo> ::= <fator> { (<intervalo> | λ) ( * | / ) (<intervalo> | λ) <fator> }
bool termo(){
    if(fator()){
        while (true)
        {
            int i = pos;
            int l = linha_atual;
            intervalo();
            char s = buffer[pos++];
            if(s == '*' || s =='/'){
                intervalo();
                if(!fator()){
                    pos = i;
                    linha_atual = l;
                    break;
                }
            }
            else{
                pos = i;
                linha_atual = l;
                break;
            }
        }
        return true;
    }
    return false;
}

// <fator> ::= <numero> | '(' (<intervalo> | λ) <expr> (<intervalo> | λ) ')'
bool fator(){
    char s = buffer[pos++];
    if(s == '('){
        intervalo();
        if(expr()){
            intervalo();
            if(buffer[pos++] == ')'){
                return true;
            }
        }
    }
    else{
        pos--;
        if(numero()){
            return true;
        }
    }
    return false;
}

// <variável> ::= <letra> ( <variável> | λ )
bool variavel(){
    if(letra()){
        variavel();
        return true;
    }
    return false;
}

// <numero> ::= <digito> ( <número> | λ )
bool numero(){
    if(digito()){
        numero();
        return true;
    }
    return false;
}

// <letra> ::= a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z
bool letra(){
    char s = buffer[pos++];
    if(s>='a'&&s<='z')
        return true;
    pos--;
    return false;
}

// <digito> ::= 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
bool digito(){
    char s = buffer[pos++];
    if(s>='0'&&s<='9')
        return true;
    pos--;
    return false;
}

// <intervalo> ::= ( // {qualquer exceto \n} \n | <espaço> | <tab> | \n ) (<intervalo> | λ)
bool intervalo(){
    char s = buffer[pos++];
    if(s==' '||s=='\t'){
        intervalo();
        return true;
    }
    if(s=='\n'){
        linha_atual++;
        intervalo();
        return true;
    }
    if( s == '/' && buffer[pos] == '/'){
        while (buffer[++pos] !='\n');
        intervalo();
        return true;
    }
    pos--;
    return false;
}

int main(int argc, char *argv[ ]){
    FILE *arquivo;
    if(argc>1)
        //abre o arquivo
        arquivo = fopen(argv[1],"r");
    else
        arquivo = fopen("teste.txt","r");

    if(arquivo == NULL){
        fprintf(stderr, "Falha ao abrir arquivo: %s\n",argv[1]);
        return 1;
    }

    //obtem tamanho do arquivo
    fseek(arquivo, 0, SEEK_END);
    long tamanho_arquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    //aloca buffer para conteudo do arquivo
    buffer = (char*)malloc(tamanho_arquivo);
    //copia o conteudo
    fread(buffer, sizeof(char), tamanho_arquivo, arquivo);
    //libera o arquivo
    fclose(arquivo);

    if(programa()){
        printf("Entrada aceita sem erros.\n%d linhas analisadas\n",linha_atual);
    }
    else{
        printf("Falha na derivacao\nErro na linha %d.\n",linha_atual);
    }

    //libera memória alocada
    free(buffer);
    return 0;
}