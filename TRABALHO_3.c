#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TAM 50

struct MESES
{
    int DIA[31][2];
};

struct REG_FUNC
{
    int CODIGO, ANO, FLAG;
    float VALOR_HORA;
    char NOME[30], CARGO[15];
    struct MESES MES[12];
}FUNCIONARIO[TAM];

void INICIALIZA_HORAS(FILE *PRINCIPAL)
{
    char LINHA[30];
    int DIA, MES, HORAS, COD;
    PRINCIPAL = fopen("Funcionarios.txt","r");
    while (fgets(LINHA, sizeof(LINHA), PRINCIPAL) != NULL)
    {
        if (sscanf(LINHA, "Codigo: %s", LINHA)==1)
        {
            COD = atoi(LINHA);
        }
        else if(sscanf(LINHA, "%d/%d: %d horas trabalhadas.", &DIA, &MES, &HORAS) == 3)
        {
            MES--;
            DIA--;
            FUNCIONARIO[COD].MES[MES].DIA[DIA][1]=HORAS;
            FUNCIONARIO[COD].MES[MES].DIA[DIA][0]=2;
        }
    }
    fclose(PRINCIPAL);
}

int ADD_HORAS_ARQUIVO(FILE *PRINCIPAL, int POS_FUNC, int MES, int DIA)
{
    FILE *TEMPORARIO;
    int COD, POS_LINHA=1, LINHA_TEMP=1;
    char LINHA[100];
    PRINCIPAL = fopen("Funcionarios.txt","r");
    TEMPORARIO = fopen("temporario.txt","w");
    if (TEMPORARIO == NULL)
    {
        printf("ERRO AO CRIAR ARQUIVO TEMPORARIO!\n");
        return 0;
    }
    while (fgets(LINHA, sizeof(LINHA), PRINCIPAL) != NULL)
    {
        if (sscanf(LINHA, "Codigo: %s", LINHA) == 1)
        {
            COD = atoi(LINHA);
            if (COD==POS_FUNC)
            {
                POS_LINHA+=5;
                break;
            }
        }
        POS_LINHA++;
    }
    rewind(PRINCIPAL);
    printf("%d\n",COD);
    while ((fgets(LINHA, sizeof(LINHA), PRINCIPAL) != NULL)&&(LINHA_TEMP!=POS_LINHA))
    {
        fprintf(TEMPORARIO, "%s", LINHA);
        LINHA_TEMP++;
    }
    fprintf(TEMPORARIO,"%02d/%02d: %d horas trabalhadas.\n",DIA+1,MES+1,FUNCIONARIO[POS_FUNC].MES[MES].DIA[DIA][1]);
    POS_LINHA = 1;
    rewind(PRINCIPAL);
    while (fgets(LINHA, sizeof(LINHA), PRINCIPAL) != NULL)
    {
        if (POS_LINHA>=LINHA_TEMP)
        {
            fprintf(TEMPORARIO, "%s", LINHA);
        }
        POS_LINHA++;
    }
    fclose(PRINCIPAL);
    fclose(TEMPORARIO);
    remove("Funcionarios.txt");
    rename("temporario.txt", "Funcionarios.txt");
    return 1;
}

int INICIALIZA_FUNCIONARIO(FILE *ARQ)
{
    int C, CODIGO, CONT = 0;
    ARQ = fopen("Funcionarios.txt","r");
    if (ARQ==NULL)
    {
        printf("Arquivo inexistente!\n");
        return 1;
    }
    else
    {
        CODIGO = ATRIBUI_CADASTRADO(ARQ);
        for(C=0;C<=CODIGO;C++)
        {
            FUNCIONARIO[C].FLAG = 1;
        }
        CODIGO++;
        for(C=CODIGO;C<TAM;C++)
        {
            FUNCIONARIO[C].FLAG = 0;
        }
        fclose(ARQ);
        return 0;
    }
}

