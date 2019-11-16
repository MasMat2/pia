#include <stdio.h>
#include <stdlib.h>

#define clear "clear"

struct cliente
{
    char nombre[20];
    char apellido[20];
    int no_cliente;
    int pswd;
    float balance;
    int no_transfer;
    int no_list;
    char archivo[30];
};

int login(struct cliente *cl, int n);
int find_acc(int no_cuenta, struct cliente *cl, int n);
int update(FILE *fp, struct cliente clientes[], int n);

int main()
{
    int i, n;
    int op = 5, on;
    int cliente_actual;
    //Abir archivo y llenar arreglo de estructuras
    struct cliente clientes[10];
    FILE *fp;
    fp = fopen("db.txt", "r+");
    if (fp == NULL)
    {
        printf("Error al abrir el archivo");
        exit(-1);
    }
    fscanf(fp, "%d", &n);
    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%d %d %s %s %d %f %d\n", &clientes[i].no_list, &clientes[i].no_cliente, clientes[i].nombre, clientes[i].apellido, &clientes[i].pswd, &clientes[i].balance, &clientes[i].no_transfer);
    }

    do
    {
        do
        {
            cliente_actual = login(clientes, n);
            update(fp, clientes, n);
        } while (cliente_actual == -1);
        do
        {
            system(clear);
            printf("\nBienvendio %s\n", clientes[cliente_actual].nombre);
            //Menu
            switch (op)
            {
            case 1:
                //Obtener informacion
                break;
            case 2:
                //Retiros
                break;
            case 3:
                //Deposito
                break;
            case 4:
                //Transferencia
                break;
            }
        } while (op != 5);
        printf("Desea apagar el cajero automatico (0-Si/1-No): ");
        scanf("%d", &on);
    } while (on);

    return 0;
}

int login(struct cliente clientes[], int n)
{
    system(clear);
    int no_cuenta, index;
    int pswd, try_acc = 0, try_pswd = 0;
    do
    {
        printf("Ingrese su numero de cuenta: ");
        scanf("%d", &no_cuenta);
        index = find_acc(no_cuenta, clientes, n);
        try_acc++;
    } while (index == -1 && try_acc < 3);
    if (try_acc == 3)
    {
        return -1;
    }

    do
    {
        printf("Ingrese su contrasena: ");
        scanf("%d", &pswd);
        try_pswd++;
    } while (pswd != clientes[index].pswd && try_pswd < 3);
    if (try_pswd == 3)
    {
        return -1;
    }
    return index;
}

int find_acc(int no_cuenta, struct cliente clientes[], int n)
{
    int index;
    for (index = 0; index < n; index++)
    {
        if (clientes[index].no_cliente == no_cuenta)
        {
            return index;
        }
    }
    return -1;
}

int update(FILE *fp, struct cliente clientes[], int n)
{
    int i;
    rewind(fp);
    fprintf(fp, "%d\n", n);
    for (i = 0; i < n; i++)
    {
        fprintf(fp, "%d %d %s %s %d %f %d\n", clientes[i].no_list, clientes[i].no_cliente, clientes[i].nombre, clientes[i].apellido, clientes[i].pswd, clientes[i].balance, clientes[i].no_transfer);
    }
    return 0;
}