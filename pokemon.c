#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <time.h>
#include <microhttpd.h>
//#include <stdbool.h>
#define bool   short
#define true   1
#define false  0

typedef struct
{
    char **array; // Elementos da lista (strings)
    int n;        // Quantidade de elementos na lista
    int tamanho;  // Capacidade atual da lista
} Lista;

/**
 * Inicializa a lista com capacidade inicial
 * @param l Lista* ponteiro para a lista.
 * @param capacidade Capacidade inicial da lista.
 */
void start(Lista *l, int capacidade)
{
    l->n = 0;
    l->tamanho = capacidade;
    l->array = (char **)malloc(l->tamanho * sizeof(char *)); // Aloca dinamicamente o array
}

/**
 * Insere uma string na primeira posição da lista
 * @param l Lista* ponteiro para a lista.
 * @param x char* string a ser inserida.
 */
void inserirInicio(Lista *l, char *x)
{
    int i;

    // Validar inserção
    if (l->n >= l->tamanho)
    {
        printf("Erro ao inserir! Lista cheia.\n");
        exit(1);
    }

    // Levar elementos para o fim do array
    for (i = l->n; i > 0; i--)
    {
        l->array[i] = l->array[i - 1];
    }

    l->array[0] = strdup(x); // Copiar a string
    l->n++;
}

/**
 * Insere uma string na última posição da lista
 * @param l Lista* ponteiro para a lista.
 * @param x char* string a ser inserida.
 */
void inserirFim(Lista *l, char *x)
{
    // Validar inserção
    if (l->n >= l->tamanho)
    {
        printf("Erro ao inserir! Lista cheia.\n");
        exit(1);
    }

    l->array[l->n] = strdup(x); // Copiar a string
    l->n++;
}

/**
 * Insere uma string em uma posição específica da lista
 * @param l Lista* ponteiro para a lista.
 * @param x char* string a ser inserida.
 * @param pos Posicao de inserção.
 */
void inserir(Lista *l, char *x, int pos)
{
    int i;

    // Validar inserção
    if (l->n >= l->tamanho || pos < 0 || pos > l->n)
    {
        printf("Erro ao inserir! Posição inválida ou lista cheia.\n");
        exit(1);
    }

    // Levar elementos para o fim do array
    for (i = l->n; i > pos; i--)
    {
        l->array[i] = l->array[i - 1];
    }

    l->array[pos] = strdup(x); // Copiar a string
    l->n++;
}

/**
 * Remove uma string da primeira posição da lista
 * @param l Lista* ponteiro para a lista.
 * @return resp char* string a ser removida.
 */
char *removerInicio(Lista *l)
{
    int i;
    char *resp;

    // Validar remoção
    if (l->n == 0)
    {
        printf("Erro ao remover! Lista vazia.\n");
        exit(1);
    }

    resp = l->array[0];
    l->n--;

    for (i = 0; i < l->n; i++)
    {
        l->array[i] = l->array[i + 1];
    }

    return resp;
}

/**
 * Remove uma string da última posição da lista
 * @param l Lista* ponteiro para a lista.
 * @return resp char* string a ser removida.
 */
char *removerFim(Lista *l)
{
    // Validar remoção
    if (l->n == 0)
    {
        printf("Erro ao remover! Lista vazia.\n");
        exit(1);
    }

    return l->array[--l->n];
}

/**
 * Remove uma string de uma posição específica da lista
 * @param l Lista* ponteiro para a lista.
 * @param pos Posicao de remoção.
 * @return resp char* string a ser removida.
 */
char *remover(Lista *l, int pos)
{
    int i;
    char *resp;

    // Validar remoção
    if (l->n == 0 || pos < 0 || pos >= l->n)
    {
        printf("Erro ao remover! Posição inválida ou lista vazia.\n");
        exit(1);
    }

    resp = l->array[pos];
    l->n--;

    for (i = pos; i < l->n; i++)
    {
        l->array[i] = l->array[i + 1];
    }

    return resp;
}

