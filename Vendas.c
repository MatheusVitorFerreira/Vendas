#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>

// Armazena os dados do Cliente 
struct Cliente {
    int codigo;
    char nome_cliente[100];
};

// Armazena os dados de venda 
struct Venda {
    int codigo;
    char nome_produto[100];
    char marca[100];
    int qtd_itens;
    int preco_unitario;
    float preco_total;
    float desconto;
    int codigo_cliente;
};

// Armazena o codigo da venda e uma variavel contadora
struct RelatorioVenda {
    int codigo_venda;
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
void ler_vendas(struct Venda** vendas, int* num_vendas, FILE* arquivo);
void gerar_relatorio(FILE* arquivo_vendas, FILE* arquivo_clientes);
int compararValorTotalVenda(const void* a, const void* b);
int contador_clientesVendas(FILE* arquivo_vendas);

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
    fprintf(arquivo, "%d;%s;%s;%d;%d;%.2f;%.2f;%d\n", venda.codigo, venda.nome_produto, venda.marca, venda.qtd_itens, venda.preco_unitario, venda.preco_total, venda.desconto, venda.codigo_cliente);
}

// Passa os dados de cada cliente para o Arquivo.Dat
void escrever_cliente(struct Cliente cliente, FILE* arquivo)
{
    fprintf(arquivo, "%d;%s\n", cliente.codigo, cliente.nome_cliente);
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

// Função que registra uma venda após verificar se o cliente está cadastrado pelo código
void registrar_Vendas(FILE* arquivo_clientes, FILE* arquivo_vendas)
{
    struct Venda nova_venda;
    struct Cliente* cliente_encontrado;

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

    printf("Codigo do produto: ");
    scanf("%d", &nova_venda.codigo);

    printf("Nome: ");
    getchar();
    fgets(nova_venda.nome_produto, sizeof(nova_venda.nome_produto), stdin);
    nova_venda.nome_produto[strcspn(nova_venda.nome_produto, "\n")] = '\0';

    printf("Marca: ");
    fgets(nova_venda.marca, sizeof(nova_venda.marca), stdin);
    nova_venda.marca[strcspn(nova_venda.marca, "\n")] = '\0';

    printf("Quantidade de Itens: ");
    scanf("%d", &nova_venda.qtd_itens);

    printf("Preço Unitário: ");
    scanf("%d", &nova_venda.preco_unitario);

    // Calcula o preço total antes do desconto

    nova_venda.preco_total = (float)(nova_venda.qtd_itens * nova_venda.preco_unitario);

    // Aplica o desconto se necessário

    if (nova_venda.qtd_itens >= 3) {
        nova_venda.desconto = nova_venda.preco_total * 0.1; // 10% de desconto
    }
    else {
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
        imprimirVenda(nova_venda);
        escrever_venda(nova_venda, arquivo_vendas);
        printf("Venda registrada com sucesso!\n");
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
    printf("\n");
    printf("Código: %d\n", venda.codigo);
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
void ler_vendas(struct Venda** vendas, int* num_vendas, FILE* arquivo)
{
    fseek(arquivo, 0, SEEK_SET);
    *num_vendas = 0;
    struct Venda venda;

    while (fscanf(arquivo, "%d;%[^;];%[^;];%d;%d;%f;%f;%d\n",
        &venda.codigo, venda.nome_produto, venda.marca,
        &venda.qtd_itens, &venda.preco_unitario,
        &venda.preco_total, &venda.desconto,
        &venda.codigo_cliente) == 8)
    {
        (*num_vendas)++;
        *vendas = realloc(*vendas, (*num_vendas) * sizeof(struct Venda));
        (*vendas)[(*num_vendas) - 1] = venda;
    }
}

void identificarItensMaisEMenosVendidos(struct Venda* vendas, int num_vendas)
{
    if (num_vendas == 0)
    {
        printf("Nenhuma venda registrada.\n");
        return;
    }

    struct RelatorioVenda* relatorios = (struct RelatorioVenda*)malloc(num_vendas * sizeof(struct RelatorioVenda));

    if (relatorios == NULL) {
        printf("Erro ao alocar memória.");
        exit(1);
    }

    // Inicialização do relatório
    for (int i = 0; i < num_vendas; i++)
    {
        relatorios[i].codigo_venda = -1;
        relatorios[i].count = 0;
        strcpy(relatorios[i].nome_produto, ""); // Inicializa o nome do produto como uma string vazia
    }

    // Preenchimento do relatório
    for (int i = 0; i < num_vendas; i++)
    {
        int codigo = vendas[i].codigo;
        for (int j = 0; j < num_vendas; j++)
        {
            if (relatorios[j].codigo_venda == -1)
            {
                relatorios[j].codigo_venda = codigo;
                strcpy(relatorios[j].nome_produto, vendas[i].nome_produto); // Copia o nome do produto
                relatorios[j].count += vendas[i].qtd_itens;
                break;
            }
            else if (relatorios[j].codigo_venda == codigo)
            {
                relatorios[j].count += vendas[i].qtd_itens;
                break;
            }
        }
    }

    // Variáveis para armazenar os dados que foram recolhidos para o relatório
    char* mais_vendido_nome = relatorios[0].nome_produto;
    int mais_vendido_codigo = relatorios[0].codigo_venda;
    char* menos_vendido_nome = relatorios[0].nome_produto;
    int menos_vendido_codigo = relatorios[0].codigo_venda;
    int mais_vendido_qtd = relatorios[0].count;
    int menos_vendido_qtd = relatorios[0].count;

    for (int i = 1; i < num_vendas; i++)
    {
        if (relatorios[i].count > mais_vendido_qtd)
        {
            mais_vendido_codigo = relatorios[i].codigo_venda;
            mais_vendido_nome = relatorios[i].nome_produto;
            mais_vendido_qtd = relatorios[i].count;
        }
        else if (relatorios[i].count < menos_vendido_qtd)
        {
            menos_vendido_codigo = relatorios[i].codigo_venda;
            menos_vendido_nome = relatorios[i].nome_produto;
            menos_vendido_qtd = relatorios[i].count;
        }
    }

    printf("Item mais vendido:  Nome %s - Código %d - %d itens\n", mais_vendido_nome, mais_vendido_codigo, mais_vendido_qtd);
    printf("Item menos vendido: Nome %s - Código %d - %d itens\n", menos_vendido_nome, menos_vendido_codigo, menos_vendido_qtd);
}

int contador_clientesVendas(FILE* arquivo_vendas)
{
    rewind(arquivo_vendas); // Voltar para o início do arquivo de vendas
    int contador_vendas_por_cliente[100] = { 0 }; // Inicializar o contador de vendas para cada cliente

    struct Venda venda;
    while (fscanf(arquivo_vendas, "%d;%[^;];%[^;];%d;%d;%f;%f;%d\n",
        &venda.codigo, venda.nome_produto, venda.marca,
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
int compararValorTotalVenda(const void* a, const void* b)
{
    //Ponteiros génericos que serão utilizados para passar duas vendas para comparar
    const struct Venda* vendaA = (const struct Venda*)a;
    const struct Venda* vendaB = (const struct Venda*)b;

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
    int num_vendas = 0;

    ler_vendas(&vendas, &num_vendas, arquivo_vendas);

    printf("\nRelatório de Vendas:\n\n");

    printf("%-10s%-30s%-20s%-15s%-15s%-15s\n",
        "Codigo", "Nome do Produto", "Marca", "Qtd. Itens", "Preço (U)", "Valor Total Venda");

    // Ordena as vendas com base na função compararValorTotalVenda
    qsort(vendas, num_vendas, sizeof(struct Venda), compararValorTotalVenda);

    float faturamento_bruto = 0.0;

    // Itera sobre cada venda e imprime seus detalhes formatados
    for (int i = 0; i < num_vendas; i++)
    {
        struct Venda venda = vendas[i];
        float preco_total_com_desconto = calcular_preco_total_com_desconto(venda);
        faturamento_bruto += preco_total_com_desconto;
        printf("%-10d%-30s%-20s%-15d%-15d%-15.2f\n",
            venda.codigo, venda.nome_produto, venda.marca,
            venda.qtd_itens, venda.preco_unitario, preco_total_com_desconto);
    }

    printf("\nTotal de vendas: %d\n", num_vendas);
    printf("Total clientes que realizaram compras: %d\n", contador_clientesVendas(arquivo_vendas));
    printf("Faturamento bruto: %.2f\n", faturamento_bruto);

    identificarItensMaisEMenosVendidos(vendas, num_vendas);

    free(vendas);

    printf("\n\nPressione qualquer tecla para voltar ao menu...\n");
    getchar();
    getchar();
}

int main()
{
    setlocale(LC_ALL, "pt_BR");

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
        printf("1. Cadastrar nova venda\n");
        printf("2. Cadastrar cliente\n");
        printf("3. Gerar relatórios\n");
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