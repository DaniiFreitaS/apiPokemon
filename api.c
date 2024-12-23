#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definição da estrutura do Pokémon
typedef struct {
    int id;
    int generation;
    char name[50];
    char description[100];
    float weight;
    float height;
    int capture_rate;
    bool is_legendary;
} Pokemon;

// Estrutura do nó para encadeamento simples
typedef struct Node {
    Pokemon data;
    struct Node* next;
} Node;

// Estrutura da fila circular
typedef struct {
    Node* primeiro;
    Node* ultimo;
    int tamanho;
    int capacidade;
} FilaPokemon;

// Funções para manipular a fila circular
void startF(FilaPokemon* l, int capacidade) {
    l->primeiro = NULL;
    l->ultimo = NULL;
    l->tamanho = 0;
    l->capacidade = capacidade;
}

bool inserirF(FilaPokemon* l, Pokemon p) {
    if (l->tamanho == l->capacidade) {
        return false; // Fila cheia
    }
    Node* novo = (Node*)malloc(sizeof(Node));
    if (!novo) {
        return false; // Falha na alocação
    }
    novo->data = p;
    novo->next = NULL;
    if (l->tamanho == 0) {
        l->primeiro = novo;
        l->ultimo = novo;
        novo->next = novo; // Circular
    } else {
        novo->next = l->primeiro;
        l->ultimo->next = novo;
        l->ultimo = novo;
    }
    l->tamanho++;
    return true;
}

void liberarF(FilaPokemon* l) {
    if (l->tamanho == 0) return;

    Node* atual = l->primeiro;
    do {
        Node* temp = atual;
        atual = atual->next;
        free(temp);
    } while (atual != l->primeiro);

    l->primeiro = NULL;
    l->ultimo = NULL;
    l->tamanho = 0;
}

// Função para buscar um Pokémon pelo ID
Pokemon* buscarPorId(FilaPokemon* l, int id) {
    if (l->primeiro == NULL) {
        return NULL; // Fila vazia
    }
    Node* atual = l->primeiro;
    do {
        if (atual->data.id == id) {
            return &atual->data;
        }
        atual = atual->next;
    } while (atual != l->primeiro);
    return NULL; // Não encontrado
}

// Função para gerar a resposta JSON
int gerarResposta(struct MHD_Connection* connection, int status_code, const char* resposta) {
    struct MHD_Response* response = MHD_create_response_from_buffer(strlen(resposta), (void*)resposta, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);
    return ret;
}

// Callback para processar as requisições
int processaRequisicao(void* cls, struct MHD_Connection* connection, const char* url, const char* method,
                       const char* version, const char* upload_data, size_t* upload_data_size, void** ptr) {
    static int dummy;
    if (&dummy != *ptr) {
        *ptr = &dummy;
        return MHD_YES;
    }
    *ptr = NULL;

    // Aceitar apenas GET
    if (strcmp(method, "GET") != 0) {
        return gerarResposta(connection, MHD_HTTP_METHOD_NOT_ALLOWED, "Método não permitido.");
    }

    // Verificar se a URL tem o formato correto (/pokemon?id=123)
    const char* id_str = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "id");
    if (!id_str) {
        return gerarResposta(connection, MHD_HTTP_BAD_REQUEST, "Parâmetro 'id' é necessário.");
    }

    int id = atoi(id_str);

    // Buscar Pokémon pelo ID (substituir pelo objeto da fila)
    FilaPokemon* fila = (FilaPokemon*)cls;
    Pokemon* pokemon = buscarPorId(fila, id);
    if (!pokemon) {
        return gerarResposta(connection, MHD_HTTP_NOT_FOUND, "Pokémon não encontrado.");
    }

    // Criar resposta JSON
    char json[512];
    snprintf(json, sizeof(json),
             "{"
             "\"id\": %d,"
             "\"generation\": %d,"
             "\"name\": \"%s\","
             "\"description\": \"%s\","
             "\"weight\": %.2f,"
             "\"height\": %.2f,"
             "\"capture_rate\": %d,"
             "\"is_legendary\": %s"
             "}",
             pokemon->id, pokemon->generation, pokemon->name, pokemon->description, pokemon->weight, pokemon->height,
             pokemon->capture_rate, pokemon->is_legendary ? "true" : "false");

    return gerarResposta(connection, MHD_HTTP_OK, json);
}

int main() {
    FilaPokemon fila;
    startF(&fila, 10);

    // Adicionar alguns Pokémon de teste
    Pokemon p1 = {1, 1, "Bulbasaur", "Seed Pokémon", 6.9, 0.7, 45, false};
    Pokemon p2 = {2, 1, "Ivysaur", "Seed Pokémon", 13.0, 1.0, 45, false};
    inserirF(&fila, p1);
    inserirF(&fila, p2);

    // Iniciar servidor
    struct MHD_Daemon* daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, 8080, NULL, NULL, &processaRequisicao, (void*)&fila, MHD_OPTION_END);
    if (!daemon) {
        printf("Erro ao iniciar o servidor.\n");
        return 1;
    }

    printf("Servidor rodando na porta 8080. Pressione Ctrl+C para encerrar.\n");
    getchar();

    MHD_stop_daemon(daemon);
    liberarF(&fila);

    return 0;
}
