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
int menu(struct cliente *cliente);
int informacion(struct cliente *cliente);
int retiros(struct cliente *clientes);
float depositos(struct cliente *cliente);
int transferencias(struct cliente *cliente, int n, int actual);

int integer_validation(char msg[], char alt_msg[]);
float float_validation(char msg[], char alt_msg[]);
int is_int(char *arr);
int is_float(char *arr);

void wait_for_input(void);

int main()
{
    int i, n;
    int op = 5, on;
    int actual;
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
                transferencias(clientes, n, actual);
                break;
            }
        } while (op != 5);
        update(fp, clientes, n);
        on = integer_validation("Desea apagar el cajero automatico (0-Si/1-No): ", "");
    } while (on);

    return 0;
}

int login(struct cliente clientes[], int n)
{
    system(clear);
    int no_cuenta, index;
    int pswd, try_acc = 0, try_pswd = 0;

    //Obtener numero de cuenta y validar:
    do
    {
        no_cuenta = integer_validation("Ingrese su numero de cuenta: ", "");
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
        pswd = integer_validation("Ingrese su contrasena: ", "");
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
    do
    {
        system(clear);
        printf("\nBienvenido %s %s\n", cliente->nombre, cliente->apellido);
        printf("\nEliga la opcion que desee: ");
        printf("\n1- Obtener informacion de la cuenta");
        printf("\n2- Realizar un retiro");
        printf("\n3- Realizar un deposito");
        printf("\n4- Realizar una transferenecia");
        printf("\n5- Salir");
        op = integer_validation("\nOpcion: ", "");
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
    system(clear);
    do
    {
        do
        {
            aux = 0;
            printf("Balance actual: %.2f\n", cliente->balance);
            retiro = float_validation("Ingreasa la cantidad que desea retirar: ", "");
            //Validamos que la cantidad ingresada no sea mayor a el balance del usuario
            if (retiro > cliente->balance)
            {
                system(clear);
                printf("La cantidad a retirar es mayor a su balance, ingrese otra cantidad:\n\n");
                aux = 1;
            }
        } while (aux == 1);
        op = integer_validation("Es correcta la cantidad?\n[1]-Si\n[2]-No\nOpcion: ", "");
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
    system(clear);
    deposito = float_validation("Ingrese la cantidad que quiere depositar: ", "");
    cliente->balance += deposito;
    printf("Se depositaron %.2f pesos en su cuenta\nSu balance actual es %.2f", deposito, cliente->balance);
    wait_for_input();
    return deposito;
}

int transferencias(struct cliente *cliente, int n, int actual)
{
    float transfer;
    int no_trans, i, aux, op = 0;
    struct cliente *ap;
    system(clear);
    //Validamos que la cantidad ingresada no sea mayor a el balance del usuario
    do
    {
        aux = 0;
        printf("Balance actual: %.2f\n\nIngreasa la cantidad que desea transferir: ", cliente->balance);
        transfer = float_validation("", "");
        if (transfer > cliente->balance)
        {
            system(clear);
            printf("\nLa cantidad a transferir esta por encima de su balance, intente otra vez: \n");
            aux = 1;
        }
    } while (aux == 1);
    do
    {
        //Validamos que el numero de transferencia exista antes de realizar una operacion
        do
        {
            printf("\nIngrese el numero de transferencia del destinatario: ");
            no_trans = integer_validation("", "");
            //Usamos otro puntero para no afectar el puntero principal
            ap = cliente;
            //Reiniciamos el puntero
            ap = ap - actual;
            aux = 1;
            for (i = 0; i < n; i++)
            {
                //Checars todos los numeros de transferencias hasta encontrar el ingresado por el usuario
                if (no_trans == ap->no_transfer)
                {
                    printf("\nNumero de transferencia encontrado satisfactoriamente");
                    aux = 0;
                }
                ap++;
            }
            if (aux == 1)
            {
                //Avisamos al usuario que no se encontro el numero ingresado y se pide volder a ingresar un valor
                system(clear);
                printf("Balance actual: %.2f\n\nNumero de transferencia no existe, intente nuevamente", cliente->balance);
                wait_for_input();
            }
        } while (aux == 1);
        printf("\nEs correcto el numero de transferencia?\n[1]-Si\n[2]-No\nOpcion: ");
        op = integer_validation("", "");
        if (op == 2)
            system(clear);
    } while (op == 2);
    //Restamos la cantidad transferida por el usuario de su balance
    cliente->balance = cliente->balance - transfer;
    printf("\nSe transferieron %.2f pesos\nSu balance actual es %.2f", transfer, cliente->balance);
    //Reiniciamos el puntero
    cliente = cliente - actual;
    for (i = 0; i < n; i++)
    {
        //Buscamos el numero de transferencia de todos los usuarios hasta que coincida con el no. de transferencia ingresado
        if (no_trans == cliente->no_transfer)
        {
            cliente->balance = cliente->balance + transfer;
        }
        //Aumentamos la direccion el puntero
        cliente++;
    }
    wait_for_input();
    return 1;
}

int integer_validation(char msg[], char alt_msg[])
{
    char numero[20];
    printf("%s", msg);
    fflush(stdin);
    fgets(numero, 20, stdin);
    while (!is_int(numero))
    {
        printf("Ingresar solamente un numero entero: ");
        fflush(stdin);
        fgets(numero, 20, stdin);
    }
    return atoi(numero);
}

float float_validation(char msg[], char alt_msg[])
{
    char numero[20];
    printf("%s", msg);
    fflush(stdin);
    fgets(numero, 20, stdin);
    while (!is_float(numero))
    {
        printf("Ingresar solamente un numero decimal: ");
        fflush(stdin);
        fgets(numero, 20, stdin);
    }
    return atof(numero);
}

int is_int(char *arr)
{
    while (*arr != '\0' && *arr != '\n')
    {
        if (*arr < 48 || *arr > 57)
        {
            return 0;
        }
        arr++;
    }
    return 1;
}

int is_float(char *arr)
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
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        arr++;
    }
    return 1;
}
void wait_for_input(void)
{
    char c;
    printf("\n\nPresione enter para continuar: ");
    getchar();
    fflush(stdin);
    scanf("%c", &c);
}