int ATRIBUI_CADASTRADO(FILE *ARQ)
{
    char LINHA[100];
    int COD=-1, N;
    while (fgets(LINHA, sizeof(LINHA), ARQ) != NULL)
    {
        if (sscanf(LINHA, "Codigo: %s", LINHA) == 1)
        {
            COD = atoi(LINHA);
            FUNCIONARIO[COD].CODIGO = COD;
            fgets(LINHA, sizeof(LINHA), ARQ);
        }
        if (sscanf(LINHA, "Nome: %[^\n]", LINHA) == 1)
        {
           strcpy(FUNCIONARIO[COD].NOME,LINHA);
           fgets(LINHA, sizeof(LINHA), ARQ);
        }
        if (sscanf(LINHA, "Ano: %[^\n]", LINHA) == 1)
        {
            N = atoi(LINHA);
            FUNCIONARIO[COD].ANO = N;
            fgets(LINHA, sizeof(LINHA), ARQ);
        }
        if (sscanf(LINHA, "Cargo: %[^\n]", LINHA) == 1)
        {
           strcpy(FUNCIONARIO[COD].CARGO,LINHA);
           fgets(LINHA, sizeof(LINHA), ARQ);
        }
        if (sscanf(LINHA, "Valor da hora trabalhada: %s", LINHA) == 1)
        {
            N = atof(LINHA);
            FUNCIONARIO[COD].VALOR_HORA = N;
            fgets(LINHA, sizeof(LINHA), ARQ);
        }
    }
    return COD;
}

