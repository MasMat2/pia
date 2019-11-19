#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

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
int crear_cuenta(FILE *fp, struct cliente clientes[], int n);
int find_acc(int no_cuenta, struct cliente *cl, int n);
int update(FILE *fp, struct cliente clientes[], int n);
int menu(struct cliente *cliente);
int informacion(struct cliente *cliente);
int retiros(struct cliente *clientes);
float depositos(struct cliente *cliente);
int transferencias(struct cliente *cliente, int n, int actual);
void imprimir_movimientos(struct cliente *cliente);

int crear_logearse();

int integer_validation(char msg[], char alt_msg[]);
float float_validation(char msg[], char alt_msg[]);
int is_int(char *arr);
int is_float(char *arr);

int historial(struct cliente *cliente, float mov, int opc);

void wait_for_input(void);

int main()
{
    struct cliente clientes[100];
    int i, n;
    int op = 5, on;
    int actual=0;
    //Abir archivo y llenar arreglo de estructuras
    FILE *fp;
    fp = fopen("db.txt", "r+");
    if (fp == NULL)
    {
        printf("Error al abrir el archivo");
        exit(-1);
    }

    fscanf(fp, "%d\n", &n);
    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%d %d %s %s %d %f %d\n", &clientes[i].no_list, &clientes[i].no_cliente, clientes[i].nombre, clientes[i].apellido, &clientes[i].pswd, &clientes[i].balance, &clientes[i].no_transfer);
    }
    //Llenado de la estructura finalizado

    do
    {
        do{
            op = crear_logearse();
            switch (op)
            {
            case 1:
                actual = login(clientes, n);
                break;
            case 2:
                n = crear_cuenta(fp, clientes, n);
                actual = n-1;
                break;
            }
        }while(actual==-1);
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
                transferencias(&clientes[actual], n, actual);
                break;
            case 5:
                imprimir_movimientos(&clientes[actual]);
                break;
            }
        } while (op != 6);
        update(fp, clientes, n);
        on = integer_validation("Desea apagar el cajero automatico (1-Si/2-No): ", "");
    } while (on==2);

    fclose(fp);
    return 0;
}


