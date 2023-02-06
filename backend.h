enum tamanho {tam = 1024, vetor = 11, vetor2 = 10, vetor3 = 60};

////ESTRUTURAS////////////////
typedef struct
{
    char *nome;
} endereco;
////ESTRUTURAS///////////////
////FUNÇÕES//////////////////

////FUNÇÕES//////////////////
//////VARIÁVEIS///////////////
unsigned int contador3 = 0, contando = 0, contagemArq = 0, ptrAcres = 0, ptrAcresAuxiliar = 0;
unsigned int *pcontando = &contando;
endereco *ptrBusca = NULL, *lista = NULL;
char buffer[tam] = {0}, buffPaCam2[tam] = {0};
bool loop = FALSE, loop2 = FALSE, NoMatch = FALSE, NoMatch2 = FALSE, NoMatch3 = FALSE;
HWND hwndListCx;
////VARIÁVEIS/////////////////

////PROTÓTIPOS DE FUNÇÕES
void concatena(unsigned char *conteudo, unsigned short quantCharsLidos);
void concatena2(unsigned char *conteudo, unsigned short quantCharsLidos);
int tamCaminho(unsigned char *chaveDeBusca);
void caminho(endereco* lista, unsigned char *chaveDeBusca);
void caminho2(void);
void limpezaPtrBusca(void);
void limpezaLista(void);
void limpezaListaTemp(void);
void lerPDF(void);
unsigned int testaCaminho2(void);
unsigned int testaCaminho3(void);
void acentuacao(unsigned char *ler,unsigned short ta);
void acentuacao2(unsigned char *ler,unsigned short ta);
void acentuacao3(unsigned char *ler, unsigned short ta);
void cxAlta(unsigned char *ler,unsigned short ta);
////PROTÓTIPOS DE FUNÇÕES