int VERIFICA_BISSEXTO(int IND_FUNC)
{
    int YEAR = FUNCIONARIO[IND_FUNC].ANO;
    if (((YEAR%4==0)&&(YEAR%100!=0))||(YEAR%400==0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void INICIALIZA_DIA(int IND_MES, int INDICE_FUNC, int V_MAX)
{
    int C;
    for (C=0;C<V_MAX;C++)
    {
        if (FUNCIONARIO[INDICE_FUNC].MES[IND_MES].DIA[C][0]!=2)
        {
            FUNCIONARIO[INDICE_FUNC].MES[IND_MES].DIA[C][0] = 0;
        }
    }
    if (V_MAX<31)
    {
        FUNCIONARIO[INDICE_FUNC].MES[IND_MES].DIA[30][0] = 1;
    }
}

int INICIALIZA_MES(int INDICE_FUNC)
{
    int INDICE_MES, C, BI6, VALID;
    do
    {
        printf("Digite o numero do mes em que quer adicionar horas:\n");
        scanf("%d",&INDICE_MES);
        INDICE_MES--;
        if ((INDICE_MES<7)&&(INDICE_MES>=0))
        {
            if (INDICE_MES%2==0)
            {
                INICIALIZA_DIA(INDICE_MES,INDICE_FUNC,31);
            }
            else
            {
                INICIALIZA_DIA(INDICE_MES,INDICE_FUNC,30);
            }
            if (INDICE_MES == 1)
            {
                FUNCIONARIO[INDICE_FUNC].MES[INDICE_MES].DIA[29][0] = 1;
                BI6 = VERIFICA_BISSEXTO(INDICE_FUNC);
                if (BI6 == 1)
                {
                    if (FUNCIONARIO[INDICE_FUNC].MES[INDICE_MES].DIA[C][0]!=2)
                    {
                        FUNCIONARIO[INDICE_FUNC].MES[INDICE_MES].DIA[28][0] = 0;
                    }
                }
                else
                {
                        FUNCIONARIO[INDICE_FUNC].MES[INDICE_MES].DIA[28][0] = 1;
                }
            }
            VALID = 0;
        }
        else if ((INDICE_MES<12)&&(INDICE_MES>=7))
        {
            if (INDICE_MES%2==0)
            {
                INICIALIZA_DIA(INDICE_MES,INDICE_FUNC,30);

            }
            else
            {
                INICIALIZA_DIA(INDICE_MES,INDICE_FUNC,31);
            }
            VALID = 0;
        }
        else
        {
            printf("Mes invalido! Digite um valor de 1 a 12.\n");
            VALID = 1;
        }
    }while(VALID!=0);
    return INDICE_MES;
}

int PROCURAR()
{
    char NAME[30];
    int C;
    do
    {
        printf("Digite o nome do funcionario:\n");
        fflush(stdin);
        gets(NAME);
        for (C=0;C<TAM;C++)
        {
            if ((strcmp(FUNCIONARIO[C].NOME,NAME)==0)&&(FUNCIONARIO[C].FLAG == 1))
            {
                return C;
            }
        }
        printf("Nao foi possivel encontrar o nome digitado na lista de funcionarios cadastrados. Digite um nome ja cadastrado.\n");
    }while(C!=-1);
}

int POS_LIVRE()
{
    int CONT=0;
    while ((FUNCIONARIO[CONT].FLAG == 1)&&(CONT < TAM))
    {
        CONT++;
    }
    return CONT;
}

void CADASTRAR(FILE *ARQUIVO)
{
    int C = 0, VALID = 0;
    C = POS_LIVRE();
    FUNCIONARIO[C].CODIGO = C;
    printf("Digite o nome do funcionario:\n");
    fflush(stdin);
    gets(FUNCIONARIO[C].NOME);
    do
    {
        printf("Digite o ano:\n");
        scanf("%d", &FUNCIONARIO[C].ANO);
    }while(VALIDA_DADO(1, FUNCIONARIO[C].ANO, 10000)!=0);
    printf("Digite o cargo ocupado pelo funcionario:\n");
    fflush(stdin);
    gets(FUNCIONARIO[C].CARGO);
    do
    {
        printf("Digite o valor em reais que o funcionario ganha por hora trabalhada:\n");
        scanf("%f", &FUNCIONARIO[C].VALOR_HORA);
        if (FUNCIONARIO[C].VALOR_HORA<0)
        {
            printf("Valor invalido digitado para hora! Digite um valor maior que 0\n");
            VALID = 1;
        }
        else
        {
            VALID = 0;
        }
    }while(VALID!=0);
    FUNCIONARIO[C].FLAG = 1;
    SALVA_CADASTRO(ARQUIVO, C);
    printf("Funcionario cadastrado com sucesso!\n");
}

void SALVA_CADASTRO(FILE *ARQUIVO, int POS_FUNC)
{
    ARQUIVO = fopen("Funcionarios.txt","a");
    fprintf(ARQUIVO,"-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
    fprintf(ARQUIVO,"Codigo: %02d\n",FUNCIONARIO[POS_FUNC].CODIGO);
    fprintf(ARQUIVO,"Nome: %s\n",FUNCIONARIO[POS_FUNC].NOME);
    fprintf(ARQUIVO,"Ano: %d\n",FUNCIONARIO[POS_FUNC].ANO);
    fprintf(ARQUIVO,"Cargo: %s\n",FUNCIONARIO[POS_FUNC].CARGO);
    fprintf(ARQUIVO,"Valor da hora trabalhada: %.2f\n",FUNCIONARIO[POS_FUNC].VALOR_HORA);
    fprintf(ARQUIVO,"-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
    fclose(ARQUIVO);
}

void PRINT_DIAS(int INDICE_MES, int INDICE_FUNC)
{
    int C;
    printf("\nDias disponiveis:\n\n");
    switch(INDICE_MES)
    {
        case 0:
            printf("\t\t     JANEIRO\n\n");
            break;
        case 1:
            printf("\t\t    FEVEREIRO\n\n");
            break;
        case 2:
            printf("\t\t      MARCO\n\n");
            break;
        case 3:
            printf("\t\t      ABRIL\n\n");
            break;
        case 4:
            printf("\t\t      MAIO\n\n");
            break;
        case 5:
            printf("\t\t      JUNHO\n\n");
            break;
        case 6:
            printf("\t\t      JULHO\n\n");
            break;
        case 7:
            printf("\t\t     AGOSTO\n\n");
            break;
        case 8:
            printf("\t\t    SETEMBRO\n\n");
            break;
        case 9:
            printf("\t\t     OUTUBRO\n\n");
            break;
        case 10:
            printf("\t\t    NOVEMBRO\n\n");
            break;
        case 11:
            printf("\t\t    DEZEMBRO\n\n");
            break;
    }
    for (C=0;C<31;C++)
    {
        if (FUNCIONARIO[INDICE_FUNC].MES[INDICE_MES].DIA[C][0]==0)
        {
            printf("%d\t",C+1);
        }
        if (((C+1)%7==0))
        {
            printf("\n");
        }
    }
    printf("\n");
}

int VALIDA_DADO(int LIM_INF, int DADO, int LIM_SUP)
{
    if ((DADO<LIM_INF)||(DADO>LIM_SUP))
    {
        printf("Valor invalido digitado! Digite um valor maior que %d e menor que %d.\n", LIM_INF, LIM_SUP);
        return 1;
    }
    else
    {
        return 0;
    }
}

void ESCOLHE_DIA(int IND_MES, int IND_FUNC, FILE *ARQ_PRINC)
{
    int DAY, VALID;
    PRINT_DIAS(IND_MES,IND_FUNC);
    do
    {
        printf("\nDia desejado: ");
        scanf("%d",&DAY);
        DAY--;
        if (FUNCIONARIO[IND_FUNC].MES[IND_MES].DIA[DAY][0]==2)
        {
            printf("Nesse dia ja foram cadastradas horas. Caso queira altera-las, voltar ao menu principal e selecionar a opcao alterar.\n");
            VALID = 1;
        }
        else if (FUNCIONARIO[IND_FUNC].MES[IND_MES].DIA[DAY][0]==1)
        {
            printf("Dia invalido digitado. Digite um dos dias existentes no mes selecionado.\n");
            VALID = 1;
        }
        else
        {
            VALID = 0;
        }
    }while(VALID!=0);
    do
    {
        printf("Digite o numero de horas trabalhadas no dia %d: ",DAY+1);
        scanf("%d",&FUNCIONARIO[IND_FUNC].MES[IND_MES].DIA[DAY][1]);
    }while(VALIDA_DADO(1,FUNCIONARIO[IND_FUNC].MES[IND_MES].DIA[DAY][1],24)!=0);
    ADD_HORAS_ARQUIVO(ARQ_PRINC, IND_FUNC, IND_MES, DAY);
    FUNCIONARIO[IND_FUNC].MES[IND_MES].DIA[DAY][0] = 2;
}

int SALVA_ALTERACAO(FILE *PRINCIPAL, int IND_FUNC, int ESCOLHA)
{
    FILE *SECUNDARIO;
    int COD, N;
    char LINHA[100];
    PRINCIPAL = fopen("Funcionarios.txt","r");
    SECUNDARIO = fopen("temporario.txt","w");
    if (SECUNDARIO == NULL)
    {
        printf("ERRO AO CRIAR ARQUIVO TEMPORARIO!\n");
        return 0;
    }
    while (fgets(LINHA, sizeof(LINHA), PRINCIPAL) != NULL)
    {
        if (sscanf(LINHA, "Codigo: %d", &COD)==1)
        {
            printf("%d\n",COD);
        }
        if (COD==IND_FUNC)
        {
            switch(ESCOLHA)
            {
                case 1:
                {
                    if(sscanf(LINHA, "Nome: %s", LINHA)==1)
                    {
                        fprintf(SECUNDARIO,"Nome: %s\n",FUNCIONARIO[IND_FUNC].NOME);
                    }
                    else
                    {
                        fprintf(SECUNDARIO,"%s",LINHA);
                    }
                    break;
                }
                case 2:
                {
                    if(sscanf(LINHA, "Cargo: %s", LINHA)==1)
                    {
                        fprintf(SECUNDARIO,"Cargo: %s\n",FUNCIONARIO[IND_FUNC].CARGO);
                    }
                    else
                    {
                        fprintf(SECUNDARIO,"%s",LINHA);
                    }
                    break;
                }
                case 3:
                {
                    if(sscanf(LINHA, "Ano: %s", LINHA)==1)
                    {
                        fprintf(SECUNDARIO,"Ano: %d\n",FUNCIONARIO[IND_FUNC].ANO);
                    }
                    else
                    {
                        fprintf(SECUNDARIO,"%s",LINHA);
                    }
                    break;
                }
                case 4:
                {
                    if(sscanf(LINHA, "Valor da hora trabalhada: %s", LINHA)==1)
                    {
                        fprintf(SECUNDARIO,"Valor da hora trabalhada: %.2f\n",FUNCIONARIO[IND_FUNC].VALOR_HORA);
                    }
                    else
                    {
                        fprintf(SECUNDARIO,"%s",LINHA);
                    }
                    break;
                }
            }
        }
        else
        {
            fprintf(SECUNDARIO,"%s",LINHA);
        }
    }
    fclose(PRINCIPAL);
    fclose(SECUNDARIO);
    remove("Funcionarios.txt");
    rename("temporario.txt", "Funcionarios.txt");
    return 1;
}

int SALVA_ALTERACAO_HORA(FILE *PRINCIPAL, int MONTH, int DAY, int IND_FUNC)
{
    FILE *SECUNDARIO;
    int COD, DIA, MES;
    char LINHA[100];
    PRINCIPAL = fopen("Funcionarios.txt","r");
    SECUNDARIO = fopen("temporario.txt","w");
    if (SECUNDARIO == NULL)
    {
        printf("ERRO AO CRIAR ARQUIVO TEMPORARIO!\n");
        return 0;
    }
    while (fgets(LINHA, sizeof(LINHA), PRINCIPAL) != NULL)
    {
        if (sscanf(LINHA, "Codigo: %d", &COD) == 1)
        {

        }
        if (COD==IND_FUNC)
        {
            if(sscanf(LINHA, "%d/%d: %s.", &DIA, &MES, LINHA) == 3)
            {
                DAY++;
                MONTH++;
                DIA--;
                MES--;
                fprintf(SECUNDARIO,"%02d/%02d: %d horas trabalhadas.\n",DAY,MONTH,FUNCIONARIO[IND_FUNC].MES[MES].DIA[DIA][1]);
            }
            else
            {
                fprintf(SECUNDARIO,"%s",LINHA);
            }
        }
        else
        {
            fprintf(SECUNDARIO,"%s",LINHA);
        }
    }
    fclose(PRINCIPAL);
    fclose(SECUNDARIO);
    remove("Funcionarios.txt");
    rename("temporario.txt", "Funcionarios.txt");
}

void ALTERAR(int IND_FUNC, FILE *PRINC)
{
    int RESP, MONTH, C, DAY, N_DIAS;
    do
    {
        printf("Escolha o dado a ser alterado do funcionario %d\n",IND_FUNC);
        printf("[1] NOME\n[2] ANO\n[3] CARGO\n[4] VALOR DA HORA TRABALHADA\n");
        printf("[5] QUANTIDADE DE HORAS TRABALHADAS EM UM DIA DETERMINADO\nSua resposta = ");
        scanf("%d",&RESP);
        switch(RESP)
        {
            case 1:
                printf("Digite o novo nome do funcionario %d:\n",IND_FUNC);
                fflush(stdin);
                gets(FUNCIONARIO[IND_FUNC].NOME);
                SALVA_ALTERACAO(PRINC, IND_FUNC, 1);
                break;
            case 2:
                do
                {
                    printf("Digite o novo ano do funcionario %d:\n",IND_FUNC);
                    scanf("%d",&FUNCIONARIO[IND_FUNC].ANO);
                }while(VALIDA_DADO(1,FUNCIONARIO[IND_FUNC].ANO,10000)!=0);
                SALVA_ALTERACAO(PRINC, IND_FUNC, 3);
                break;
            case 3:
                printf("Digite o novo cargo do funcionario %d:\n",IND_FUNC);
                fflush(stdin);
                gets(FUNCIONARIO[IND_FUNC].CARGO);
                SALVA_ALTERACAO(PRINC, IND_FUNC, 2);
                break;
            case 4:
                printf("Digite o novo valor da hora trabalhada para o funcionario %d:\n", IND_FUNC);
                scanf("%f",&FUNCIONARIO[IND_FUNC].VALOR_HORA);
                SALVA_ALTERACAO(PRINC, IND_FUNC, 4);
                break;
            case 5:
                N_DIAS = 0;
                do
                {
                    printf("Digite o numero do mes em que esta o dia a serem alteradas as horas trabalhadas:\n");
                    scanf("%d",&MONTH);
                }while(VALIDA_DADO(1,MONTH,12)!=0);
                MONTH--;
                printf("\n");
                for(C=0;C<31;C++)
                {
                    if(FUNCIONARIO[IND_FUNC].MES[MONTH].DIA[C][0]==2)
                    {
                        printf("%d\t",C+1);
                        N_DIAS++;
                    }
                }
                printf("\n\n");
                if(N_DIAS>0)
                {
                    do
                    {
                        printf("Os dias acima sao aqueles nos quais ja foram cadastradas horas do funcionario no mes %d.\n",MONTH+1);
                        printf("Digite em qual deles gostaria de alterar o numero de horas cadastradas:\n");
                        scanf("%d",&DAY);
                        DAY--;
                        if (FUNCIONARIO[IND_FUNC].MES[MONTH].DIA[DAY][0]!=2)
                        {
                            printf("Nesse dia nao foram cadastradas horas.\n");
                        }
                    }while(FUNCIONARIO[IND_FUNC].MES[MONTH].DIA[DAY][0]!=2);
                    printf("Digite a quantidade de horas trabalhadas no dia %d:\n",DAY+1);
                    scanf("%d",&FUNCIONARIO[IND_FUNC].MES[MONTH].DIA[DAY][1]);
                    SALVA_ALTERACAO_HORA(PRINC, MONTH, DAY, IND_FUNC);
                }
                else
                {
                    printf("Em nenhum dia desse mes foram cadastradas horas. \nPara fazer a alteracao, ja devem ter sido inseridas horas no dia em questao\n");
                }
                break;
            default:
                printf("OPCAO INVALIDA, DIGITE UM NUMERO DE 1 A 6\n");
                break;
        }
        if ((RESP >= 1)&&(RESP<=6))
            printf("Alteracao realizada com sucesso!\n");
            printf("Deseja alterar mais algum dado desse funcionario?\n[1] SIM\n[2] NAO\nSua resposta = ");
        scanf("%d",&RESP);
        if ((RESP!=1)&&(RESP!=2))
        {
            printf("VALOR INVALIDO DIGITADO, DIGITE 1 OU 2!\n");
        }
    }while(RESP!=2);
}

int SALVA_EXCLUIR(FILE *PRINC, int IND_FUNC)
{
    FILE *SEC;
    char LINHA[100];
    int CONT1=1, CONT2=1, COD;
    PRINC = fopen("Funcionarios.txt","r");
    SEC = fopen("temporario.txt","w");
    if (SEC == NULL)
    {
        printf("ERRO AO CRIAR ARQUIVO TEMPORARIO!\n");
        return 0;
    }
    while (fgets(LINHA, sizeof(LINHA), PRINC) != NULL)
    {
        if(sscanf(LINHA, "Codigo: %d", &COD) == 1)
        {

        }
        if(COD==IND_FUNC)
        {
            CONT2++;
        }
        else if(CONT2==1)
        {
            CONT1++;
        }
    }
    CONT1--;
    CONT2--;
    CONT2+=CONT1;
    COD = 1;
    rewind(PRINC);
    while (fgets(LINHA, sizeof(LINHA), PRINC) != NULL)
    {
        COD++;
        if((COD<CONT1)||(COD>CONT2))
        {
            fprintf(SEC,"%s",LINHA);
        }
    }
    fclose(PRINC);
    fclose(SEC);
    remove("Funcionarios.txt");
    rename("temporario.txt", "Funcionarios.txt");
    return 1;
}

void EXCLUIR(int IND_FUNC, FILE *ARQ)
{
    FUNCIONARIO[IND_FUNC].FLAG = 0;
    SALVA_EXCLUIR(ARQ, IND_FUNC);
    printf("Funcionario excluido com sucesso!\n");
}

int SALVA_SALARIO(float SALARIO ,FILE *PRINCIPAL, int IND_FUNC, int MES)
{
    FILE *TEMPORARIO;
    char LINHA[100];
    int COD, POS_LIN=1, MONTH;
    TEMPORARIO = fopen("temporario.txt","w");
    PRINCIPAL = fopen("Funcionarios.txt","r");
    if (TEMPORARIO == NULL)
    {
        printf("ERRO AO CRIAR ARQUIVO TEMPORARIO!\n");
        return 0;
    }
    while (fgets(LINHA, sizeof(LINHA), PRINCIPAL) != NULL)
    {
        if (sscanf(LINHA, "Codigo: %s", LINHA) == 1)
        {
            COD = atoi(LINHA);
            if (COD == IND_FUNC)
            {
                fprintf(TEMPORARIO,"Codigo: %s\n",LINHA);
                POS_LIN++;
                while(fgets(LINHA, sizeof(LINHA), PRINCIPAL) != NULL)
                {
                    if (sscanf(LINHA, "-*-*-*-*-*-*-*-%s",LINHA) == 1)
                    {
                        fprintf(TEMPORARIO,"Salario no mes %d: %.2f\n",MES+1,SALARIO);
                        break;
                    }
                    else if (sscanf(LINHA, "Salario no mes %d:",&MONTH) == 1)
                    {
                        if (MONTH == MES+1)
                        {

                        }
                        else
                        {
                            fprintf(TEMPORARIO, "%s",LINHA);
                        }
                    }
                    else
                    {
                        fprintf(TEMPORARIO,"%s",LINHA);
                    }
                    POS_LIN++;
                }
                break;
            }
            fprintf(TEMPORARIO,"Codigo: %s\n",LINHA);
            POS_LIN++;
        }
        else
        {
            fprintf(TEMPORARIO,"%s",LINHA);
            POS_LIN++;
        }
    }
    COD = 1;
    rewind(PRINCIPAL);
    while (fgets(LINHA, sizeof(LINHA), PRINCIPAL) != NULL)
    {
        if(COD>=POS_LIN)
        {
            fprintf(TEMPORARIO,"%s",LINHA);
        }
        COD++;
    }
    fclose(PRINCIPAL);
    fclose(TEMPORARIO);
    remove("Funcionarios.txt");
    rename("temporario.txt", "Funcionarios.txt");
}

void CALCULA_SALARIO(int IND_FUNC, FILE *ARQUIVO)
{
    int MONTH, C;
    float SAL=0;
    printf("Digite o mes em que deseja calcular o salario do funcionario %d:\n",IND_FUNC);
    scanf("%d",&MONTH);
    MONTH--;
    for(C=0;C<31;C++)
    {
        if (FUNCIONARIO[IND_FUNC].MES[MONTH].DIA[C][0] == 2)
        {
            SAL = SAL + ((float)FUNCIONARIO[IND_FUNC].MES[MONTH].DIA[C][1] * (float)FUNCIONARIO[IND_FUNC].VALOR_HORA);
        }
    }
    printf("Salario do funcionario %d no mes %d: %.2f reais\n",IND_FUNC,MONTH+1,SAL);
    SALVA_SALARIO(SAL, ARQUIVO, IND_FUNC, MONTH);
}

void LISTAR(FILE *FUNCS)
{
    char LINHA[100];
    FUNCS = fopen("Funcionarios.txt","r");
    while (fgets(LINHA, sizeof(LINHA), FUNCS) != NULL)
    {
        printf("%s",LINHA);
    }
    fclose(FUNCS);
}

int main()
{
    int OPCAO, POS_FUNCIONARIO, IND_MES, ARQUIVO_EXISTE;
    FILE *REGISTRO_FUNCIONARIOS;
    ARQUIVO_EXISTE = INICIALIZA_FUNCIONARIO(REGISTRO_FUNCIONARIOS);
    INICIALIZA_HORAS(REGISTRO_FUNCIONARIOS);
    if (ARQUIVO_EXISTE==0)
    {
        do
        {
            printf("=*=*=*=*=*=*=*=*=*=*=*=*=*=\n");
            printf("\t   MENU\n");
            printf("=*=*=*=*=*=*=*=*=*=*=*=*=*=\n");
            printf("Digite o que deseja fazer:\n");
            printf("[1] CADASTRAR FUNCIONARIO\n[2] INSERIR HORAS TRABALHADAS\n[3] CALCULAR SALARIO\n");
            printf("[4] LISTAR FUNCIONARIOS\n[5] ALTERAR INFORMACOES DO FUNCIONARIO\n");
            printf("[6] EXCLUIR FUNCIONARIO\n[7] SAIR\nSua resposta = ");
            scanf("%d",&OPCAO);
            switch(OPCAO)
            {
                case 1:
                    CADASTRAR(REGISTRO_FUNCIONARIOS);
                    break;
                case 2:
                    POS_FUNCIONARIO = PROCURAR();
                    IND_MES = INICIALIZA_MES(POS_FUNCIONARIO);
                    ESCOLHE_DIA(IND_MES, POS_FUNCIONARIO, REGISTRO_FUNCIONARIOS);
                    break;
                case 3:
                    POS_FUNCIONARIO = PROCURAR();
                    CALCULA_SALARIO(POS_FUNCIONARIO, REGISTRO_FUNCIONARIOS);
                    break;
                case 4:
                    LISTAR(REGISTRO_FUNCIONARIOS);
                    break;
                case 5:
                    POS_FUNCIONARIO = PROCURAR();
                    ALTERAR(POS_FUNCIONARIO, REGISTRO_FUNCIONARIOS);
                    break;
                case 6:
                    POS_FUNCIONARIO = PROCURAR();
                    EXCLUIR(POS_FUNCIONARIO, REGISTRO_FUNCIONARIOS);
                    break;
                case 7:
                    printf("Ate mais, volte sempre!\n");
                    break;
                default:
                    printf("OPCAO INVALIDA, DIGITE UM VALOR DE 1 A 7!\n");
            }
        }while(OPCAO!=7);
    }
    system("pause");
    return 0;
}
