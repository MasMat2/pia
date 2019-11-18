#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int menu(struct cliente *cliente);
int informacion(struct cliente *cliente);
int retiros(struct cliente *clientes);
float depositos(struct cliente *cliente);
int transferencias(struct cliente *cliente, int n, int actual);

int is_int(char *arr);
float is_float(char *arr);
void wait_for_input(void);

int main()
{
    int i, n;
    int op = 5, on;
    int actual;
    char numero[20];
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
    //Llenado de la estructura finalizado

    do
    {
        do
        {
            actual = login(clientes, n);
        } while (actual == -1);
        do
        {
            system(clear);
            op = menu(&clientes[actual]);
            switch (op)
            {
            case 1:
                informacion(&clientes[actual]);
                break;
            case 2:
                retiros(&clientes[actual]);
                break;
            case 3:
                depositos(&clientes[actual]);
                break;
            case 4:
                // transferencias(clientes, n, actual);
                break;
            }
        } while (op != 5);
        update(fp, clientes, n);
        printf("Desea apagar el cajero automatico (0-Si/1-No): ");
        fflush(stdin);
        fgets(numero, 20, stdin);
        while ((on = is_int(numero)) == -1)
        {
            printf("Ingrese un numero entero: ");
            fflush(stdin);
            fgets(numero, 20, stdin);
        }
    } while (on);

    return 0;
}

int login(struct cliente clientes[], int n)
{
    system(clear);
    int no_cuenta, index;
    int pswd, try_acc = 0, try_pswd = 0;
    char numero[20];

    //Obtener numero de cuenta y validar:
    do
    {
        printf("Ingrese su numero de cuenta: ");
        fflush(stdin);
        fgets(numero, 20, stdin);
        while ((no_cuenta = is_int(numero)) == -1)
        {
            printf("Ingrese un numero entero: ");
            fflush(stdin);
            fgets(numero, 20, stdin);
        }
        //Verificar que pertenezca a un cliente
        index = find_acc(no_cuenta, clientes, n);
        try_acc++;
        //Repetir mientras el indice sea -1 o el numero de intentos sea menor a 3
    } while (index == -1 && try_acc < 3);
    //Si hubo 3 intentos fallidos regresar error (-1)
    if (try_acc == 3)
    {
        return -1;
    }

    //Obtener contrasena y validar
    do
    {
        printf("Ingrese su contrasena: ");
        fflush(stdin);
        fgets(numero, 20, stdin);
        while ((pswd = is_int(numero)) == -1)
        {
            printf("Ingrese un numero entero: ");
            fflush(stdin);
            fgets(numero, 20, stdin);
        }
        try_pswd++;
        //Verificar que la contrasena indicada sea igual a la contrasena del usuario seleccionado
    } while (pswd != clientes[index].pswd && try_pswd < 3);
    //Si hubo 3 intentos fallidos regresar error (-1)
    if (try_pswd == 3)
    {
        return -1;
    }

    //Regresar el indice del usuario logeado en la estructura
    return index;
}

int find_acc(int no_cuenta, struct cliente clientes[], int n)
{
    int index;
    //Recorrer lista de clientes, si el numero de cliente es igual
    //al parametro no_cuenta, regresar el indice del cliente
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
    //Regresar el apuntador al inicio del archivo
    rewind(fp);
    //Escribir el numero de clientes que se guardaran
    fprintf(fp, "%d\n", n);
    //Escribir cada la estuctura de cada uno de los clientes
    for (i = 0; i < n; i++)
    {
        fprintf(fp, "%d %d %s %s %d %f %d\n", clientes[i].no_list, clientes[i].no_cliente, clientes[i].nombre, clientes[i].apellido, clientes[i].pswd, clientes[i].balance, clientes[i].no_transfer);
    }
    return 0;
}

int menu(struct cliente *cliente)
{
    int op;
    char numero[20];
    do
    {
        system(clear);
        printf("\nBienvenido %s %s\n", cliente->nombre, cliente->apellido);
        printf("\nElgia la opcion que desse: ");
        printf("\n1- Obtener informacion de la cuenta");
        printf("\n2- Realizar un retiro");
        printf("\n3- Realizar un deposito");
        printf("\n4- Realizar una transferenecia");
        printf("\n5- Salir");
        printf("\nOpcion: ");
        fflush(stdin);
        fgets(numero, 20, stdin);
        while ((op = is_int(numero)) == -1)
        {
            printf("Ingrese un numero entero: ");
            fflush(stdin);
            fgets(numero, 20, stdin);
        }
    } while (op < 1 || op > 5);
    return op;
}