/**
 * Mostra os elementos da lista separados por espaços
 * @param l Lista* ponteiro para a lista.
 */
void mostrar(Lista *l)
{
    int i;

    printf("[");
    for (i = 0; i < l->n; i++)
    {
        printf("'%s'", l->array[i]);
        if (l->n > 1 && i < l->n - 1)
        {
            printf(", ");
        }
    }
    printf("]");
}

char *listaParaString(Lista *l){
    int i;
    char *resp = (char*)malloc(sizeof(char*));
    strcpy(resp, "");
    for (i = 0; i < l->n; i++)
    {
        strcat(resp, l->array[i]);
        if (l->n > 1 && i < l->n - 1)
        {
            strcat(resp, ", ");
        }
    }
    return resp;
}

/**
 * Libera a memória alocada pela lista
 * @param l Lista* ponteiro para a lista.
 */
void liberar(Lista *l)
{
    for (int i = 0; i < l->n; i++)
    {
        free(l->array[i]); // Libera cada string alocada
    }
    free(l->array); // Libera o array dinâmico
}

typedef struct Pokemon
{
    int id;
    int generation;
    char name[80];
    char description[200];
    Lista type;
    Lista abilities;
    double weight;
    double height;
    int capture_rate;
    bool is_legendary;
    struct tm captureDate;
} Pokemon;
int toInt(char *s)
{
    int resp = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        resp = resp * 10 + ((int)(s[i] - 48));
    }
    return resp;
}
char *booleanToString(bool b)
{
    return b ? "true" : "false";
}
void imprimir(Pokemon p)
{
    printf("[#%d -> %s: %s - ", p.id, p.name, p.description);
    mostrar(&p.type);
    printf(" - ");
    mostrar(&p.abilities);
    printf(" - %0.1fkg - %0.1fm - %d%% - %s - %d gen] - ", p.weight, p.height, p.capture_rate, booleanToString(p.is_legendary), p.generation);
    printf("%02d/%02d/%d\n", p.captureDate.tm_mday, p.captureDate.tm_mon + 1, p.captureDate.tm_year + 1900);
}
Pokemon ler(char *s)
{
    char saida[12][200];
    Pokemon p;
    int sinal = 0;
    int sentinelaLista = 0;
    int i = 0;
    for (int k = 0; k < 12; k++)
    {
        strcpy(saida[k], ""); // Inicializa cada string como vazia
    }
    while (i < strlen(s))
    {
        if (s[i] == '[')
        {
            sentinelaLista++;
            i++;
        }
        else if (s[i] == ']')
        {
            sentinelaLista++;
            i++;
        }
        if (s[i] == ',')
        {
            if (sentinelaLista == 1)//verifica se esta dentro da lista
            {
                strncat(saida[sinal], &s[i], 1);//concatena a string
                i++;
            }
            else
            {
                sinal++;//passa para o proximo item
                i++;
            }
        }
        else if (s[i] == 39)//
        {
            i++;
        }
        else if (s[i] == '"')
        {
            i++;
        }
        else
        {
            strncat(saida[sinal], &s[i], 1);//concatena a string
            i++;
        }
    }
    start(&p.type, 2);
    // id,generation,name,description,type1,type2,abilities,weight_kg,height_m,capture_rate,is_legendary,capture_date
    p.id = toInt(saida[0]);
    p.generation = toInt(saida[1]);
    strcpy(p.name, saida[2]);
    strcpy(p.description, saida[3]);
    inserirFim(&p.type, saida[4]);
    if (strlen(saida[5]) > 2)
    {
        inserirFim(&p.type, saida[5]);
    }
    start(&p.abilities, 20);//inicia a lista abilities
    char *token = strtok(saida[6], ",");
    while (token != NULL)
    {
        //printf("%s\n", token);
        while (*token == ' ') token++;//remover o espaço depois da virgula
        inserirFim(&p.abilities, token);//insere na lista
        token = strtok(NULL, ",");
        
    }
    if (strlen(saida[7]) > 0)
    {
        p.weight = atof(saida[7]);
    }
    else
    {
        p.weight = 0;
    }
    if (strlen(saida[8]) > 0)
    {
        p.height = atof(saida[8]);
    }
    else
    {
        p.height = 0;
    }
    p.capture_rate = toInt(saida[9]);
    p.is_legendary = (toInt(saida[10]) == 1);
    strptime(saida[11], "%d/%m/%Y", &p.captureDate);//data
    //imprimir(p);
    return p;
}

