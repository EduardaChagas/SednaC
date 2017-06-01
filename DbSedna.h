#include "libsedna.h"

namespace fcdbwgp {

class DbSedna {

    struct SednaConnection *conn{nullptr};

    public:
        bool initialize();
        int carregaXMLtoDoc(struct SednaConnection *conn);
        int criarColecao(struct SednaConnection *conn);
        int loadDocument(struct SednaConnection *conn);
        char* consultaPosicao(struct SednaConnection *conn,char* elemento,char* id,int opcao);
        char* consultaCor(struct SednaConnection *conn,char* elemento,char* id);
        void closeSection(struct SednaConnection *conn);
};

}

