#include "libsedna.h"
#include "DbSedna.h"
#include <cstdio>
#include <cstring>

using namespace std;

namespace fcdbwgp {

//Inicializando o banco de dados
bool DbSedna::initialize() {
    struct SednaConnection conn = SEDNA_CONNECTION_INITIALIZER;

    const char* url = "localhost";
    const char* login = "SYSTEM";
    const char* password = "MANAGER";
    const char* db_name = "Sedna";
    int res;

    printf("01_connect started.\n");

    //connecting to database "testdb" with login "SYSTEM", password "MANAGER"
    res = SEconnect(&conn, url, db_name, login, password);
    if(res != SEDNA_SESSION_OPEN){
        printf("Session starting failed: \n%s\n", SEgetLastErrorMsg(&conn));
		return true;
    }
    printf("Session opened successfully.\n");
    return false;
}

//Carrega documento XML
int DbSedna::carregaXMLtoDoc(struct SednaConnection *conn){
	int res;

    res = SEexecute(conn, "LOAD \"cenario.xml\" \"cenario\""); 
    if(res != SEDNA_BULK_LOAD_SUCCEEDED) 
    {
        printf("Bulk load failed: \n%s\n", SEgetLastErrorMsg(conn));
        // closing session
        SEclose(conn);
        return -1;
    }
    printf("Document has been successfully loaded into database.\n");
    return 0;
}

//Configurar documento XML em uma coleção
int DbSedna::criarColecao(struct SednaConnection *conn){
	int res;
	res = SEexecute(conn, "CREATE COLLECTION \"cenarioCollection\""); 
    if(res != SEDNA_UPDATE_SUCCEEDED) 
    {
        printf("Create collection failed: \n%s\n", SEgetLastErrorMsg(conn));
        // closing session
        SEclose(conn);
        return -1;
    }
    printf("Collection 'cenarioCollection' has been created successfully.\n");
    return 0;
}

//Carregar o XML do documento na coleção
int DbSedna::loadDocument(struct SednaConnection *conn){

	int res;
	char* query;

	strcpy(query, "LOAD \"cenario.xml\" \"cenario\" \"cenarioCollection\"");

    res = SEexecute(conn, query); 

    if(res != SEDNA_BULK_LOAD_SUCCEEDED)
    {
        printf("Bulk load failed: \n%s\n", SEgetLastErrorMsg(conn));
        // closing session
        SEclose(conn);
        return -1;
    }
    printf("Document cenario has been loaded into collection.\n");
    return 0;
}

//Consulta a posição dos elementos no cenário
char* DbSedna::consultaPosicao(struct SednaConnection *conn,char* elemento,char* id,int opcao){
    size_t bytes_read;
    char buf[1024];
    int res,final;
    char* query ="for $e in doc(\"cenario.xml\")/*/*";
    query = strcat(query,elemento);
    query = strcat(query,"\n");
    query = strcat(query,"where $e/@id=\'");
    query = strcat(query,id);
    query = strcat(query,"\'\n");
    if(opcao == 1){
            query = strcat(query,"return number($e/*posicaoX)");
    }else if(opcao == 2){
            query = strcat(query,"return number($e/*posicaoY)");
    }
    res = SEexecute(conn, query); 
    if(res != SEDNA_QUERY_SUCCEEDED) 
    {
        printf("Query failed: \n%s\n", SEgetLastErrorMsg(conn));
        //closing session
        SEclose(conn);
    }    
    do
    {
        bytes_read = SEgetData(conn, buf, 1024 - 1);
        if(bytes_read == SEDNA_ERROR)
        {
            printf("Failed to get result data from server: \n%s\n", SEgetLastErrorMsg(conn));
            //closing session
            SEclose(conn);
        }
        buf[bytes_read] = '\0';
    }while(bytes_read > 0);
    return buf;
}

//Consulta a cor dos elementos no cenário
char* DbSedna::consultaCor(struct SednaConnection *conn,char* elemento,char* id){
    size_t bytes_read;
    char buf[1024];
    int res,final;
    char* query ="for $e in doc(\"cenario.xml\")/*/*";
    query = strcat(query,elemento);
    query = strcat(query,"\n");
    query = strcat(query,"where $e/@id=\'");
    query = strcat(query,id);
    query = strcat(query,"\'\n");
    query = strcat(query,"return number($e/*cor)");
    res = SEexecute(conn, query); 
    if(res != SEDNA_QUERY_SUCCEEDED) 
    {
        printf("Query failed: \n%s\n", SEgetLastErrorMsg(conn));
        //closing session
        SEclose(conn);
    }    
    do
    {
        bytes_read = SEgetData(conn, buf, 1024 - 1);
        if(bytes_read == SEDNA_ERROR)
        {
            printf("Failed to get result data from server: \n%s\n", SEgetLastErrorMsg(conn));
            //closing session
            SEclose(conn);
        }
        buf[bytes_read] = '\0';
    }while(bytes_read > 0);
    return buf;
}


//Pronto
void DbSedna::closeSection(struct SednaConnection *conn){
	int res;
	res = SEclose(conn);
    if(res != SEDNA_SESSION_CLOSED) 
    {
        printf("A error occured while trying to close session: \n%s\n", SEgetLastErrorMsg(conn));
        
    }

    printf("Session closed successfully.\n");
}

}


