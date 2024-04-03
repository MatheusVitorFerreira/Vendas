#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

void titulo();

struct Cliente {
    int codigo;
    char nome_cliente[100];
};

struct Venda {
    int codigo;
    char nome_produto[100];
    char marca[100];
    int qtd_itens;
    int preco_unitario;
    float preco_total;
    float desconto;
    struct Cliente *cliente;
};

struct RelatorioVenda {
    int codigo_venda;
    int count;
};

enum Operacoes {
    REGISTRAR_VENDA = 1,
    CADASTRAR_CLIENTE,
    GERAR_RELATORIO,
    FINALIZAR
};

void imprimirVenda(struct Venda venda);
float calcular_preco_total_com_desconto(struct Venda venda);
void RegistrarCliente(FILE* arquivo);
void escrever_venda(struct Venda venda, FILE *arquivo);
void escrever_cliente(struct Cliente cliente, FILE *arquivo);
struct Cliente* buscarClientePorCodigo(int codigo, FILE* arquivo_clientes);
void registrar_Vendas(FILE* arquivo_clientes, FILE* arquivo_vendas);
void identificarItensMaisEMenosVendidos(struct Venda* vendas, int num_vendas);
void ler_vendas(struct Venda** vendas, int* num_vendas, FILE *arquivo);
int ler_quantidade_clientes(FILE *arquivo);
void gerar_relatorio(FILE *arquivo_vendas, FILE* arquivo_clientes);