int informacion(struct cliente *cliente)
{
    system(clear);
    printf("Informacion de la cuenta:");
    printf("\n%-24s %s %s", "Nombre:", cliente->nombre, cliente->apellido);
    printf("\n%-24s %d", "Numero de cuenta:", cliente->no_cliente);
    printf("\n%-24s %d", "Numero de transferencia:", cliente->no_transfer);
    printf("\n%-24s %.2f", "Balance:", cliente->balance);
    wait_for_input();
    return 1;
}
int retiros(struct cliente *cliente)
{
    float retiro;
    int op, aux;
    char numero[20];
    system(clear);
    do
    {
        do
        {
            aux = 0;
            printf("Balance actual: %.2f\nIngreasa la cantidad que desea retirar: ", cliente->balance);
            fflush(stdin);
            fgets(numero, 20, stdin);
            while ((retiro = is_float(numero)) == -1)
            {
                printf("Ingrese un numero entero: ");
                fflush(stdin);
                fgets(numero, 20, stdin);
            }
            //Validamos que la cantidad ingresada no sea mayor a el balance del usuario
            if (retiro > cliente->balance)
            {
                system(clear);
                printf("La cantidad a retirar es mayor a su balance, ingrese otra cantidad:\n\n");
                aux = 1;
            }
        } while (aux == 1);
        printf("Es correcta la cantidad?\n[1]-Si\n[2]-No\nOpcion: ");
        fflush(stdin);
        fgets(numero, 20, stdin);
        while ((op = is_int(numero)) == -1)
        {
            printf("Ingrese un numero entero: ");
            fflush(stdin);
            fgets(numero, 20, stdin);
        }
    } while (op == 2);
    //Actualizamos el balance del usuario actual
    cliente->balance = cliente->balance - retiro;
    printf("Su balance actual es %.2f", cliente->balance);
    wait_for_input();
    return 1;
}
float depositos(struct cliente *cliente)
{
    float deposito;
    char numero[20];
    system(clear);
    printf("Ingrese la cantidad que quiere depositar: ");
    fflush(stdin);
    fgets(numero, 20, stdin);
    while ((deposito = is_float(numero)) == -1)
    {
        printf("Ingrese un numero entero: ");
        fflush(stdin);
        fgets(numero, 20, stdin);
    }
    cliente->balance += deposito;
    printf("Se depositaron %.2f pesos en su cuenta\nSu balance actual es %.2f", deposito, cliente->balance);
    wait_for_input();
    return deposito;
}

int integer_validation(char *arr)
{
    while (*arr != '\0' && *arr != '\n')
    {
        if (*arr < 48 || *arr > 57)
        {
            return -1;
        }
    }
    return atoi(arr);
}

float float_validation(char *arr)
{
    int periods = 0;
    while (*arr != '\0' && *arr != '\n')
    {
        if (*arr < 48 || *arr > 57)
        {
            if (*arr == '.' || *arr == ',')
            {
                periods++;
                if (periods == 2)
                {
                    return -1.0;
                }
            }
            else
            {
                return -1.0;
            }
        }
    }
    return atof(arr);
}

int is_int(char arr[])
{
    char entrada[20];
    strcpy(entrada, arr);
    while (*arr != '\0' && *arr != '\n')
    {
        if (*arr < 48 || *arr > 57)
        {
            return -1;
        }
        arr++;
    }
    return atoi(entrada);
}

float is_float(char arr[])
{
    char entrada[20];
    strcpy(entrada, arr);
    int periods = 0;
    while (*arr != '\0' && *arr != '\n')
    {
        if (*arr < 48 || *arr > 57)
        {
            if (*arr == '.' || *arr == ',')
            {
                periods++;
                if (periods == 2)
                {
                    return -1;
                }
            }
            else
            {
                return -1;
            }
        }
        arr++;
    }
    return atof(entrada);
}
void wait_for_input(void)
{
    char c;
    printf("\n\nPresione enter para continuar: ");
    getchar();
    fflush(stdin);
    scanf("%c", &c);
}
