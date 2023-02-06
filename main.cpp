#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <stdio.h>
#include <locale.h>
#include <malloc.h>
#include <fileapi.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <shlobj.h>

#include "resource.h"
#include "backend.h"


HINSTANCE hInst;

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


    switch(uMsg)
    {

    case WM_INITDIALOG:

    {

    BROWSEINFO br;
    ZeroMemory(&br, sizeof(BROWSEINFO));
    br.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    br.hwndOwner = NULL;
    br.lpszTitle = "Selecione o diretório com arquivos";
    br.lParam = (LPARAM)"C:\\";

    LPITEMIDLIST pidl = NULL;
    if ((pidl = SHBrowseForFolder(&br)) != NULL)
    {
        SHGetPathFromIDList(pidl, buffer);
        printf("buffer NA ENTRADA-> %s\n", buffer);
    }else{return 1;}


    }

    return TRUE;

    case WM_CLOSE:
    {


        if (MessageBox(hwndDlg, "Encerrar essa aplicação?", "Buscador", MB_OKCANCEL) == IDOK)
        {

            limpezaPtrBusca();
            limpezaLista();

            DestroyWindow(hwndDlg);
        }
        // Else: User canceled. Do nothing.
        return 0;
//        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        int wmEvent = HIWORD(wParam);

        switch(LOWORD(wParam))
        {


        case BTN_BUSCAR:

        {

            //LIMPA TELA
            if(*pcontando)
            {
                int x = *pcontando;
                HWND hwndListando = GetDlgItem(hwndDlg,SHOW_LIST);
                while(x != -1)
                {
                    SendMessageA(hwndListando,LB_DELETESTRING,0,(LPARAM)(x));
                    //SendMessage(hwndList,LB_SETITEMDATA,pos,(LPARAM)x);
                    --x;
                }
                SetFocus(hwndListando);
            }

////////////////////////////////////////////////////////////////////////////////////////
            //Pega texto de CX_TEXT
            hwndListCx = GetDlgItem(hwndDlg,CX_TEXT);

            unsigned char chaveDeBusca[tam] = {0};
            unsigned short verify = GetWindowTextA(hwndListCx,(LPSTR)chaveDeBusca,1024);
            chaveDeBusca[verify] = 0;

            if(verify == 0)
            {
                char aviso[] = "Necessário digitar algo para que eu possa pesquisar!";
                MessageBoxA(hwndDlg,aviso,"Atenção!",MB_OK | MB_ICONEXCLAMATION);
                return TRUE;
            }
            acentuacao2(chaveDeBusca,verify);
            concatena2((unsigned char*)chaveDeBusca,verify);
            verify = strlen((const char*)chaveDeBusca);
            chaveDeBusca[verify] = 0;
            /////////////////////ptrBusca recebe o termo da pesquisa e guarda

            if(loop == FALSE && loop2 == FALSE && NoMatch == FALSE)
            {
                ++ptrAcres;
                ptrBusca = (endereco*)realloc(ptrBusca,ptrAcres*sizeof(endereco*));
                if(ptrBusca != NULL)
                {
                    (ptrBusca+ptrAcresAuxiliar)->nome = strdup((const char*)chaveDeBusca);
                    ptrAcresAuxiliar = ptrAcres;
                }

                else
                {
                    //printf("sem memória disponível!\n");
                    return TRUE;
                }

                if(ptrAcres < 2)
                {
                    contagemArq = tamCaminho(chaveDeBusca);//CAPTURA ARQUIVOS INICIAIS
                }
                printf("buffer ANTES IF-> %s\n", buffer);
                if(contagemArq == 0)
                {
                    char aviso2[] = "Não encontrei nenhum arquivo que contivesse o termo digitado.";
                    MessageBoxA(hwndDlg,aviso2,"Atenção!",MB_OK | MB_ICONEXCLAMATION);
                    limpezaPtrBusca();
                    limpezaLista();
                    return TRUE;
                }

                //ALOCA ESPAÇO PARA ARMAZENAR ARQUIVOS
                if(loop == FALSE && ptrAcres < 2)
                {
                    lista = (endereco*)realloc(lista,contagemArq*sizeof(endereco*));
                    contagemArq = 0;
                    caminho(lista,chaveDeBusca);//ESSA FUNÇÃO ALTERA O VALOR DE contagemArq
                }

                //BUSCA O TERMO DE PESQUISA DENTRO DOS ARQUIVOS ENCONTRADOS

                if(ptrAcres > 1 && loop == FALSE && loop2 == FALSE && NoMatch == FALSE)
                {
                    unsigned int testeRecebido = 0;
                    testeRecebido = testaCaminho2();
                    if(testeRecebido < contagemArq)
                    {
                        caminho2();
                    }
                }
            }
            /////////////////////VERIFICA QUANDO ARGUMENTO FOI BLOQUEADO
            if(NoMatch2 == TRUE)
            {
                char *verificador = NULL;
                if((verificador = strstr((ptrBusca+ptrAcres)->nome,(const char*)chaveDeBusca))== NULL)
                {
                    free((ptrBusca+ptrAcres)->nome);
                    (ptrBusca+ptrAcres)->nome = NULL;
                    (ptrBusca+ptrAcres)->nome = strdup((const char*)chaveDeBusca);

                    unsigned int testeRecebido = 0;
                    testeRecebido = testaCaminho3();
                    if(testeRecebido < contagemArq)
                    {
                        NoMatch = FALSE;
                        NoMatch2 = FALSE;
                        NoMatch3 = FALSE;
                        loop = FALSE;
                        loop2 = FALSE;
                        ++ptrAcres;
                        caminho2();
                    }
                }
            }

            if(NoMatch3 == TRUE)
            {
                char aviso2[] = "Não encontrei o termo informado nos arquivos atuais.";
                MessageBoxA(hwndDlg,aviso2,"Atenção!",MB_OK | MB_ICONEXCLAMATION);
//                limpezaPtrBusca();
//                limpezaLista();

            }
//            if(ptrAcres > 1){
//            unsigned int ptrReduzido = ptrAcres-1,ptrReduzido2 = ptrReduzido-1;
//            printf("ptrBusca ANTES-> %s, ptrBusca APOS-> %s\n",(ptrBusca+ptrReduzido2)->nome,(ptrBusca+ptrReduzido)->nome);
//            }
//            printf("loop-> %d, loop2-> %d noMatch->%d ptracress -> %d\n", loop,loop2,NoMatch,ptrAcres);
//            printf("ptrAcress: %d\n", ptrAcres);
//            printf("contagemarq ANTES DE PRINTAR: %d\n", contagemArq);
            ///IMPRIME OS ARQUIVOS ENCONTRADOS
            unsigned int pos = 0;
            HWND hwndLista2 = GetDlgItem(hwndDlg,SHOW_LIST);
            for(unsigned int x = 0; x < contagemArq; ++x)
            {
                pos = (unsigned int)SendMessageA(hwndLista2,LB_ADDSTRING,0,(LPARAM)(lista+x)->nome);
                SendMessage(hwndLista2,LB_SETITEMDATA,pos,(LPARAM)x);
            }
            SetFocus(hwndLista2);
            if(ptrAcres == 1)
            {
                *pcontando = SendMessage(hwndLista2,LB_GETCOUNT,0,0);
            }

            return TRUE;
        }
////////////////////////////////////////////////////////////////////////////


        case BTN_LIMPAR:
        {


            //LIMPA TELA
            if(*pcontando)
            {
                int x = *pcontando;
                HWND hwndListando = GetDlgItem(hwndDlg,SHOW_LIST);
                while(x != -1)
                {
                    SendMessageA(hwndListando,LB_DELETESTRING,0,(LPARAM)(x));
                    //SendMessage(hwndList,LB_SETITEMDATA,pos,(LPARAM)x);
                    --x;
                }
                SetFocus(hwndListando);
            }

            //LIBERA MEMORIA

            limpezaPtrBusca();
            limpezaLista();

            return TRUE;
        }

        case SHOW_LIST:

        {
            if(wmEvent == LBN_DBLCLK)
            {
                HWND hwndListando = GetDlgItem(hwndDlg,SHOW_LIST);
                //Pega o índice selecionado
                int listItem = (int)SendMessageA(hwndListando,LB_GETCARETINDEX,0,0);
                //Pega o ítem selecionado
                char item[tam];
                int ta = SendMessageA(hwndListando,LB_GETTEXT,listItem,(LPARAM)item);
                item[ta] = 0;
                char address[tam] = {0};
                int kontando = strlen((buffer));
                for(int l = 0; l < kontando;++l)
                {
                    address[l] = buffer[l];
                }
                address[kontando] = 0;
                address[kontando] = '\\';
                strcat(address,item);
                ShellExecute(hwndListando, "open","notepad", address, NULL, SW_SHOWNORMAL);
            }
        }

        }
    }
    return TRUE;
    }
    return FALSE;
}
///////////////////////
//int ativa(void){
//
// ShellExecuteA(NULL,"runas","verificador.exe",NULL,NULL,SW_HIDE);
//     return 0;
//
//}
///////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

    /////////////////////////////////////////////////////////////////
    ///Verifica a presença do Registro
    unsigned char codigo[BUFSIZ] = {0};
    unsigned int buffSize = 8192;
    //SE NÃO ENCONTROU NENHUM REGISTRO
    if(RegGetValueA(HKEY_LOCAL_MACHINE,"SOFTWARE\\Buscador",
                    "ProductId",RRF_RT_ANY,NULL,(PVOID)&codigo,(LPDWORD)&buffSize)!= ERROR_SUCCESS)
    {
        //printf("primeiro IF\n");
        int sms = MessageBoxA(NULL,"Buscador não está ativado para este computador!\nDeseja ativar?","Atenção!",MB_YESNOCANCEL | MB_ICONQUESTION | MB_TASKMODAL);

        switch(sms)
        {
        case IDYES:
        {
            //printf("Apertou YES!");
            long unsigned int retorna = 0;
            SHELLEXECUTEINFO ShExecInfo = {0};
            ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
            ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
            ShExecInfo.hwnd = NULL;
            ShExecInfo.lpVerb = "runas";
            ShExecInfo.lpFile = "verificador.exe";
            ShExecInfo.lpParameters = "";
            ShExecInfo.lpDirectory = NULL;
            ShExecInfo.nShow = SW_HIDE;
            ShExecInfo.hInstApp = NULL;
            ShellExecuteEx(&ShExecInfo);
            WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
            GetExitCodeProcess(ShExecInfo.hProcess,&retorna);
           // printf("retorna-> %lu\n", retorna);
            if(retorna == 1)
                {
                    MessageBoxA(NULL,"Erro na ativação. Contate o fornecedor do software.","ATENÇÃO!",MB_ICONERROR | MB_OK);
                    return 1;
                }
            if(retorna != 30)
            {
                //printf("retorna-> %lu\n", retorna);
                return 1;
            }

            break;
        }
        case IDNO:
        {
            //printf("Apertou NO");
            return 1;
            break;
        }
        case IDCANCEL:
        {
            //printf("Apertou CANCEL");
            return 1;
            break;

        }
        default:
            return 1;

        }

    }

    ///SE ENCONTROU ALGUM REGISTRO, PORÉM VERIFICAR SE É VÁLIDO

    else if(RegGetValueA(HKEY_LOCAL_MACHINE,"SOFTWARE\\Buscador",
                         "ProductId",RRF_RT_ANY,NULL,(PVOID)&codigo,(LPDWORD)&buffSize)== ERROR_SUCCESS)
    {
        //printf("Segundo IF\n");

        ////////////////////////////////////////////////////////////////////////////////
        unsigned long HASH = 5381;
        unsigned char *registro = (unsigned char*)"8A5E8A8BE6524AADF067BDD8476D7677952E8DHK803CF4193FGE507C93B04070\0";
        unsigned char *str = NULL, *STRdATA = NULL,STRdATAa[vetor] = {0},strConca[vetor3] = {0} ;
        unsigned char comparador2[vetor] = {0};
        int salto = 0, salto2 = 10, kount = 0, kounta = 0, itera = 0;
        for( int m = 0; m < 6; ++m)
        {
            int contagem = 0;
            for(int x = salto; x < salto2; ++x)
            {
                comparador2[contagem]=registro[x];

                if(x == salto2-1)
                {
                    salto = salto2;
                    salto2 = salto2+10;
                    break;
                }
                ++contagem;
            }

            str = comparador2;
            int konta = strlen((const char*)str);
            str[konta] = 0;
            // printf("konta-> %d\n", konta);
            //printf("comparador2-> %s\n", comparador2);
            //printf("str-> %s\n", str);
            int c = 0;

            do
            {
                if(c == EOF)
                {
                    break;
                }
                HASH = ((HASH << 5) + HASH) + c; /* hash * 33 + c */

            }
            while ((c = *str++));
            //printf("row-> %s\n", row[i]);
            sprintf((char*)STRdATAa,"%lu",HASH);
            kount = strlen((const char*)STRdATAa);
            STRdATAa[kount] = 0;
            kounta = kount + kounta;
            int iteragem = 0;
            for(int g = itera; g < kounta; ++g)
            {
                strConca[g] = STRdATAa[iteragem];
                ++iteragem;
            }
            itera = kounta;
            int zera = strlen((const char*)strConca);
            strConca[zera] = 0;


        }
        strConca[0] = STRdATAa[2];
        strConca[6] = STRdATAa[5];
        strConca[1] = STRdATAa[3];
        strConca[7] = STRdATAa[9];
        strConca[8] = STRdATAa[4];
        STRdATA = strConca;
//        printf("codigo-> %s\n", codigo);
//        printf("STRDATA-> %s\n", STRdATA);
        int teste = 0;
        if((teste = strcmp((const char*)STRdATA,(const char*)codigo))!= 0)
        {
            int sms = MessageBoxA(NULL,"Buscador não está ativado para este computador!\nDeseja ativar?","Atenção!",MB_YESNOCANCEL | MB_ICONQUESTION | MB_TASKMODAL);

            switch(sms)
            {
            case IDYES:
            {
            //printf("Apertou YES!");
            long unsigned int retorna = 0;
            SHELLEXECUTEINFO ShExecInfo = {0};
            ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
            ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
            ShExecInfo.hwnd = NULL;
            ShExecInfo.lpVerb = "runas";
            ShExecInfo.lpFile = "verificador.exe";
            ShExecInfo.lpParameters = "";
            ShExecInfo.lpDirectory = NULL;
            ShExecInfo.nShow = SW_HIDE;
            ShExecInfo.hInstApp = NULL;
            ShellExecuteEx(&ShExecInfo);
            WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
            GetExitCodeProcess(ShExecInfo.hProcess,&retorna);
            printf("retorna-> %lu\n", retorna);
            if(retorna == 1)
                {
                    MessageBoxA(NULL,"Erro na ativação. Contate o fornecedor do software.","ATENÇÃO!",MB_ICONERROR | MB_OK);
                    return 1;
                }
            if(retorna != 30)
            {
                //printf("retorna-> %lu\n", retorna);
                return 1;
            }
                break;
            }
            case IDNO:
            {
                //printf("Apertou NO");
                return 1;
                break;
            }
            case IDCANCEL:
            {
                //printf("Apertou CANCEL");
                return 1;
                break;

            }
            default:
                return 1;

            }

        }
    }

    /////////////////////////////////////////////////////////////////
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