int login(struct cliente clientes[], int n)
{
    system(clear);
    printf("********Cajero Automatico ACME*******\n");
    int no_cuenta, index;
    int pswd, try_acc = 0, try_pswd = 0;

    //Obtener numero de cuenta y validar:
    do
    {
        no_cuenta = integer_validation("\nIngrese su numero de cuenta, (0 para salir): ", "");
        if(no_cuenta==0)
            return -1;
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
        pswd = integer_validation("\nIngrese su contrasena: ", "");
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

int crear_logearse()
{
    int op;
    do
    {
        system(clear);
        printf("********Cajero Automatico ACME*******\n");
        printf("\nEliga la opcion que desee: ");
        printf("\n1- Logearse");
        printf("\n2- Crear cuenta");
        op = integer_validation("\nOpcion: ", "");
    } while (op < 0 || op > 2);
    return op;
}
int crear_no_cuenta(struct cliente clientes[], int n)
{
    int i, no_cliente;
    int flag = 1;

    while (flag)
    {
        flag = 0;
        no_cliente = 1000 + (rand() % 1000);
        for (i = 0; i < n; i++)
        {
            if (clientes[i].no_cliente == no_cliente)
            {
                flag = 1;
            }
        }
    }
    return no_cliente;
}
int crear_no_transferencia(struct cliente clientes[], int n)
{
    int i, no_transfer;
    int flag = 1;

    while (flag)
    {
        flag = 0;
        no_transfer = 1000 + (rand() % 1000);
        for (i = 0; i < n; i++)
        {
            if (clientes[i].no_transfer == no_transfer)
            {
                flag = 1;
            }
        }
    }
    return no_transfer;
}
int crear_cuenta(FILE *fp, struct cliente clientes[], int n)
{
    // int i;
    char *aux;
    system(clear);
    printf("********Cajero Automatico ACME*******\n");
    printf("\nIngrese su nombre: ");
    fflush(stdin);
    fgets(clientes[n].nombre, 20, stdin);
    if ((aux = strchr(clientes[n].nombre, '\n')) != NULL)
        *aux = '\0';
    printf("Ingrese su apellido: ");
    fflush(stdin);
    fgets(clientes[n].apellido, 20, stdin);
    if ((aux = strchr(clientes[n].apellido, '\n')) != NULL)
        *aux = '\0';
    clientes[n].no_cliente = crear_no_cuenta(clientes, n);
    printf("Ingrese su nueva contrasena (numero de 4 digitos): ");
    clientes[n].pswd = integer_validation("", "");
    while(clientes[n].pswd<100 ||clientes[n].pswd>9999){
        printf("Por favor solo una contrasena de 4 digitos): ");
        clientes[n].pswd = integer_validation("", "");
    }
    clientes[n].balance = 0;
    clientes[n].no_transfer = crear_no_transferencia(clientes, n);
    clientes[n].no_list = n + 1;
    printf("\nSu numero de cuenta es %d es importante que lo recuerde para que pueda logearse.", clientes[n].no_cliente);
    historial(&clientes[n], 0, 0);
    wait_for_input();
    return n+1;
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
    // rewind(fp);
    fseek(fp,0,0);
    //Escribir el numero de clientes que se guardaran
    fprintf(fp, "%d\n", n);
    //Escribir cada la estuctura de cada uno de los clientes
    for (i = 0; i < n; i++)
    {
        fprintf(fp, "%d %d %s %s %d %f %d\n", clientes[i].no_list, clientes[i].no_cliente, clientes[i].nombre, clientes[i].apellido, clientes[i].pswd, clientes[i].balance, clientes[i].no_transfer);
    }
    return 0;
}

int historial(struct cliente *cliente, float mov, int opc)
{
    char nume[' '];
    float sum;
    sprintf(nume, "movimientos/%d.txt", cliente->no_cliente);
    FILE *archivo = fopen(nume, "a");
    if(mov==0){
        return 0;
    }
    fprintf(archivo, "Balance Inicial: %f", cliente->balance);
    switch (opc)
    {
    case 2:
        sum = cliente->balance - mov;
        fprintf(archivo, "\tRetiro: %f", mov);
        break;
    case 3:
        sum = cliente->balance + mov;
        fprintf(archivo, "\tDeposito: %f", mov);
        break;
    case 4:
        sum = cliente->balance - mov;
        fprintf(archivo, "\tTransferencia: %f", mov);
        break;
    case 5:
        sum = cliente->balance + mov;
        fprintf(archivo, "\tTransferencia: %f", mov);
        break;
    }
    fprintf(archivo, "\tBalance actual: %f\n", sum);
    SYSTEMTIME t;
    GetLocalTime(&t);
    fprintf(archivo, "\tFecha: %d/%d/%d a las %d:%d\n", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute);
    fclose(archivo);
    return 1;
}




int menu(struct cliente *cliente)
{
    int op;
    do
    {
        system(clear);
        printf("********Cajero Automatico ACME*******\n");
        printf("\nBienvenido %s %s\n", cliente->nombre, cliente->apellido);
        printf("\nEliga la opcion que desee: ");
        printf("\n\n1- Obtener informacion de la cuenta");
        printf("\n2- Realizar un retiro");
        printf("\n3- Realizar un deposito");
        printf("\n4- Realizar una transferenecia");
        printf("\n5- Imprimir movimientos recientes");
        printf("\n6-Salir");
        op = integer_validation("\n\nOpcion: ", "");
    } while (op < 1 || op > 6);
    return op;
}

int informacion(struct cliente *cliente)
{
    system(clear);
    printf("********Cajero Automatico ACME*******\n");
    printf("\nInformacion de la cuenta:");
    printf("\n\n%-24s %s %s", "Nombre:", cliente->nombre, cliente->apellido);
    printf("\n%-24s %d", "Numero de cuenta:", cliente->no_cliente);
    printf("\n%-24s %d", "Numero de transferencia:", cliente->no_transfer);
    printf("\n%-24s %.2f", "Balance:", cliente->balance);
    wait_for_input();
    return 1;
}
int retiros(struct cliente *cliente)
{
    float retiro;
    int op, aux=0;
    do
    {
        do
        {
            system(clear);
            printf("********Cajero Automatico ACME*******\n");
            if(aux)
                printf("\nLa cantidad a retirar es mayor a su balance, ingrese otra cantidad\n\n");
            printf("\nBalance actual: %.2f\n", cliente->balance);
            retiro = float_validation("\n\nIngreasa la cantidad que desea retirar (0 para salir): ", "");
            if(retiro==0){
                return 0;
            }
            //Validamos que la cantidad ingresada no sea mayor a el balance del usuario
            if (retiro > cliente->balance)
                aux = 1;
            else
                aux = 0;
        } while (aux);
        printf("\nEs correcta la cantidad de %.2f?\n1-Si\n2-No\nOpcion: ", retiro);
        op = integer_validation("", "");
    } while (op==2);
    //Actualizamos el balance del usuario actual
    historial(cliente, retiro, 2);
    cliente->balance = cliente->balance - retiro;
    printf("\nSu balance actual es %.2f", cliente->balance);
    wait_for_input();
    return 1;
}
float depositos(struct cliente *cliente)
{
    float deposito;
    system(clear);
    printf("********Cajero Automatico ACME*******\n");
    deposito = float_validation("\nIngrese la cantidad que quiere depositar (0 para salir): ", "");
    if(deposito==0){
        return 0;
    }
    historial(cliente, deposito, 3);
    cliente->balance += deposito;
    printf("Se depositaron %.2f pesos en su cuenta\n\nSu balance actual es %.2f", deposito, cliente->balance);
    wait_for_input();
    return deposito;
}

int transferencias(struct cliente *cliente, int n, int actual)
{
    float transfer;
    int no_trans, i, aux, op = 0;
    struct cliente *ap;
    system(clear);
    printf("********Cajero Automatico ACME*******\n");
    //Validamos que la cantidad ingresada no sea mayor a el balance del usuario
    do
    {
        aux = 0;
        printf("\nBalance actual: %.2f\n\nIngreasa la cantidad que desea transferir (0 para salir): ", cliente->balance);
        transfer = float_validation("", "");
        if(transfer==0){
            return 0;
        }
        if (transfer > cliente->balance)
        {
            system(clear);
            printf("********Cajero Automatico ACME*******\n");
            printf("\nLa cantidad a transferir esta por encima de su balance, intente otra vez: \n");
            aux = 1;
        }
    } while (aux == 1);
    do
    {
        //Validamos que el numero de transferencia exista antes de realizar una operacion
        do
        {
            printf("\nIngrese el numero de transferencia del destinatario (0 para salir): ");
            if(transfer==0){
                return 0;
            }
            no_trans = integer_validation("", "");
            if(no_trans==0)
                return 1;
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
                    printf("\nEl numero de transferencia es de %s %s", ap->nombre, ap->apellido);
                    aux = 0;
                }
                ap++;
            }
            if (aux == 1)
            {
                //Avisamos al usuario que no se encontro el numero ingresado y se pide volder a ingresar un valor
                system(clear);
                printf("********Cajero Automatico ACME*******\n");
                printf("\nBalance actual: %.2f\n\nNumero de transferencia no existe, intente nuevamente presione 0 para salir\n", cliente->balance);
            }
        } while (aux == 1);
        printf("\nEs correcto el numero de transferencia?\n1-Si\n2-No\nOpcion: ");
        op = integer_validation("", "");
    } while (op == 2);
    //Restamos la cantidad transferida por el usuario de su balance
    historial(cliente, transfer, 4);
    cliente->balance = cliente->balance - transfer;
    printf("\nSe transferieron %.2f pesos\nSu balance actual es %.2f", transfer, cliente->balance);
    //Reiniciamos el puntero
    cliente = cliente - actual;
    for (i = 0; i < n; i++)
    {
        //Buscamos el numero de transferencia de todos los usuarios hasta que coincida con el no. de transferencia ingresado
        if (no_trans == cliente->no_transfer)
        {
            historial(cliente, transfer, 5);
            cliente->balance = cliente->balance + transfer;
        }
        //Aumentamos la direccion el puntero
        cliente++;
    }
    wait_for_input();
    return 1;
}

void imprimir_movimientos(struct cliente *cliente){
    FILE *cf;
    char archivo_linea[100];
    char archivo[100];
    sprintf(archivo, "movimientos/%d.txt", cliente->no_cliente);
    cf = fopen(archivo, "r");

    system(clear);
    printf("********Cajero Automatico ACME*******\n");
    printf("\nMovimientos recientes: \n\n");
    while(fgets(archivo_linea, 100, cf)){
        printf("%s", archivo_linea);
    }
    wait_for_input();
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
