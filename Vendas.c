//  Carolina de Jesus Menezes   RA: 1230208153
//  Iris da Silva Reis          RA: 1230212137
//  Matheus Vitor Ferreira      RA: 1230204711
//  Lucas Suares da Silva       RA: 1230105076
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Armazena os dados de data
struct Data
{
   int dia;
   int mes;
   int ano;
};

// Armazena os dados do Cliente 
struct Cliente {
    int codigo;
    char nome_cliente[100];
};

// Armazena os dados de venda 
struct Venda {
    char codigo_venda[6]; 
    char nome_produto[100];
    char marca[100];
    int qtd_itens;
    int preco_unitario;
    float preco_total;
    float desconto;
    int codigo_cliente;
    struct Data data; 
};

// Armazena o codigo da venda e uma variavel contadora
struct RelatorioVenda {
    char codigo_venda[6];
    int count;
    char nome_produto[100];
};

// Enum para facilitar o direcionamento no menu
enum Operacoes {
    REGISTRAR_VENDA = 1,
    CADASTRAR_CLIENTE,
    GERAR_RELATORIO,
    FINALIZAR
};

struct MapaQuantidade {
    char codigo_produto[7];
    char nome_produto[100];
    int quantidade_vendida;
};

// Instanciamento de Funções do Codigo
void titulo();
void imprimirVenda(struct Venda venda);
float calcular_preco_total_com_desconto(struct Venda venda);
void RegistrarCliente(FILE* arquivo);
void escrever_venda(struct Venda venda, FILE* arquivo);
void escrever_cliente(struct Cliente cliente, FILE* arquivo);
struct Cliente* buscarClientePorCodigo(int codigo, FILE* arquivo_clientes);
void registrar_Vendas(FILE* arquivo_clientes, FILE* arquivo_vendas);
void identificarItensMaisEMenosVendidos(struct Venda* vendas, int num_vendas);
void ler_vendas(struct Venda** vendas, int* num_vendas, FILE* arquivo, struct Data data);
void gerar_relatorio(FILE* arquivo_vendas, FILE* arquivo_clientes);
int compararValorTotalVenda(const void* a, const void* b);
int contador_clientesVendas(FILE* arquivo_vendas);
void consultar_por_Data(FILE* arquivo_vendas, struct Data data_consulta);
void gerar_codigo_venda(char codigo[], int tamanho);

// Titulo 
void titulo()
{
    printf("|-----------------------------------|\n");
    printf("|          Programa de Vendas       |\n");
    printf("|         Autores:                  |\n");
    printf("|---------------------------------- |\n");
    printf("| Carolina de Jesus Menezes         |\n");
    printf("| Iris da Silva Reis                |\n");
    printf("| Matheus Vitor Ferreira            |\n");
    printf("| Lucas Suares da Silva             |\n");
    printf("|-----------------------------------|\n\n");
}

// Passa os dados de cada venda para o Arquivo.Dat
void escrever_venda(struct Venda venda, FILE* arquivo)
{
    fprintf(arquivo, "%s;%s;%s;%d;%d;%.2f;%.2f;%d;%02d/%02d/%04d\n", venda.codigo_venda, venda.nome_produto, venda.marca, venda.qtd_itens, venda.preco_unitario, venda.preco_total, venda.desconto, venda.codigo_cliente, venda.data.dia, venda.data.mes, venda.data.ano);
}