////RETORNA ARQUIVOS DO DIRETORIO INFORMADO/////////////////////
/////////////retorna o endereco do diretorio atual//////////////////////////////////
void caminho(endereco *lista, unsigned char *chaveDeBusca)
{

    char buffer2[tam], buffer3[tam];
    ////////////Lista os arquivos presentes no diretório atual//////////////////////
    unsigned short tamCharsDiretorio = strlen((const char*)buffer);
    WIN32_FIND_DATA data;
    short conta = 0;
    for(unsigned short x = 0; x < tamCharsDiretorio; ++x)
    {
        buffer2[conta] = buffer[x];
        if(buffer[x] == '\\')
        {
            ++tamCharsDiretorio;
            buffer2[conta+1] = '\\';
            ++conta;
        }
        ++conta;

        if(x == tamCharsDiretorio-1)
        {
            for(short y = tamCharsDiretorio; y < tamCharsDiretorio+4; ++y)
            {

                buffer2[y] = '\\';
                if(y == tamCharsDiretorio+2)
                {
                    buffer2[y] = '*';
                }
                if(y == tamCharsDiretorio+3)
                {
                    buffer2[y] = 0;
                }
            }
        }
    }
/////////////////////////////////////////
//    WIN32_FIND_DATA arquivo;
//    HANDLE arqFind;
//    arqFind = FindFirstFileA(buffer2,&arquivo);
    HANDLE listaVerifica = FindFirstFileA(buffer2, &data);
    if(listaVerifica != INVALID_HANDLE_VALUE)
    {
        /////////////ATENÇÂO!!!AQUI COMEÇA/////////////////
        do
        {

/////////////Atribuição de endereço ao buffer3 para concatenagem
            for(short z = 0; z <= tamCharsDiretorio; ++z)
            {
                buffer3[z] = buffer2[z];
                if(z == tamCharsDiretorio)
                {
                    buffer3[tamCharsDiretorio+1] = 0;
                }
            }
/////////////////////////////////////////
            const char *substr = ".txt";
            strcat(buffer3,data.cFileName);
            strcpy(buffPaCam2,buffer2);
            unsigned int contaBuff = strlen(buffPaCam2);
            buffPaCam2[contaBuff-1] = 0;

            //RETORNA SOMENTE OS ARQUIVOS///////////////////////

            if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //printf("arquivo: %s <DIR>\n", data.cFileName);
            }
            else if((strstr(buffer3, substr)) != NULL)
            {

                FILE *file;
                unsigned char conteudo[65534] = {0};
                file = fopen(buffer3,"r");
                unsigned short quantCharsLidos = fread(&conteudo,sizeof(unsigned char),65534,file);
                fclose(file);
                conteudo[quantCharsLidos] = 0;
                acentuacao(conteudo,quantCharsLidos);
                quantCharsLidos = strlen((const char*)conteudo);
                conteudo[quantCharsLidos] = 0;

                ////convertendo para caixa alta
                unsigned short itera = 0;
                while(itera < quantCharsLidos)
                {
                    int b = conteudo[itera];
                    conteudo[itera] = toupper(b);

                    ++itera;
                }
                   cxAlta(conteudo,quantCharsLidos);//ALTERA ACENTUADOS PARA CAIXA ALTA

                for(int h = 0; h< quantCharsLidos; ++h)
                {
                    if(conteudo[h] == 10)
                    {
                        ++quantCharsLidos;
                    }
                }
                //--quantCharsLidos;//REAL QUANTIDADE DE CHARs LIDOS
                conteudo[quantCharsLidos] = 0;

                //CONCATENANDO/////////////////////////////////////
                concatena(conteudo, quantCharsLidos);

                quantCharsLidos = strlen((const char*)conteudo);
                conteudo[quantCharsLidos] = 0;

                //AQUI VERIFICA SE ENCONTRA O TERMO DE PESQUISA NOS ARQUIVOS
                unsigned char *conteudo2 = (unsigned char*)strdup((const char*)conteudo);
                unsigned char *chaveDeBusca2 = (unsigned char*)strdup((const char*)chaveDeBusca);
                unsigned short tempTam = strlen((const char*)conteudo2);
                unsigned short tempTam2 = strlen((const char*)chaveDeBusca2);
                conteudo2[tempTam] = 0;
                chaveDeBusca2[tempTam2] = 0;
                acentuacao3(conteudo2,tempTam);
                acentuacao3(chaveDeBusca2,tempTam2);
                char *oxe;
                if((oxe = strstr((const char*)conteudo,(const char*)chaveDeBusca)) != NULL || (oxe = strstr((const char*)conteudo2,(const char*)chaveDeBusca2)) != NULL)
                {
//                    printf("ENCONTREI!!%s\n", chaveDeBusca);
                    (lista+contagemArq)->nome = strdup(data.cFileName);
                    ++contagemArq;
                }free(conteudo2);free(chaveDeBusca2);

            }
            //RETORNA SOMENTE OPS ARQUIVOS///////////////////////
//    printf("arquivos: %s\n", buffer3);
        }
        while(FindNextFileA(listaVerifica, &data));
    }
    FindClose(listaVerifica);
}
//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
int tamCaminho(unsigned char *chaveDeBusca)
{

    // endereco *lista2 = (endereco*)malloc(contagemArquivos*sizeof(endereco));
    char buffer2[tam], buffer3[tam], buffer4[tam];

    ////////////Lista os arquivos presentes no diretório atual//////////////////////
    unsigned short tamCharsDiretorio = strlen((const char*)buffer);
    WIN32_FIND_DATA data;
    short conta = 0;
    for(unsigned short x = 0; x < tamCharsDiretorio; ++x)
    {
        buffer2[conta] = buffer[x];
        if(buffer[x] == '\\')
        {
            ++tamCharsDiretorio;
            buffer2[conta+1] = '\\';
            ++conta;
        }
        ++conta;

        if(x == tamCharsDiretorio-1)
        {
            for(short y = tamCharsDiretorio; y < tamCharsDiretorio+4; ++y)
            {

                buffer2[y] = '\\';
                if(y == tamCharsDiretorio+2)
                {
                    buffer2[y] = '*';
                }
                if(y == tamCharsDiretorio+3)
                {
                    buffer2[y] = 0;
                }
            }
        }
    }
    HANDLE listaVerifica = FindFirstFileA(buffer2, &data);
    if(listaVerifica != INVALID_HANDLE_VALUE)
    {
        /////////////ATENÇÂO!!!AQUI COMEÇA/////////////////
        do
        {

/////////////Atribuição de endereço ao buffer3 para concatenagem
            for(short z = 0; z <= tamCharsDiretorio; ++z)
            {
                buffer3[z] = buffer2[z];
                if(z == tamCharsDiretorio)
                {
                    buffer3[tamCharsDiretorio+1] = 0;
                }
            }
/////////////////////////////////////////
            const char *substr = ".txt", substr2[11] = {'_','c','o','p','i','a','.','t','x','t','\0'};
            strcat(buffer3,data.cFileName);
            if((strstr(buffer3, substr)) != NULL)
            {

                //Se necessário, alterando bytes EF(239) BB(187) BF(191) iniciais referentes ao modo BOM.
                //Esta função gera um novo arquivo com o mesmo nome do arquivo de referência
                FILE *hex, *hex2;
                unsigned short verificadorBOM[3] = {0};
                hex = fopen(buffer3, "rb");
                for(short n = 0; n < 3; ++n)
                {
                    fseek(hex, n, SEEK_SET);
                    verificadorBOM[n] = fgetc(hex);
                }
                fclose(hex);

                short leitor = 0;
                if(verificadorBOM[0]==239 && verificadorBOM[1]==187 && verificadorBOM[2]==191)
                {
                    hex= fopen(buffer3,"rb");
                    short soma = strlen(buffer3);
                    for(short h = 0; h < soma; ++h)
                    {
                        buffer4[h] = buffer3[h];
                        if(h == soma-1)
                        {
                            buffer4[h+1] = 0;
                        }
                    }
                    short tamanho = strlen(buffer4), tamanhoRed = tamanho-4, tamanhoAcre = tamanhoRed+10, som = 0;
                    for(short g = tamanhoRed; g < tamanhoAcre; ++g)
                    {
                        buffer4[g] = substr2[som];
                        ++som;
                        if(g == tamanhoAcre)
                        {
                            buffer4[tamanhoAcre] = 0;
                        }
                    }

                    hex2= fopen(buffer4,"wb");
                    fseek(hex,3,SEEK_SET);
                    while((leitor = fgetc(hex)) != EOF)
                    {
                        fputc(leitor,hex2);
                    }
                    fclose(hex);
                    fclose(hex2);
                    remove(buffer3);
                    rename(buffer4,buffer3);
                    remove(buffer4);
                }
            }
            //RETORNA SOMENTE OS ARQUIVOS///////////////////////

            if((strstr(buffer3, substr)) != NULL)
            {

                FILE *file;
                unsigned char conteudo[65534] = {0};
                file = fopen(buffer3,"rt");
                unsigned short quantCharsLidos = fread(&conteudo,sizeof(unsigned char),65534,file);
                fclose(file);
                conteudo[quantCharsLidos] = 0;

                ////ACENTUA E ALTERA OS ACENTUADOS PARA CAIXA ALTA
                acentuacao(conteudo,quantCharsLidos);
                quantCharsLidos = strlen((const char*)conteudo);
                conteudo[quantCharsLidos] = 0;
                cxAlta(conteudo,quantCharsLidos);//ALTERA OS ACENTUADOS APRA CAIXA ALTA

                ////convertendo para caixa alta
                unsigned short itera = 0;
                while(itera < quantCharsLidos)
                {
                    int b = conteudo[itera];
                    conteudo[itera] = toupper(b);
                    ++itera;
                }


                for(int h = 0; h< quantCharsLidos; ++h)
                {
                    if(conteudo[h] == 10)
                    {
                        ++quantCharsLidos;
                    }
                }
                //--quantCharsLidos;//REAL QUANTIDADE DE CHARs LIDOS
                conteudo[quantCharsLidos] = 0;

                //CONCATENANDO/////////////////////////////////////
                concatena((unsigned char*)conteudo, quantCharsLidos);

                quantCharsLidos = strlen((const char*)conteudo);
                conteudo[quantCharsLidos] = 0;

                //AQUI VERIFICA SE ENCONTRA O TERMO DE PESQUISA NOS ARQUIVOS
                unsigned char *conteudo2 = (unsigned char*)strdup((const char*)conteudo);
                unsigned char *chaveDeBusca2 = (unsigned char*)strdup((const char*)chaveDeBusca);
                unsigned short tempTam = strlen((const char*)conteudo2);
                unsigned short tempTam2 = strlen((const char*)chaveDeBusca2);
                conteudo2[tempTam] = 0;
                chaveDeBusca2[tempTam2] = 0;
                acentuacao3(conteudo2,tempTam);
                acentuacao3(chaveDeBusca2,tempTam2);
                char *oxe;
                if((oxe = strstr((const char*)conteudo,(const char*)chaveDeBusca)) != NULL || (oxe = strstr((const char*)conteudo2,(const char*)chaveDeBusca2)) != NULL)
                {
                    ++contagemArq;

                }free(conteudo2);free(chaveDeBusca2);

            }


            //RETORNA SOMENTE OPS ARQUIVOS///////////////////////
//    printf("arquivos: %s\n", buffer3);

        }
        while(FindNextFileA(listaVerifica, &data));
    }
    FindClose(listaVerifica);
    return contagemArq;
}
/////////////////////////////////////////////////////////////////////////
//////////////////////concatenador///////////////////////////////////////////////////
void concatena(unsigned char *stringConca, unsigned short quantCharsLidos)
{

    int quanCharsLidos = quantCharsLidos;

    for(short x = 0; x < quanCharsLidos+2; ++x)
    {
        while(stringConca[x] == 13 || stringConca[x] == 10 || stringConca[x] == 32 || stringConca[x] == 9 || stringConca[x] == 11)
        {
            for(short t = x; t < quanCharsLidos; ++t)
            {

                stringConca[t] = stringConca[t+1];
            }
        }
    }

}
////////////////////////////////////////////////////////////////////////////////
void concatena2(unsigned char *stringConca, unsigned short quantCharsLidos)
{

    for(unsigned int x = 0; x < quantCharsLidos; ++x)
    {
        while(stringConca[x] == 13 || stringConca[x] == 10 || stringConca[x] == 32 || stringConca[x] == 9 || stringConca[x] == 11)
        {
            for(unsigned int t = x; t < quantCharsLidos; ++t)
            {

                stringConca[t] = stringConca[t+1];
            }
        }
    }

}
////////////////////////////////////////////////////////////////////////////////
void limpezaPtrBusca(void)
{


    /////LIBERA MEMÓRIA ANTES DE ENCERRAR
    //printf("ptracres antes limpeza %d\n", ptrAcres);
    if(ptrAcresAuxiliar > 0)
    {
        unsigned int verificador = 0;
        while(verificador < ptrAcresAuxiliar)
        {
            free((ptrBusca+verificador)->nome);
            (ptrBusca+verificador)->nome = NULL;
            //printf("ptrBusca+ APOS NULL: %p\n", (ptrBusca+verificador)->nome);
            ++verificador;
        }
       // printf("ptrBusca ANTES: %p\n", ptrBusca);
        free(ptrBusca);
       // printf("ptrBusca APOS: %p\n", ptrBusca);
        ptrBusca = NULL;
       // printf("ptrBusca APOS NULL: %p\n", ptrBusca);
    }
    ptrAcres = 0;
    ptrAcresAuxiliar = 0;
    loop2 = FALSE;
    loop = FALSE;
    NoMatch = FALSE;
    NoMatch2 = FALSE;
    NoMatch3 = FALSE;
    SetWindowTextA(hwndListCx,"");
}
///////////////////////////////////////////////////////////////////////////
void limpezaLista(void)
{

    /////LIBERA MEMÓRIA ANTES DE ENCERRAR
//printf("*pcontando antes limpeza %d\n", *pcontando);
    if(contagemArq > 0)
    {
        unsigned int verificador = 0;
        while(verificador < contagemArq)
        {
            free((lista+verificador)->nome);
            (lista+verificador)->nome = NULL;
        //    printf("lista+ APOS NULL: %p\n", (lista+verificador)->nome);
            ++verificador;
        }
      //  printf("lista+ ANTES: %p\n", lista);
        free(lista);
       // printf("lista+ APOS: %p\n", lista);
        lista = NULL;
      //  printf("lista+ APOS NULL: %p\n", lista);

    }
    contagemArq = 0;

}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
void caminho2(void)
{

    unsigned int ptrReduzido = ptrAcres-1, contador = 0;
    char buffPaCam3[tam];
    unsigned char conteudo[65534] = {0};;
    int bytes = strlen(buffPaCam2);
    memmove(buffPaCam3,buffPaCam2,bytes);
    contador3 = 0;
    buffPaCam3[bytes] = 0;

        for(unsigned int z = 0; z < contagemArq; ++z)
        {
            if((lista+z)->nome != NULL){
            strcat(buffPaCam2,(lista+z)->nome);
            FILE *file;
            file = fopen(buffPaCam2,"r");
            unsigned short quantCharsLidos = fread(&conteudo,sizeof(unsigned char),65534,file);
            fclose(file);
            conteudo[quantCharsLidos] = 0;
            acentuacao(conteudo,quantCharsLidos);
            quantCharsLidos = strlen((const char*)conteudo);
            conteudo[quantCharsLidos] = 0;

            ////convertendo para caixa alta
            unsigned short itera = 0;
            while(itera < quantCharsLidos)
            {
                int b = conteudo[itera];
                conteudo[itera] = toupper(b);

                ++itera;
            }
             cxAlta(conteudo,quantCharsLidos);//ALTERA ACENTUADOS PARA CAIXA ALTA

            for(int h = 0; h< quantCharsLidos; ++h)
            {
                if(conteudo[h] == 10)
                {
                    ++quantCharsLidos;
                }
            }

            //CONCATENANDO/////////////////////////////////////
            concatena(conteudo, quantCharsLidos);
            quantCharsLidos = strlen((const char*)conteudo);
            conteudo[quantCharsLidos] = 0;
            }

            //AQUI VERIFICA SE ENCONTRA O TERMO DE PESQUISA NOS ARQUIVOS
                unsigned char *conteudo2 = (unsigned char*)strdup((const char*)conteudo);
                unsigned char *chaveDeBusca2 = (unsigned char*)strdup((const char*)(ptrBusca+ptrReduzido)->nome);
                unsigned short tempTam = strlen((const char*)conteudo2);
                unsigned short tempTam2 = strlen((const char*)chaveDeBusca2);
                conteudo2[tempTam] = 0;
                chaveDeBusca2[tempTam2] = 0;
                acentuacao3(conteudo2,tempTam);
                acentuacao3(chaveDeBusca2,tempTam2);
                char *oxe;

                if((oxe = strstr((const char*)conteudo,(ptrBusca+ptrReduzido)->nome)) == NULL && (oxe = strstr((const char*)conteudo2,(const char*)chaveDeBusca2)) == NULL)
                {
                ++contador;
                (lista+z)->nome = NULL;
                }free(conteudo2);free(chaveDeBusca2);

            memmove(buffPaCam2,buffPaCam3,bytes);
            buffPaCam2[bytes] = 0;
        }

}