// Função titulo
void titulo() {
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

void escrever_venda(struct Venda venda, FILE *arquivo) {
    fwrite(&venda, sizeof(struct Venda), 1, arquivo);
}

void escrever_cliente(struct Cliente cliente, FILE *arquivo) {
    fwrite(&cliente, sizeof(struct Cliente), 1, arquivo);
}

struct Cliente* buscarClientePorCodigo(int codigo, FILE* arquivo_clientes) {
    struct Cliente* cliente = (struct Cliente*)malloc(sizeof(struct Cliente));
    if (cliente == NULL) {
        printf("Erro ao alocar memória para cliente.\n");
        exit(1);
    }

    // Reinicializar o ponteiro do arquivo para o início
    rewind(arquivo_clientes);

    while (fread(cliente, sizeof(struct Cliente), 1, arquivo_clientes) == 1) {
        if (cliente->codigo == codigo) {
            return cliente;
        }
    }
    free(cliente);
    return NULL;
}

void registrar_Vendas(FILE* arquivo_clientes, FILE* arquivo_vendas) {
    struct Venda nova_venda;
    struct Cliente *cliente_encontrado;

    do {
        titulo();
        printf("Registrar Venda:\n\n");

        printf("Codigo do cliente: ");
        int codigo_cliente;
        scanf("%d", &codigo_cliente);

        cliente_encontrado = buscarClientePorCodigo(codigo_cliente, arquivo_clientes);

        if (cliente_encontrado == NULL) {
            printf("Cliente não cadastrado! Escolha uma das opções abaixo:\n\n");
            printf("1. Fornecer um codigo diferente\n");
            printf("2. Voltar ao menu\n");
            printf("Escolha uma opção: ");
            int opcao;
            scanf("%d", &opcao);
            if (opcao == 2) {
                return;
            }
        } else {
            break;
        }

    } while (1);

    nova_venda.cliente = cliente_encontrado;

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

    nova_venda.preco_total = (float)(nova_venda.qtd_itens * nova_venda.preco_unitario);
    printf("Preço total: %.2f\n\n", nova_venda.preco_total);
    imprimirVenda(nova_venda);
    char resposta[10];
    printf("\n");
    printf("Deseja confirmar a venda? (sim/nao): ");
    scanf("%s", resposta);

    if (strcmp(resposta, "sim") == 0) {
        imprimirVenda(nova_venda);
        escrever_venda(nova_venda, arquivo_vendas);
        printf("Venda registrada com sucesso!\n");
    } else {
        printf("Venda cancelada. Retornando ao menu.\n");
    }
}

void imprimirVenda(struct Venda venda) {
    printf("\n");
    printf("Código: %d\n", venda.codigo);
    printf("Nome do Produto: %s\n", venda.nome_produto);
    printf("Marca: %s\n", venda.marca);
    printf("Quantidade de Itens: %d\n", venda.qtd_itens);
    printf("Preço Unitário: %d\n", venda.preco_unitario);
    float preco_total_com_desconto = calcular_preco_total_com_desconto(venda);
    printf("Preço Total com Desconto: %.2f\n", preco_total_com_desconto);
    printf("Desconto: %.2f\n", venda.preco_total - preco_total_com_desconto);
}

float calcular_preco_total_com_desconto(struct Venda venda) {
    float preco_total = venda.preco_unitario * venda.qtd_itens;

    if (venda.qtd_itens >= 3) {
        preco_total *= 0.9;
    }

    return preco_total;
}

void RegistrarCliente(FILE *arquivo) {
    struct Cliente novo_cliente;
    printf("** \tRegistrar Cliente\t**\n");
    printf("\n\nCodigo: ");
    scanf("%d", &novo_cliente.codigo);

    printf("Nome: ");
    fflush(stdin);
    fgets(novo_cliente.nome_cliente, sizeof(novo_cliente.nome_cliente), stdin);
    novo_cliente.nome_cliente[strcspn(novo_cliente.nome_cliente, "\n")] = '\0';

    escrever_cliente(novo_cliente, arquivo);
}

struct RelatorioVenda* buscarRelatorioPorCodigo(struct RelatorioVenda* relatorios, int codigo, int num_vendas) {
    for (int i = 0; i < num_vendas; i++) {
        if (relatorios[i].codigo_venda == codigo) {
            return &relatorios[i];
        }
    }
    return NULL;
}

void identificarItensMaisEMenosVendidos(struct Venda* vendas, int num_vendas) {
    struct RelatorioVenda relatorios[num_vendas];

    for (int i = 0; i < num_vendas; i++) {
        relatorios[i].codigo_venda = -1;
        relatorios[i].count = 0;
    }

    for (int i = 0; i < num_vendas; i++) {
        int codigo = vendas[i].codigo;
        struct RelatorioVenda* relatorio_encontrado = buscarRelatorioPorCodigo(relatorios, codigo, num_vendas);

        if (relatorio_encontrado == NULL) {
            for (int j = 0; j < num_vendas; j++) {
                if (relatorios[j].codigo_venda == -1) {
                    relatorios[j].codigo_venda = codigo;
                    relatorios[j].count = 1;
                    break;
                }
            }
        } else {
            relatorio_encontrado->count++;
        }
    }

    struct RelatorioVenda mais_vendido = relatorios[0];
    struct RelatorioVenda menos_vendido = relatorios[0];

    for (int i = 1; i < num_vendas; i++) {
        if (relatorios[i].codigo_venda == -1) {
            break; 
        }

        if (relatorios[i].count > mais_vendido.count) {
            mais_vendido = relatorios[i];
        }
        if (relatorios[i].count < menos_vendido.count) {
            menos_vendido = relatorios[i];
        }
    }

    printf("Item mais vendido: %d Codigo -  %d vezes)\n", mais_vendido.codigo_venda, mais_vendido.count);
    printf("Item menos vendido: %d Codigo - %d vezes)\n", menos_vendido.codigo_venda, menos_vendido.count);
}

void ler_vendas(struct Venda** vendas, int* num_vendas, FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    *num_vendas = ftell(arquivo) / sizeof(struct Venda);
    rewind(arquivo);

    *vendas = (struct Venda*)malloc((*num_vendas) * sizeof(struct Venda));
    if (*vendas == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    for (int i = 0; i < *num_vendas; i++) {
        fread(&((*vendas)[i]), sizeof(struct Venda), 1, arquivo);
    }
}

int ler_quantidade_clientes(FILE *arquivo) {
    struct Cliente cliente;
    int count = 0;

    while (fread(&cliente, sizeof(struct Cliente), 1, arquivo) == 1) {
        if (cliente.codigo != 0) {
            count++;
        }
    }

    return count;
}

void gerar_relatorio(FILE *arquivo_vendas, FILE* arquivo_clientes) {
    struct Venda* vendas;
    int num_vendas = 0;
    int num_clientes = ler_quantidade_clientes(arquivo_clientes);

    ler_vendas(&vendas, &num_vendas, arquivo_vendas);

    printf("\nRelatório de Vendas:\n\n");

    printf("%-10s%-30s%-20s%-15s%-15s%-15s\n",
        "Código", "Nome do Produto", "Marca", "Qtd. Itens", "Preço (U)", "Valor Total Venda");

    float faturamento_bruto = 0.0;

    for (int i = 0; i < num_vendas; i++) {
        struct Venda venda = vendas[i];
        float preco_total_com_desconto = calcular_preco_total_com_desconto(venda);
        faturamento_bruto += preco_total_com_desconto;
        printf("%-10d%-30s%-20s%-15d%-15d%-15.2f\n", venda.codigo, venda.nome_produto, venda.marca, venda.qtd_itens, venda.preco_unitario, preco_total_com_desconto);
    }

    printf("\nTotal de vendas: %d\n", num_vendas);
    printf("Total clientes que realizaram compras: %d\n", num_clientes);
    printf("Faturamento bruto: %.2f\n", faturamento_bruto);

    identificarItensMaisEMenosVendidos(vendas, num_vendas);

    // Liberar a memória alocada para o array de vendas
    free(vendas);

    printf("\n\nPressione qualquer tecla para voltar ao menu...\n");
    getchar();
    getchar();
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    FILE *arquivo_vendas;
    arquivo_vendas = fopen("vendas.bin", "rb+");

    FILE* arquivo_clientes;
    arquivo_clientes = fopen("clientes.bin", "rb+");

    if (arquivo_vendas == NULL) {
        perror("Erro ao abrir arquivo vendas.");
        return 1;
    }

    if (arquivo_clientes == NULL) {
        perror("Erro ao abrir arquivo clientes.");
        return 1;
    }

    enum Operacoes operacoes;

    do {
        titulo();
        printf("\nMenu:\n");
        printf("1. Cadastrar nova venda\n");
        printf("2. Cadastrar cliente\n");
        printf("3. Gerar relatórios\n");
        printf("4. Finalizar venda\n\n");
        printf("Escolha uma opção: ");
        scanf("%u", &operacoes);

        switch (operacoes) {
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
                printf("\nFinalizando o programa.\n");
                break;

            default:
                printf("\nOpção inválida. Escolha novamente.\n");
        }
    } while (operacoes != FINALIZAR);

    fclose(arquivo_vendas);
    fclose(arquivo_clientes);

    system("pause");

    return 0;
}