//Vetor Pokemon global a ser usado
Pokemon *p[802];//cria vetor de ponteiros para pokemon

void startV(){
    FILE *raw = fopen("pokemon.csv", "r");//abre arquivo csv
    char str[802][500];//vetor de string com os valores do csv
    Pokemon pokemon[802];//cria vetor de pokemon para guardar todos
    
    fscanf(raw, " %[^\r\n]", str[0]);//pula a linha 0 do csv
    for (int i = 0; i < 802; i++)
    {
        fscanf(raw, " %[^\r\n]", str[i]);
        pokemon[i] = ler(str[i]);//transforma a string do csv em um tipo pokemon
        p[i] = &pokemon[i];
    }
    fclose(raw);
}

// Adiciona cabeçalhos CORS na resposta
void adicionarCabecalhosCORS(struct MHD_Response* response) {
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, OPTIONS");
    MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");
}

// Função para gerar a resposta JSON
int gerarResposta(struct MHD_Connection* connection, int status_code, const char* resposta) {
    struct MHD_Response* response = MHD_create_response_from_buffer(strlen(resposta), (void*)resposta, MHD_RESPMEM_PERSISTENT);
    adicionarCabecalhosCORS(response); // Adicionar os cabeçalhos CORS
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

    // Buscar Pokémon pelo ID
    Pokemon* pokemon = p[id - 1];
    if (!pokemon) {
        return gerarResposta(connection, MHD_HTTP_NOT_FOUND, "Pokémon não encontrado.");
    }
    printf("Gerando JSON para ID: %d\n", id);

    // Criar resposta JSON
    char json[2048] = {0};
    int tamanho = snprintf(json, sizeof(json),
              "{"
             "\"id\": %d,"
             "\"name\": \"%s\","
             "\"description\": \"%s\","
             "\"type\": \"%s\","
             "\"abilities\": \"%s\","
             "\"weight\": %.1f,"
             "\"height\": %.1f,"
             "\"capture_rate\": %d,"
             "\"is_legendary\": %s,"
             "\"generation\": %d,"
             "\"capture_date\": \"%02d/%02d/%d\""
             "}",
             pokemon->id, pokemon->name, pokemon->description, listaParaString(&pokemon->type), listaParaString(&pokemon->abilities), pokemon->weight, pokemon->height,
             pokemon->capture_rate, booleanToString(pokemon->is_legendary), pokemon->generation,
             pokemon->captureDate.tm_mday, pokemon->captureDate.tm_mon + 1, pokemon->captureDate.tm_year + 1900);
    printf("Pokemon gerado\n");
    
    if (tamanho < 0 || tamanho >= sizeof(json)) {
    return gerarResposta(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, "Erro ao gerar JSON.");
    }
    return gerarResposta(connection, MHD_HTTP_OK, json);
}

int main(int argc, char** argv) {
    
    startV();//carregar csv
// Iniciar servidor
    struct MHD_Daemon* daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, 8080, NULL, NULL, &processaRequisicao, NULL, MHD_OPTION_END);
    if (!daemon) {
        printf("Erro ao iniciar o servidor.\n");
        return 1;
    }

    printf("Servidor rodando na porta 8080. Pressione Ctrl+C para encerrar.\n");
    getchar();

    MHD_stop_daemon(daemon);

    return 0;
}