//////////////////////////////////////////////////////////////////////////
unsigned int testaCaminho2(void){

    unsigned int ptrReduzido = ptrAcres-1,ptrReduzido2 = ptrReduzido-1, contador = 0;
    char buffPaCam3[tam];
    unsigned char conteudo[65534] = {0};
    int bytes = strlen(buffPaCam2);
    memmove(buffPaCam3,buffPaCam2,bytes);
    contador3 = 0;
    buffPaCam3[bytes] = 0;

    //IMPEDE QUE UM ARGUMENTO (ptrAcres) SEJA REPETIDO
    char *impede = strstr((ptrBusca+ptrReduzido2)->nome,(ptrBusca+ptrReduzido)->nome);
    if(impede  == NULL)
    {

        for(unsigned int z = 0; z < contagemArq; ++z)
        {
            if((lista+z)->nome != NULL){
            strcat(buffPaCam2,(lista+z)->nome);
            FILE *file;
            file = fopen(buffPaCam2,"r");
            unsigned short quantCharsLidos = fread(&conteudo,sizeof(unsigned char),65534,file);
            fclose(file);
            conteudo[quantCharsLidos] = 0;
            acentuacao(conteudo,quantCharsLidos);
            quantCharsLidos = strlen((const char*)conteudo);
            conteudo[quantCharsLidos] = 0;

            ////convertendo para caixa alta
            unsigned short itera = 0;
            while(itera < quantCharsLidos)
            {
                int b = conteudo[itera];
                conteudo[itera] = toupper(b);

                ++itera;
            }

            cxAlta(conteudo,quantCharsLidos);//ALTERA ACENTUADOS PARA CAIXA ALTA

            for(int h = 0; h< quantCharsLidos; ++h)
            {
                if(conteudo[h] == 10)
                {
                    ++quantCharsLidos;
                }
            }

            //CONCATENANDO/////////////////////////////////////
            concatena((unsigned char*)conteudo, quantCharsLidos);
            quantCharsLidos = strlen((const char*)conteudo);
            conteudo[quantCharsLidos] = 0;
            }
             //printf("z: %d\n", z);
            //AQUI VERIFICA SE ENCONTRA O TERMO DE PESQUISA NOS ARQUIVOS
                unsigned char *conteudo2 = (unsigned char*)strdup((const char*)conteudo);
                unsigned char *chaveDeBusca2 = (unsigned char*)strdup((const char*)(ptrBusca+ptrReduzido)->nome);
                unsigned short tempTam = strlen((const char*)conteudo2);
                unsigned short tempTam2 = strlen((const char*)chaveDeBusca2);
                conteudo2[tempTam] = 0;
                chaveDeBusca2[tempTam2] = 0;
                acentuacao3(conteudo2,tempTam);
                acentuacao3(chaveDeBusca2,tempTam2);
                char *oxe;

                if((oxe = strstr((const char*)conteudo,(ptrBusca+ptrReduzido)->nome)) == NULL && (oxe = strstr((const char*)conteudo2,(const char*)chaveDeBusca2)) == NULL)
            {
                ++contador;
            }free(conteudo2);free(chaveDeBusca2);

            memmove(buffPaCam2,buffPaCam3,bytes);
            buffPaCam2[bytes] = 0;
        }
    }else
        {

        if(ptrAcres > 1)
        {
            --ptrAcres;
            loop = TRUE;
            NoMatch2 = TRUE;

        }
        if(ptrAcres > 2)
        {
            --ptrAcres;
            loop2 = TRUE;
            NoMatch = TRUE;
        }

    }
    if(contador == contagemArq){


              --ptrAcres;
              NoMatch = TRUE;
              NoMatch2 = TRUE;
              NoMatch3 = TRUE;
    }

return contador;
}
////////////////////////////////////////////////////////////////////////////////
unsigned int testaCaminho3(void){

    unsigned int contador = 0;
    char buffPaCam3[tam];
    unsigned char conteudo[65534] = {0};;
    int bytes = strlen(buffPaCam2);
    memmove(buffPaCam3,buffPaCam2,bytes);
    contador3 = 0;
    buffPaCam3[bytes] = 0;

        for(unsigned int z = 0; z < contagemArq; ++z)
        {
            if((lista+z)->nome != NULL){
            strcat(buffPaCam2,(lista+z)->nome);
            FILE *file;
            file = fopen(buffPaCam2,"r");
            unsigned short quantCharsLidos = fread(&conteudo,sizeof(unsigned char),65534,file);
            fclose(file);
            conteudo[quantCharsLidos] = 0;
            acentuacao(conteudo,quantCharsLidos);
            quantCharsLidos = strlen((const char*)conteudo);
            conteudo[quantCharsLidos] = 0;

            ////convertendo para caixa alta
            unsigned short itera = 0;
            while(itera < quantCharsLidos)
            {
                int b = conteudo[itera];
                conteudo[itera] = toupper(b);

                ++itera;
            }

              cxAlta(conteudo,quantCharsLidos);//ALTERA ACENTUADOS PARA CAIXA ALTA

            for(int h = 0; h< quantCharsLidos; ++h)
            {
                if(conteudo[h] == 10)
                {
                    ++quantCharsLidos;
                }
            }

            //CONCATENANDO/////////////////////////////////////
            concatena((unsigned char*)conteudo, quantCharsLidos);
            quantCharsLidos = strlen((const char*)conteudo);
            conteudo[quantCharsLidos] = 0;
            }
             //printf("z: %d\n", z);
             //AQUI VERIFICA SE ENCONTRA O TERMO DE PESQUISA NOS ARQUIVOS
                unsigned char *conteudo2 = (unsigned char*)strdup((const char*)conteudo);
                unsigned char *chaveDeBusca2 = (unsigned char*)strdup((const char*)(ptrBusca+ptrAcres)->nome);
                unsigned short tempTam = strlen((const char*)conteudo2);
                unsigned short tempTam2 = strlen((const char*)chaveDeBusca2);
                conteudo2[tempTam] = 0;
                chaveDeBusca2[tempTam2] = 0;
                acentuacao3(conteudo2,tempTam);
                acentuacao3(chaveDeBusca2,tempTam2);
                char *oxe;

            if((oxe = strstr((const char*)conteudo,(ptrBusca+ptrAcres)->nome)) == NULL && (oxe = strstr((const char*)conteudo2,(const char*)chaveDeBusca2)) == NULL)
            {
                ++contador;
            }free(conteudo2);free(chaveDeBusca2);

            memmove(buffPaCam2,buffPaCam3,bytes);
            buffPaCam2[bytes] = 0;
        }

    if(contador == contagemArq){

              NoMatch = TRUE;
              NoMatch2 = TRUE;
    }

return contador;
}
//////////////////////////////////ACENTUAÇÃO
void acentuacao(unsigned char *ler, unsigned short ta){

unsigned char carEspeciais[49] = {0xA2,0xA0,0xA1,0xA3,0x82,0x80,0x81,0x83,0xAA,0xA8,0xA9,0x8A,0x88,
0x89,0xAD,0xAC,0xAE,0x8D,0x8C,0x8E,0xB3,0xB2,0xB4,0xB5,0x93,0x92,0x94,0x95,0xBA,0xB9,0xBB,
0x9A,0x99,0x9B,0xA7,0x87,0xAA,0xBA,0xA7,0xB1,0x91,0xBC,0x9C,0xB6,0x96,0xA4,0x84,0xAB,0x8B};
unsigned char carRespectivo[49] = {0x83,0x85,0xA0,0xC6,0xB6,0xB7,0xB5,0xC7,0x88,0x8A,0x82,0xD2,0xD4,
0x90,0xA1,0x8D,0x8C,0xD6,0xDE,0xD7,0xA2,0x95,0x93,0xE4,0xE0,0xE3,0xE2,0xE5,0xA3,0x97,0x96,
0xE9,0xEB,0xEA,0x87,0x80,0xA6,0xA7,0xF5,0xA4,0xA5,0x81,0x9A,0x94,0x99,0x84,0x8E,0x89,0xD3};

    for(unsigned short k = 0; k < ta;++k){

        for(unsigned int l = 0; l < 49;++l){

            if(ler[k] == carEspeciais[l]){
                ler[k-1] = carRespectivo[l];
                for(unsigned int h = k; h < ta;++h){
                    ler[h] = ler[h+1];
                }

            }
        }
    }

}
void acentuacao2(unsigned char *ler, unsigned short ta){

unsigned char carEspeciais[24] = {0xC7,0xC2,0xC0,0xC1,0xC3,0xCA,0xC8,0xC9,0xCD,0xCC,0xCE,
    0xD3,0xD5,0xDA,0xD9,0xDB,0xD1,0xDC,0xD6,0xC4,0xCB,0xD2,0xD4};
unsigned char carRespectivo[24] = {0x80,0xB6,0xB7,0xB5,0xC7,0xD2,0xD4,0x90,0xD6,0xDE,0xD7,
    0xE0,0xE5,0xE9,0xEB,0xEA,0xA5,0x9A,0x99,0x8E,0xD3,0xE3,0xE2};


    for(unsigned short k = 0; k < ta;++k){

        for(unsigned int l = 0; l < 24;++l){

            if(ler[k] == carEspeciais[l]){

                ler[k] = carRespectivo[l];
                break;
            }
        }
    }

}
////EM CAIXA ALTA RETIRA OS ACENTOS E CEDILHAS
void acentuacao3(unsigned char *ler, unsigned short ta){

unsigned char carEspeciais[23] = {0x80,0xB6,0xB7,0xB5,0xC7,0xD2,0xD4,0x90,0xD6,0xDE,0xD7,
    0xE0,0xE3,0xE2,0xE5,0xE9,0xEB,0xEA,0xA5,0x9A,0x99,0x8E,0xD3};
unsigned char carRespectivo[23] = {0x43,0x41,0x41,0x41,0x41,0x45,0x45,0x45,0x49,0x49,0x49,
    0x4F,0x4F,0x4F,0x4F,0x55,0x55,0x55,0x4E,0x55,0x4F,0x41,0x45};


    for(unsigned short k = 0; k < ta;++k){

        for(unsigned int l = 0; l < 23;++l){

            if(ler[k] == carEspeciais[l]){

                ler[k] = carRespectivo[l];
                break;
            }
        }
    }

}
void cxAlta(unsigned char *ler, unsigned short ta){

unsigned char carEspeciais[23] = {0x87,0x83,0x85,0xA0,0xC6,0x88,0x8A,0x82,0xA1,0x8D,0x8C,
0xA2,0x95,0x93,0xE4,0xA3,0x97,0x96,0xA4,0x81,0x94,0x84,0x89};
unsigned char carRespectivo[23] = {0x80,0xB6,0xB7,0xB5,0xC7,0xD2,0xD4,0x90,0xD6,0xDE,0xD7,
0xE0,0xe3,0xE2,0xE5,0xE9,0xEB,0xEA,0xA5,0x9A,0x99,0x8E,0xD3};


    for(unsigned short k = 0; k < ta;++k){

        for(unsigned int l = 0; l < 23;++l){

            if(ler[k] == carEspeciais[l]){

                ler[k] = carRespectivo[l];
                break;
            }
        }
    }

}
