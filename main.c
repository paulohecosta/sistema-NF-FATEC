/*
by PAULO COSTA
criado em 15/05/2016
editado em 16/05/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <string.h>
#include <unistd.h>

#define N_TAM 50        //tamanho de nome de cliente
#define D_TAM 50        //tamanho da descrição do produto
#define ID_TAM 20       //tamanho para CPF ou CNPJ
#define C_QTDE 1000       //quantidade maxima de clientes
#define P_QTDE 1000       //quantidade maxima de produtos
#define NF_QTDE 10       //quantidade de produtos em uma nota fiscal

typedef struct
{
    char nome[N_TAM];
    char id[ID_TAM];
} CLIENTE;

typedef struct
{
    int codigo;
    char descricao[D_TAM];
    float valor;
} PRODUTO;

//carrega os dados a partir dos arquivos dat armazenados
void carregarDados(CLIENTE *cvetor, int *cpos, PRODUTO *pvetor, int *ppos, int *npos)
{
    CLIENTE *cload = cvetor+(*cpos);
    FILE *clientes;
    PRODUTO *pload = pvetor+(*ppos);
    FILE *produtos;
    //carrega clientes
    clientes = fopen("registros\\clientes.dat", "a+");
    if (clientes == NULL)
        printf("OCORREU UM PROBLEMA AO ABRIR O ARQUIVO!\n");
    else
    {
        fread(&(*cload), sizeof(CLIENTE), 1, clientes);
        while (!feof(clientes))
        {
            *cpos = (*cpos) + 1;
            CLIENTE *cload = cvetor+(*cpos);
            fread(&(*cload), sizeof(CLIENTE), 1, clientes);
        }
        if(ferror(clientes))
            printf("ERRO NA LEITURA!!!");
        else
        {
            printf("CARREGANDO CLIENTES...\n");
            Sleep(1000);
        }
    }
    fclose(clientes);
    //carrega produtos
    produtos = fopen("registros\\produtos.dat", "a+");
    if (produtos == NULL)
        printf("OCORREU UM PROBLEMA AO ABRIR O ARQUIVO!\n");
    else
    {
        fread(&(*pload), sizeof(PRODUTO), 1, produtos);
        while (!feof(produtos))
        {
            *ppos = (*ppos) + 1;
            PRODUTO *pload = pvetor+(*ppos);
            fread(&(*pload), sizeof(PRODUTO), 1, produtos);
        }
        if(ferror(produtos))
            printf("ERRO NA LEITURA!!!");
        else
        {
            printf("CARREGANDO PRODUTOS...\n");
            Sleep(1000);
            system("cls");
        }
    }
    fclose(produtos);
    //carrega o indice de notas, para o sistema reconhecer onde parou a contagem
    FILE *notas;
    for((*npos)=0; ; (*npos)++)
    {
        int indice = (*npos);
        char* extensao = ".dat";
        char arquivo[19];
        snprintf(arquivo, sizeof(arquivo), "notas\\NF-%05d%s", indice, extensao);

        notas = fopen(arquivo, "r+");
        if (notas == NULL) break;
        fclose(notas);
    }
}
//corrige cpf/cnpj sem ponto e traço, para com ponto e traço
void corrigirCPF(CLIENTE *cvetor, int cpos)
{
    CLIENTE *novo = cvetor+cpos;
    char corrigido[ID_TAM];
    if(strlen((*novo).id) <= 12)
    {
        corrigido[0] = (*novo).id[0];
        corrigido[1] = (*novo).id[1];
        corrigido[2] = (*novo).id[2];
        corrigido[3] = '.';
        corrigido[4] = (*novo).id[3];
        corrigido[5] = (*novo).id[4];
        corrigido[6] = (*novo).id[5];
        corrigido[7] = '.';
        corrigido[8] = (*novo).id[6];
        corrigido[9] = (*novo).id[7];
        corrigido[10] = (*novo).id[8];
        corrigido[11] = '-';
        corrigido[12] = (*novo).id[9];
        corrigido[13] = (*novo).id[10];
        corrigido[14] = '\0';
    }
    else
    {
        corrigido[0] = (*novo).id[0];
        corrigido[1] = (*novo).id[1];
        corrigido[2] = '.';
        corrigido[3] = (*novo).id[2];
        corrigido[4] = (*novo).id[3];
        corrigido[5] = (*novo).id[4];
        corrigido[6] = '.';
        corrigido[7] = (*novo).id[5];
        corrigido[8] = (*novo).id[6];
        corrigido[9] = (*novo).id[7];
        corrigido[10] = '/';
        corrigido[11] = (*novo).id[8];
        corrigido[12] = (*novo).id[9];
        corrigido[13] = (*novo).id[10];
        corrigido[14] = (*novo).id[11];
        corrigido[15] = '-';
        corrigido[16] = (*novo).id[12];
        corrigido[17] = (*novo).id[13];
        corrigido[18] = '\0';
    }
    strcpy((*novo).id, corrigido);
}
//cadastro de cliente
void cadastrarCliente(CLIENTE *cvetor, int *cpos)
{
    CLIENTE *novo = cvetor+(*cpos);
    FILE *clientes;
    //cadastro do nome ou razão social do cliente
    printf("DIGITE O NOME OU RAZÃO SOCIAL DO NOVO CLIENTE:\n");
    fflush(stdin);
    fgets((*novo).nome, N_TAM, stdin);
    strupr((*novo).nome);
    //cadastro do cpf ou cnpj do cliente
    printf("DIGITE O CPF/CNPJ DO NOVO CLIENTE SEM PONTOS OU HÍFENS:\n");
    fflush(stdin);
    fgets((*novo).id, ID_TAM, stdin);
    while(strlen((*novo).id) != 12 && strlen((*novo).id) != 15)
    {
        printf("VOCÊ DIGITOU UM NÚMERO INCORRETO!\n");
        printf("CPF TEM 11 DIGITOS, CNPJ TEM 14, SEM PONTO OU HÍFEN:\n");
        fflush(stdin);
        fgets((*novo).id, ID_TAM, stdin);
    }
    corrigirCPF(cvetor, (*cpos));
    //guarda clientes no arquivo
    clientes = fopen("registros\\clientes.dat", "a+");
    if (clientes == NULL)
        printf("OCORREU UM PROBLEMA AO ABRIR O ARQUIVO!\n");
    else
    {
        fwrite(&(*novo), sizeof(CLIENTE), 1, clientes);
        if(ferror(clientes))
            printf("ERRO NA GRAVAÇÃO DO ARQUIVO!\n");
        else
            printf("CADASTRO REALIZADO COM SUCESSO!");
    }
    fclose(clientes);
    Sleep(1000);

    *cpos = (*cpos) + 1;
}
//cadastro de produto
void cadastrarItem(PRODUTO *pvetor, int *ppos)
{
    PRODUTO *novo = pvetor+(*ppos);
    FILE *produtos;
    //cadastro do codigo do produto
    (*novo).codigo = (*ppos);
    //cadastro da descricao do produto
    printf("DIGITE A DESCRIÇÃO DO PRODUTO:\n");
    fflush(stdin);
    fgets((*novo).descricao, N_TAM, stdin);
    strupr((*novo).descricao);
    //cadastro do valor do produto
    printf("DIGITE O VALOR, EM REAIS, DO PRODUTO:\n");
    scanf("%f", &(*novo).valor);
    //guarda produtos no arquivo
    produtos = fopen("registros\\produtos.dat", "a+");
    if (produtos == NULL)
        printf("OCORREU UM PROBLEMA AO ABRIR O ARQUIVO!\n");
    else
    {
        fwrite(&(*novo), sizeof(PRODUTO), 1, produtos);
        if(ferror(produtos))
            printf("ERRO NA GRAVAÇÃO DO ARQUIVO!\n");
        else
            printf("CADASTRO REALIZADO COM SUCESSO!");
    }
    fclose(produtos);
    Sleep(1000);

    *ppos = (*ppos) + 1;
}
//busca cliente por nome, ou parte do nome(busca mais inteligente)
void buscarPorNome(CLIENTE cvetor[], int cpos)
{
    char nome[N_TAM];
    int cont;
    int busc1, busc2, busc3=0;
    printf("DIGITE O NOME, OU PARTE DO NOME DO CLIENTE:\n");
    fflush(stdin);
    fgets(nome, N_TAM, stdin);
    strupr(nome);
    for(cont=0; cont<cpos; cont++)
    {
        busc2 = 0;
        for(busc1=0; busc1<strlen(cvetor[cont].nome); busc1++)
        {
            if(nome[busc2] == cvetor[cont].nome[busc1]) busc2++;
            if(busc2 == strlen(nome))
            {
                if(strlen(cvetor[cont].id)<=15)
                    printf("CLIENTE Nº:%04d  CPF:%s\nNOME:%s\n", cont, cvetor[cont].id, cvetor[cont].nome);
                else
                    printf("CLIENTE Nº:%04d  CNPJ:%s\nRAZÃO SOCIAL:%s\n", cont, cvetor[cont].id, cvetor[cont].nome);
                busc3 = 1;
            }
        }
    }
    if(busc3==0) printf("NENHUM RESULTADO ENCONTRADO!\n");
}
//busca cliente por cpf ou cnpj, ou parte dele (busca inteligente)
void buscarPorCPF(CLIENTE cvetor[], int cpos)
{
    char id[ID_TAM];
    int cont;
    int busc1, busc2, busc3=0;
    printf("DIGITE O CPF OU CNPJ DO CLIENTE, OU PARTE DELE:\n");
    fflush(stdin);
    fgets(id, N_TAM, stdin);
    for(cont=0; cont<cpos; cont++)
    {
        busc2 = 0;
        for(busc1=0; busc1<strlen(cvetor[cont].id); busc1++)
        {
            if(id[busc2] == cvetor[cont].id[busc1]) busc2++;
            if(busc2 == strlen(id))
            {
                if(strlen(cvetor[cont].id)<=15)
                    printf("CLIENTE Nº:%04d  CPF:%s\nNOME:%s\n", cont, cvetor[cont].id, cvetor[cont].nome);
                else
                    printf("CLIENTE Nº:%04d  CNPJ:%s\nRAZÃO SOCIAL:%s\n", cont, cvetor[cont].id, cvetor[cont].nome);
                busc3 = 1;
            }
        }
    }
    if(busc3==0) printf("NENHUM RESULTADO ENCONTRADO!\n");
}
//tela de chamada para as opções de busca de cliente
void buscarCliente(CLIENTE cvetor[], int cpos)
{
    int tela = 1;
    while(tela>0)
    {
        system("cls");
        printf("+++++++++++++++++++++++++++++++++++++\n");
        printf("+++        BUSCAR CLIENTE         +++\n");
        printf("+++++++++++++++++++++++++++++++++++++\n");
        printf("+++  [1]-BUSCAR POR NOME          +++\n");
        printf("+++  [2]-BUSCAR POR CPF/CNPJ      +++\n");
        printf("+++  [9]-SAIR                     +++\n");
        printf("+++++++++++++++++++++++++++++++++++++\n\n");
        printf("DIGITE O NÚMERO DA OPÇÃO DESEJADA:\n");
        scanf("%d", &tela);
        switch(tela)
        {
        case 1:
            buscarPorNome(cvetor, cpos);
            system("pause");
            break;

        case 2:
            buscarPorCPF(cvetor, cpos);
            system("pause");
            break;

        case 9:
            tela = 0;
            printf("VOLTANDO AO MENU PRINCIPAL...");
            Sleep(1000);
            break;
        default:
            printf("NÚMERO INVÁLIDO!!!");
            Sleep(1000);
        }
    }
}
//busca simples de produto por código
void buscarPorCodigo(PRODUTO pvetor[], int ppos)
{
    int codigo;
    int cont;
    int busc = 0;
    printf("DIGITE O CÓDIGO DO PRODUTO:\n");
    scanf("%d", &codigo);
    for(cont=0; cont<ppos; cont++)
    {
        if(codigo == pvetor[cont].codigo)
        {
            printf("PRODUTO CÓDIGO:%04d    VALOR:%.2f REAIS\nDESCRIÇÃO:%s\n", pvetor[cont].codigo, pvetor[cont].valor, pvetor[cont].descricao);
            busc = 1;
        }
    }
    if(busc==0) printf("NENHUM RESULTADO ENCONTRADO!\n");
}
//busca inteligente por descrição ou parte da descrição
void buscarPorDescricao(PRODUTO pvetor[], int ppos)
{
    char desc[N_TAM];
    int cont;
    int busc1, busc2, busc3=0;
    printf("DIGITE A DESCRIÇÃO DO ITEM, OU PARTE DELA:\n");
    fflush(stdin);
    fgets(desc, N_TAM, stdin);
    strupr(desc);
    for(cont=0; cont<ppos; cont++)
    {
        busc2 = 0;
        for(busc1=0; busc1<strlen(pvetor[cont].descricao); busc1++)
        {
            if(desc[busc2] == pvetor[cont].descricao[busc1]) busc2++;
            if(busc2 == strlen(desc))
            {
                printf("PRODUTO CÓDIGO:%04d    VALOR:%.2f REAIS\nDESCRIÇÃO:%s\n", pvetor[cont].codigo, pvetor[cont].valor, pvetor[cont].descricao);
                busc3 = 1;
            }
        }
    }
    if(busc3==0) printf("NENHUM RESULTADO ENCONTRADO!\n");
}
//tela de chamada das opções de busca de produto
void buscarItem(PRODUTO pvetor[], int ppos)
{
    int tela = 1;
    while(tela>0)
    {
        system("cls");
        printf("+++++++++++++++++++++++++++++++++++++\n");
        printf("+++         BUSCAR ITEM           +++\n");
        printf("+++++++++++++++++++++++++++++++++++++\n");
        printf("+++  [1]-BUSCAR POR CÓDIGO        +++\n");
        printf("+++  [2]-BUSCAR POR DESCRIÇÃO     +++\n");
        printf("+++  [9]-SAIR                     +++\n");
        printf("+++++++++++++++++++++++++++++++++++++\n\n");
        printf("DIGITE O NÚMERO DA OPÇÃO DESEJADA:\n");
        scanf("%d", &tela);
        switch(tela)
        {
        case 1:
            buscarPorCodigo(pvetor, ppos);
            system("pause");
            break;

        case 2:
            buscarPorDescricao(pvetor, ppos);
            system("pause");
            break;

        case 9:
            tela = 0;
            printf("VOLTANDO AO MENU PRINCIPAL...");
            Sleep(1000);
            break;
        default:
            printf("NÚMERO INVÁLIDO!!!");
            Sleep(1000);
        }
    }
}
//função completa de geração de nota
void gerarNota(int *npos, CLIENTE cvetor[], int cpos, PRODUTO pvetor[], int ppos)
{
    FILE *notas;
    int indice = (*npos);
    char* extensao = ".dat";
    char arquivo[19];
    snprintf(arquivo, sizeof(arquivo), "notas\\NF-%05d%s", indice, extensao);

    int qtde[NF_QTDE];
    float total = 0.0;
    int cont;
    //escolher cliente da nota
    int cliente;
    int produto;
    printf("DIGITE O NÚMERO DO CLIENTE, OU DIGITE -2 PARA PESQUISAR POR NOME:\n");
    fflush(stdin);
    scanf("%d", &cliente);
    while(cliente<0)
    {
        if(cliente == -2)
        {
            buscarPorNome(cvetor, cpos);
        }
        printf("DIGITE O NÚMERO DO CLIENTE, OU DIGITE -2 PARA PESQUISAR POR NOME:\n");
        fflush(stdin);
        scanf("%d", &cliente);
    }
    system("cls");
    printf("LANÇANDO NOTA PARA O CLIENTE:%s", cvetor[cliente].nome);
    printf("ESCOLHA OS PRODUTOS PARA LANÇAR:\n\n");

    notas = fopen(arquivo, "w+");
    if (notas == NULL)
        printf("OCORREU UM PROBLEMA AO CRIAR O ARQUIVO!");
    else
    {
        //salvar numero da nota
        fprintf(notas,"NOTA FISCAL - Nº:%05d\n", indice);
        //salvar nome do cliente
        if(strlen(cvetor[cliente].id)<=15)
            fprintf(notas,"NOME DO CLIENTE:%s", cvetor[cliente].nome);
        else
            fprintf(notas,"RAZÃO SOCIAL DO CLIENTE:%s", cvetor[cliente].nome);
        //salvar cpf do cliente
        if(strlen(cvetor[cliente].id)<=15)
            fprintf(notas,"CPF:%s", cvetor[cliente].id);
        else
            fprintf(notas,"CNPJ:%s", cvetor[cliente].id);

        fprintf(notas,"\n\n+++++++++++++++++++++++++++++++++++++++\n\n");
    }
    fclose(notas);
    //escolher produtos para a nota
    for(cont=0; cont<NF_QTDE; cont++)
    {
        printf("DIGITE O CÓDIGO DO PRODUTO, OU DIGITE -1 PARA FECHAR NOTA\nCASO NÃO LEMBRE O CÓDIGO DO PRODUTO, DIGITE -2 PESQUISAR POR DESCRIÇÃO:\n");
        fflush(stdin);
        scanf("%d", &produto);
        if(produto==-1) break;

        notas = fopen(arquivo, "a+");
        if (notas == NULL)
            printf("OCORREU UM PROBLEMA AO LER O ARQUIVO!");
        else
        {
            while(produto<0)
            {
                if(produto == -2)
                {
                    //opção de busca dentro da opção de gerar nota
                    buscarPorDescricao(pvetor, ppos);
                }
                printf("DIGITE O CÓDIGO DO PRODUTO\nCASO NÃO LEMBRE O CÓDIGO DO PRODUTO, DIGITE -2 PESQUISAR POR DESCRIÇÃO:\n");
                fflush(stdin);
                scanf("%d", &produto);
            }
            printf("VOCÊ ESCOLHEU %s", pvetor[produto].descricao);
            //solicitação da quantidade ao usuário
            printf("DIGITE A QUANTIDADE:\n");
            scanf("%d", &qtde[cont]);
            //geração dos valores na nota
            fprintf(notas,"CÓDIGO DO PRODUTO:%04d\n", pvetor[produto].codigo);
            fprintf(notas,"     DESCRIÇÃO:%s", pvetor[produto].descricao);
            fprintf(notas,"     VALOR UNITÁRIO:%.2f\n", pvetor[produto].valor);
            fprintf(notas,"     QUANTIDADE:%02d\n", qtde[cont]);
            fprintf(notas,"     VALOR TOTAL DO PRODUTO:%.2f\n\n", (pvetor[produto].valor)*(qtde[cont]));
            total += ((pvetor[produto].valor)*(qtde[cont]));
        }
        fclose(notas);
        //se atingiu limite da nota, fecha a nota
        if(cont==(NF_QTDE-1))
        {
            printf("LIMITE DE PRODUTOS POR NOTA ATINGIDO.\nFECHANDO NOTA...");
        }
        //se não, exibe uma parcial e continua a solicitação de entrada de produtos
        else
        {
            system("cls");
            printf("NOTA PARCIAL!!!");
            char leitura[30];
            notas = fopen(arquivo, "r+");
            if (notas == NULL)
                printf("OCORREU UM PROBLEMA AO LER O ARQUIVO!");
            else
            {
                while (!feof(notas))
                {
                    fgets(leitura,30,notas);
                    if(ferror(notas))
                        printf("ERRO NA LEITURA!");
                    else
                        printf("%s", leitura);
                }
            }
            printf("TOTAL DA NOTA FISCAL: %.2f\n\n", total);
            fclose(notas);
        }

    }
    //inserir rodapé na nota com informações finais
    notas = fopen(arquivo, "a+");
    if (notas == NULL)
        printf("OCORREU UM PROBLEMA AO LER O ARQUIVO!");
    else
    {
        fprintf(notas,"\n+++++++++++++++++++++++++++++++++++++++\n");
        fprintf(notas, "TOTAL DA NOTA FISCAL: %.2f", total);
        fprintf(notas,"\n+++++++++++++++++++++++++++++++++++++++\n\n");
    }
    fclose(notas);

    (*npos)++;
    system("pause");
    system("cls");

}
//exibir na tela uma nota salva no HD
void imprimirNota()
{
    FILE *notas;
    int indice;
    char* extensao = ".dat";
    char arquivo[19];
    char leitura[30];

    printf("DIGITE O NÚMERO DA NOTA QUE DESEJA EXIBIR:\n");
    fflush(stdin);
    scanf("%d", &indice);
    snprintf(arquivo, sizeof(arquivo), "notas\\NF-%05d%s", indice, extensao);

    system("cls");
    notas = fopen(arquivo, "r+");
    if (notas == NULL)
        printf("ESTA NOTA FISCAL NÃO EXISTE!\n");
    else
    {
        while (!feof(notas))
        {
            fgets(leitura,30,notas);
            if(ferror(notas))
                printf("ERRO NA LEITURA!");
            else
                printf("%s", leitura);
        }
    }
    fclose(notas);
    system("pause");
    system("cls");
}
//função main
int main()
{
    setlocale(LC_ALL, "portuguese");
    int ic = 0;                      //indice de clientes
    int ip = 0;                      //indice de produtos
    int in = 0;                      //indice de notas
    CLIENTE cliente[C_QTDE];
    PRODUTO produto[P_QTDE];
    //criar pastas de registros e notas fiscais
    mkdir("registros");
    mkdir("notas");

    //carregar dados dos arquivos
    carregarDados(cliente, &ic, produto, &ip, &in);

    int tela = 1;
    while(tela>0)
    {
        printf("+++++++++++++++++++++++++++++++++++++\n");
        printf("+++       REPOLHO'S COMPANY       +++\n");
        printf("+++++++++++++++++++++++++++++++++++++\n");
        printf("+++  [1]-CADASTRAR NOVO CLIENTE   +++\n");
        printf("+++  [2]-CADASTRAR NOVO ITEM      +++\n");
        printf("+++  [3]-BUSCAR CLIENTE           +++\n");
        printf("+++  [4]-BUSCAR ITEM              +++\n");
        printf("+++  [5]-GERAR NOTA FISCAL        +++\n");
        printf("+++  [6]-IMPRIMIR NOTA FISCAL     +++\n");
        printf("+++  [9]-SAIR                     +++\n");
        printf("+++++++++++++++++++++++++++++++++++++\n");
        printf("+++        BY PAULO COSTA         +++\n");
        printf("+++++++++++++++++++++++++++++++++++++\n\n");
        printf("DIGITE O NÚMERO DA OPÇÃO DESEJADA:\n");
        scanf("%d", &tela);
        switch(tela)
        {
        case 1:
            cadastrarCliente(cliente, &ic);
            break;

        case 2:
            cadastrarItem(produto, &ip);
            break;

        case 3:
            if(ic==0)
            {
                printf("NÃO EXISTEM CLIENTES CADASTRADOS.\n");
                system("pause");
            }
            else buscarCliente(cliente, ic);
            break;

        case 4:
            if(ip==0)
            {
                printf("NÃO EXISTEM PRODUTOS CADASTRADOS.\n");
                system("pause");
            }
            else buscarItem(produto, ip);
            break;

        case 5:
            if(ic==0)
            {
                printf("NÃO EXISTEM CLIENTES CADASTRADOS.\n");
                system("pause");
            }
            else if(ip==0)
            {
                printf("NÃO EXISTEM PRODUTOS CADASTRADOS.\n");
                system("pause");
            }
            else gerarNota(&in, cliente, ic, produto, ip);
            break;

        case 6:
            if(in==0)
            {
                printf("NÃO EXISTEM NOTAS LANÇADAS.\n");
                system("pause");
            }
            else imprimirNota();
            break;

        case 9:
            tela = 0;
            printf("ENCERRANDO SISTEMA...");
            Sleep(2000);
            break;

        default:
            printf("NÚMERO INVÁLIDO!!!");
            Sleep(1000);
        }
        system("cls");
    }

    return 0;
}