// Passa os dados de cada cliente para o Arquivo.Dat
void escrever_cliente(struct Cliente cliente, FILE* arquivo)
{
    fprintf(arquivo, "%d;%s\n", cliente.codigo, cliente.nome_cliente);

}
void gerar_codigo(char codigo[])
{
    const char caracteres[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int tamanho = sizeof(caracteres) - 1;

    srand((unsigned int)time(NULL));
    for (int i = 0; i < 5; ++i)
    {
        codigo[i] = caracteres[rand() % tamanho];
    }
    codigo[5] = '\0'; // Adiciona o caractere nulo no final
}
// Função para buscar o cliente com o seu código de identificação
struct Cliente* buscarClientePorCodigo(int codigo, FILE* arquivo_clientes)
{
    struct Cliente* cliente = (struct Cliente*)malloc(sizeof(struct Cliente));
    if (cliente == NULL)
    {
        printf("Erro ao alocar memória para cliente.\n");
        exit(1);
    }

    rewind(arquivo_clientes);

    while (fscanf(arquivo_clientes, "%d;%[^\n]\n", &cliente->codigo, cliente->nome_cliente) == 2)
    {
        if (cliente->codigo == codigo)
        {
            return cliente;
        }
    }
    free(cliente);
    return NULL;
}
void consultar_por_Data(FILE* arquivo_vendas, struct Data data_consulta) {
    struct Venda venda;
    int encontrada = 0;

    rewind(arquivo_vendas);

    while (fscanf(arquivo_vendas, "%6[^;];%[^;];%[^;];%d;%d;%f;%f;%d;%d/%d/%d\n",
                  venda.codigo_venda, venda.nome_produto, venda.marca,
                  &venda.qtd_itens, &venda.preco_unitario,
                  &venda.preco_total, &venda.desconto,
                  &venda.codigo_cliente, &venda.data.dia, &venda.data.mes, &venda.data.ano) == 11) {

        if (venda.data.dia == data_consulta.dia && venda.data.mes == data_consulta.mes && venda.data.ano == data_consulta.ano) {
            encontrada = 1;
        }
    }

    if (!encontrada) {
        printf("Nenhuma venda encontrada para a data especificada.\n");
    }
}


void registrar_Vendas(FILE* arquivo_clientes, FILE* arquivo_vendas)
{
    struct Venda nova_venda;
    struct Cliente* cliente_encontrado;
    struct Data data;

    gerar_codigo(nova_venda.codigo_venda);

    do
    {
        titulo();
        printf("Registrar Venda:\n\n");

        printf("Codigo do cliente: ");
        int codigo_cliente;
        scanf("%d", &codigo_cliente);

        cliente_encontrado = buscarClientePorCodigo(codigo_cliente, arquivo_clientes);

        if (cliente_encontrado == NULL)
        {
            system("cls");
            printf("Cliente não cadastrado! Escolha uma das opções abaixo:\n");
            printf("\n1. Fornecer um codigo diferente\n");
            printf("2. Voltar ao menu\n");
            printf("Escolha uma opção: ");
            int opcao;
            scanf("%d", &opcao);
            if (opcao == 2)
            {
                return;
            }
        }
        else
        {
            break;
        }

    } while (1);

    nova_venda.codigo_cliente = cliente_encontrado->codigo;

    printf("Informe a Data (DD/MM/YYYY): ");
    scanf("%d/%d/%d", &nova_venda.data.dia, &nova_venda.data.mes, &nova_venda.data.ano);

    printf("Nome: ");
    getchar();
    scanf(" %99[^\n]", nova_venda.nome_produto);

    printf("Marca: ");
    scanf(" %99[^\n]", nova_venda.marca);

    printf("Quantidade de Itens: ");
    scanf("%d", &nova_venda.qtd_itens);

    printf("Preço Unitário: ");
    scanf("%d", &nova_venda.preco_unitario);

    nova_venda.preco_total = (float)(nova_venda.qtd_itens * nova_venda.preco_unitario);

    if (nova_venda.qtd_itens >= 3)
    {
        nova_venda.desconto = nova_venda.preco_total * 0.1;
    }
    else
    {
        nova_venda.desconto = 0.0;
    }

    system("cls");
    imprimirVenda(nova_venda);
    char resposta[10];
    printf("\n");
    printf("Deseja confirmar a venda? (sim/nao): ");
    scanf("%s", resposta);

    if (strcmp(resposta, "sim") == 0)
    {
        escrever_venda(nova_venda, arquivo_vendas);
        sleep(2);
    }
    else
    {
        printf("Venda cancelada. Retornando ao menu.\n");
        sleep(2);
    }
}
// Função para imprimir todos os dados da venda
void imprimirVenda(struct Venda venda)
{
    struct Data data;

    printf("\n");
    printf("Data informada: %02d/%02d/%04d\n", venda.data.dia, venda.data.mes, venda.data.ano);
    printf("Código Venda: %s\n", venda.codigo_venda);
    printf("Nome do Produto: %s\n", venda.nome_produto);
    printf("Marca: %s\n", venda.marca);
    printf("Quantidade de Itens: %d\n", venda.qtd_itens);
    printf("Preço Unitário: %d\n", venda.preco_unitario);
    printf("Preço Total com Desconto: %.2f\n", venda.preco_total);
    printf("Desconto: %.2f\n", venda.desconto);
}
// Função para calcular o desconto com restrição de apenas para itens com quantidades acima ou igual a 3
float calcular_preco_total_com_desconto(struct Venda venda)
{
    float preco_total = venda.preco_unitario * venda.qtd_itens;

    if (venda.qtd_itens >= 3)
    {
        preco_total *= 0.9;
    }

    return preco_total;
}

// Função para registrar um cliente
// strcspn tem como objetivo eliminar o caractere de nova linha (\n)
// escrever_cliente é uma função feita anteriormente para passar os dados para o arquivo.dat
void RegistrarCliente(FILE* arquivo)
{
    struct Cliente novo_cliente;
    printf("* \tRegistrar Cliente\t*\n");
    printf("\n\nCodigo: ");
    scanf("%d", &novo_cliente.codigo);

    printf("Nome: ");
    fflush(stdin);
    fgets(novo_cliente.nome_cliente, sizeof(novo_cliente.nome_cliente), stdin);
    novo_cliente.nome_cliente[strcspn(novo_cliente.nome_cliente, "\n")] = '\0';

    escrever_cliente(novo_cliente, arquivo);
}

// A memória para armazenar as vendas é realocada usando realloc
// - 1 é o índice da última posição do array, já que os índices começam em 0
void ler_vendas(struct Venda** vendas, int* num_vendas, FILE* arquivo, struct Data data) {
    rewind(arquivo); 
    char codigo_venda[7];
    char nome_produto[100];
    char marca[100];
    int qtd_itens;
    int preco_unitario;
    float preco_total;
    float desconto;
    int codigo_cliente;
    int dia, mes, ano;

    *num_vendas = 0;
    size_t capacidade = 10; 
    *vendas = (struct Venda*)malloc(capacidade * sizeof(struct Venda)); 

    if (*vendas == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    // Enquanto houver vendas para ler no arquivo
    while (fscanf(arquivo, "%6[^;];%99[^;];%99[^;];%d;%d;%f;%f;%d;%d/%d/%d\n",
        codigo_venda, nome_produto, marca, &qtd_itens, &preco_unitario,
        &preco_total, &desconto, &codigo_cliente, &dia, &mes, &ano) == 11) {
        
        // Verifica se a data da venda é igual à data especificada pelo usuário
        if (dia == data.dia && mes == data.mes && ano == data.ano) {
            // Verifica se é necessário realocar mais memória
            if (*num_vendas >= capacidade) {
                capacidade *= 2; // Dobra a capacidade
                *vendas = (struct Venda*)realloc(*vendas, capacidade * sizeof(struct Venda));
                if (*vendas == NULL) {
                    printf("Erro ao realocar memória.\n");
                    exit(1);
                }
            }

            // Preenche a estrutura de venda atual
            strcpy((*vendas)[*num_vendas].codigo_venda, codigo_venda);
            strcpy((*vendas)[*num_vendas].nome_produto, nome_produto);
            strcpy((*vendas)[*num_vendas].marca, marca);
            (*vendas)[*num_vendas].qtd_itens = qtd_itens;
            (*vendas)[*num_vendas].preco_unitario = preco_unitario;
            (*vendas)[*num_vendas].preco_total = preco_total;
            (*vendas)[*num_vendas].desconto = desconto;
            (*vendas)[*num_vendas].codigo_cliente = codigo_cliente;
            (*vendas)[*num_vendas].data.dia = dia;
            (*vendas)[*num_vendas].data.mes = mes;
            (*vendas)[*num_vendas].data.ano = ano;

            (*num_vendas)++;
        }
    }
}
void identificarItensMaisEMenosVendidos(struct Venda* vendas, int num_vendas) {
    if (num_vendas == 0) {
        printf("Nenhuma venda registrada.\n");
        return;
    }

    // Inicializar o mapa com os códigos de produto únicos e a quantidade vendida como zero
    struct MapaQuantidade mapa_quantidade[num_vendas];
    for (int i = 0; i < num_vendas; i++) {
        strcpy(mapa_quantidade[i].codigo_produto, vendas[i].codigo_venda);
        strcpy(mapa_quantidade[i].nome_produto, vendas[i].nome_produto);
        mapa_quantidade[i].quantidade_vendida = 0;
    }

    // Preencher o mapa com a quantidade vendida de cada produto
    for (int i = 0; i < num_vendas; i++) {
        for (int j = 0; j < num_vendas; j++) {
            if (strcmp(mapa_quantidade[j].codigo_produto, vendas[i].codigo_venda) == 0) {
                mapa_quantidade[j].quantidade_vendida += vendas[i].qtd_itens;
                break;
            }
        }
    }

    // Encontrar a quantidade máxima e mínima de vendas
    int max_vendas = mapa_quantidade[0].quantidade_vendida;
    int min_vendas = mapa_quantidade[0].quantidade_vendida;

    for (int i = 1; i < num_vendas; i++) {
        if (mapa_quantidade[i].quantidade_vendida > max_vendas) {
            max_vendas = mapa_quantidade[i].quantidade_vendida;
        }
        if (mapa_quantidade[i].quantidade_vendida < min_vendas) {
            min_vendas = mapa_quantidade[i].quantidade_vendida;
        }
    }

    // Mostrar todos os itens mais vendidos
    printf("\nItens mais vendidos:\n");
    for (int i = 0; i < num_vendas; i++) {
        if (mapa_quantidade[i].quantidade_vendida == max_vendas) {
            printf("Código: %s - Nome: %s - %d itens\n", mapa_quantidade[i].codigo_produto, mapa_quantidade[i].nome_produto, max_vendas);
        }
    }

    // Mostrar todos os itens menos vendidos
    printf("\nItens menos vendidos:\n");
    for (int i = 0; i < num_vendas; i++) {
        if (mapa_quantidade[i].quantidade_vendida == min_vendas) {
            printf("Código: %s - Nome: %s - %d itens\n", mapa_quantidade[i].codigo_produto, mapa_quantidade[i].nome_produto, min_vendas);
        }
    }
}

int contador_clientesVendas(FILE* arquivo_vendas)
{
    rewind(arquivo_vendas); // Voltar para o início do arquivo de vendas
    int contador_vendas_por_cliente[100] = { 0 }; // Inicializar o contador de vendas para cada cliente

    struct Venda venda;
    while (fscanf(arquivo_vendas, "%d;%[^;];%[^;];%d;%d;%f;%f;%d\n",
        &venda.codigo_venda, venda.nome_produto, venda.marca,
        &venda.qtd_itens, &venda.preco_unitario,
        &venda.preco_total, &venda.desconto,
        &venda.codigo_cliente) == 8) {
        contador_vendas_por_cliente[venda.codigo_cliente]++;
    }

    int total_clientes_compras = 0;

    // Iterar sobre os contadores para contar quantos clientes compraram pelo menos uma vez
    for (int i = 0; i < 100; i++) {
        if (contador_vendas_por_cliente[i] > 0) {
            total_clientes_compras++;
        }
    }

    return total_clientes_compras;
}

//Comparar dados de uma venda e utilizaram o valor total como parametro para ordenar
int compararValorTotalVenda(const void* item_a, const void* item_b)
{
    //Ponteiros génericos que serão utilizados para passar duas vendas para comparar
    const struct Venda* vendaA = (const struct Venda*)item_a;
    const struct Venda* vendaB = (const struct Venda*)item_b;

    if (vendaA->preco_total < vendaB->preco_total)
        return 1;
    else if (vendaA->preco_total > vendaB->preco_total)
        return -1;
    else
        return 0;
}

//Função responsavel em gerar o relatório
void gerar_relatorio(FILE* arquivo_vendas, FILE* arquivo_clientes)
{
    struct Venda* vendas = NULL;
    struct Data data;
    int num_vendas = 0;
    printf("\n=>   RELATÓRIO DIÁRIO DE VENDAS   <=\n\n");
    printf("Informe a data para consulta diária (DD/MM/YYYY): ");
    scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
    fflush(stdin);

    consultar_por_Data(arquivo_vendas, data); 
    system("cls");
    ler_vendas(&vendas, &num_vendas, arquivo_vendas, data);
    if (num_vendas == 0) {
        printf("Nenhuma venda registrada para a data especificada.\n");
        printf("\n\nPressione qualquer tecla para voltar ao menu...\n");
        getchar();
        return;
    }

    printf("\n=>   RELATÓRIO DIÁRIO DE VENDAS   <=\n\n");
    printf("| %-10s | %-25s | %-10s | %-10s | %-12s | %-15s | %-12s |\n",
           "Código", "Produto", "Marca", "Qtd. Itens", "Cód. Cliente", "Preço Unitário", "Valor Total");
    printf("-------------------------------------------------------------------------------------------------\n");

    float faturamento_bruto = 0.0;

    for (int i = 0; i < num_vendas; i++)
    {
        struct Venda venda = vendas[i];
        float preco_total_com_desconto = calcular_preco_total_com_desconto(venda);
        faturamento_bruto += preco_total_com_desconto;

        printf("| %-10s | %-25s | %-10s | %-10d | %-12d | %-15d | %-12.2f |\n",
               venda.codigo_venda, venda.nome_produto, venda.marca,
               venda.qtd_itens, venda.codigo_cliente, venda.preco_unitario, preco_total_com_desconto);
    }

    printf("\nTotal de vendas: %d\n", num_vendas);
    printf("Total de clientes que realizaram compras: %d\n", contador_clientesVendas(arquivo_vendas));
    printf("Faturamento bruto: R$ %.2f\n", faturamento_bruto);

    // Identificar itens mais e menos vendidos
    identificarItensMaisEMenosVendidos(vendas, num_vendas);

    free(vendas);

    printf("\n\nPressione qualquer tecla para voltar ao menu...\n");
    getchar();
}

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

    FILE* arquivo_vendas;
    arquivo_vendas = fopen("vendas.dat", "a+");

    FILE* arquivo_clientes;
    arquivo_clientes = fopen("clientes.dat", "a+");

    if (arquivo_vendas == NULL)
    {
        perror("Erro ao abrir arquivo vendas.");
        return 1;
    }

    if (arquivo_clientes == NULL)
    {
        perror("Erro ao abrir arquivo clientes.");
        return 1;
    }

    enum Operacoes operacoes;

    do
    {
        titulo();
        printf("\nMenu:\n");
        printf("1. Cadastrar Venda\n");
        printf("2. Cadastrar cliente\n");
        printf("3. Relatórios Diarios\n");
        printf("4. Finalizar venda\n\n");
        printf("Escolha uma opção: ");
        scanf("%u", &operacoes);

        switch (operacoes)
        {
        case REGISTRAR_VENDA:
            system("CLS");
            registrar_Vendas(arquivo_clientes, arquivo_vendas);
            system("CLS");
            break;

        case CADASTRAR_CLIENTE:
            system("CLS");
            RegistrarCliente(arquivo_clientes);
            system("CLS");
            break;

        case GERAR_RELATORIO:
            system("CLS");
            gerar_relatorio(arquivo_vendas, arquivo_clientes);
            system("CLS");
            break;

        case FINALIZAR:
            break;

        default:
            printf("Opção inválida. Tente novamente.\n");
            sleep(2);
            system("CLS");
            break;
        }

    } while (operacoes != FINALIZAR);

    fclose(arquivo_vendas);
    fclose(arquivo_clientes);

    return 0;
}