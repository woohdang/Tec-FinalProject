


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>

struct player{

    char nick[6];
    char name[21];
    char lastName[21];
    int age= 0;
    int wins= 0;

};

struct actual_Players{

    char playerA[6];
    char playerB[6];

};

void board_inicialize(char[10][10]);
void trees_position(char[10][10]);
void show_board(char [10][10]);
void show_turn(int turn);
void get_direction(int direction [2]);
void game_menu(player [10], actual_Players actualPlayer, int &contPlayers);
void player_list(player [10], int contPlayers);
void register_player(player [10], int &contPlayers);
void player_data(player [10], actual_Players actualPlayer, int contPlayers);
void modify_player(void);
void delete_player(void);
void show_player(int cont);
void player_who_plays(player players[10], actual_Players &actualPlayer, int contPlayers);
void add_wins(player players[10], actual_Players actualPlayer, int contPlayers, int AoB);
bool exit_game(void);
bool game_over(void);
int search_player(player players[10], int contPlayers);
int game_start(player [10], actual_Players actualPlayer, int contPlayers);
int  move_player(int direction [2], char board [10][10], int player [3]);
int  random_player(void);
int  shoot(int direction[2], char board [10][10], int player[3]);
char read_letter(void);
char get_shoot_or_move(void);
char lowercase_to_uppercase(char letra);

const int DIR_SOUTH [2]= {1,0};
const int DIR_NORTH [2]= {-1,0};
const int DIR_EAST [2]= {0,1};
const int DIR_WEST [2]= {0,-1};

const int X=0;
const int Y=1;
const int ACTUAL=2;
const char LETTER [2]= {'A','B'};

int GAME_OVER=0;
int EXIT=0;

int main(){                                                                                  /*MAIN*/

    player players[10];
    actual_Players actualPlayer;
    int contPlayers=0;

    do{
        game_menu(players, actualPlayer, contPlayers);
    }while(!exit_game());
    printf("Gracias Por Jugar!\n\n");
    return 0;
}

    int game_start(player players [10], actual_Players actualPlayer, int contPlayers){       /*INICIAR JUEGO*/

        srand(time(NULL));
        char board [10][10];
        char action;
        int actual_player [3] = {0,0,0};
        int actual_direction [2]= {0,0};
        int playerTurn= 0, cont= 0;
        int PLAYERS [2][3] = {{9,0,0},{0,9,1}};
        int AoB= 0;

        player_who_plays(players, actualPlayer, contPlayers);
        printf("\nEl jugador A es: %s\n", actualPlayer.playerA);
        printf("El jugador B es: %s\n\n", actualPlayer.playerB);
        playerTurn = random_player();
        board_inicialize(board);
        trees_position(board);

        do{
            show_board(board);
            show_turn(playerTurn);
            action = get_shoot_or_move();
            int error= 1;

            do{
            get_direction(actual_direction);

                switch(action){
                case 'T':
            		error= shoot(actual_direction,board,PLAYERS[playerTurn]);
                break;
                case 'M':
                    error= move_player(actual_direction, board, PLAYERS[playerTurn]);
                break;
                }

            }while(error);

            playerTurn= abs(playerTurn-1);

        }while(!game_over());

        if(LETTER[PLAYERS[abs(playerTurn-1)][ACTUAL]] == 'A'){
            printf("Juego Terminado, Jugador %s Gana!\n", actualPlayer.playerA);
            add_wins(players, actualPlayer, contPlayers, AoB);
        }else{
            printf("Juego Terminado, Jugador %s Gana!\n", actualPlayer.playerB);
            AoB= 1;
            add_wins(players, actualPlayer, contPlayers, AoB);
        }

        return 0;
    }

    void board_inicialize(char table [10][10]){                                              /*INICIALIZACIÓN DEL TABLERO*/
        int row= 0, column= 0;
        for (row=0; row< 10; row++){
            for (column=0; column< 10; column++){
                table [row][column] = ' ';
            }
        }

        table [9][0] = 'A';
        table [0][9] = 'B';
    }

    void trees_position(char table[10][10]){                                                 /*INICIALIZACIÓN DE LOS ÁRBOLES*/
        int cont=0, coorRand1= rand(), coorRand2= rand();
        while(cont<10){
            coorRand1= rand()%8 + 1;
            coorRand2= rand()%8 + 1;
            if(table[coorRand1][coorRand2] != 'X'){
                table[coorRand1][coorRand2] = 'X';
                cont++;
            }
        }
    }

    void show_board(char table[10][10]){                                                     /*IMPRESIÓN DEL TABLERO*/
        printf("\n-------------------------------\n");
        for (int row=0; row< 10; row++){
            for (int column=0; column< 10; column++){
                printf("| %c", table[row][column]);
            }
        printf("|");
        printf("\n-------------------------------\n");
        }
    }

    int random_player(){                                                                     /*JUGADOR RANDOM*/
        int player=rand()%2;
        return player;
    }

    char get_shoot_or_move(){                                                                /*OPCIÓN DE TIRAR O MOVER*/
        char option;
        do{
            if(option!='\n')printf("(M)over o (T)irar Flecha?");
            option= read_letter();
        }while(option!= 'M' && option!= 'T');
        return option;
    }

    void get_direction(int actual_direction [2]){                                            /*OBTENER DIRECCIÓN*/
        char dir;
        do{
            if(dir!='\n')printf("Dirección: (S)ur, (N)orte, (E)ste u (O)este?");
            dir= read_letter();

        }while(dir!= 'S' && dir!= 'N' && dir!= 'E' && dir!= 'O');


        switch(dir){
        case 'S':
            actual_direction[0]= DIR_SOUTH[0];
            actual_direction[1]= DIR_SOUTH[1];
        break;
        case 'N':
            actual_direction[0]= DIR_NORTH[0];
            actual_direction[1]= DIR_NORTH[1];
        break;
        case 'E':
            actual_direction[0]= DIR_EAST[0];
            actual_direction[1]= DIR_EAST[1];
        break;
        case 'O':
            actual_direction[0]= DIR_WEST[0];
            actual_direction[1]= DIR_WEST[1];
        break;
        }

    }

    int  move_player(int direction [2], char board [10][10], int player [3]){                /*MOVER JUGADOR*/

        if(!(player[X]+direction[X] >= 0 &&
            player[Y]+direction[Y] >= 0 &&
            player[X]+direction[X] <= 9 &&
            player[Y]+direction[Y] <= 9 &&
            board[player[X]+direction[X]][player[Y]+direction[Y]] ==' ')){
            printf("Dirección Equivocada, Intente Nuevamente: \n");
            return 1;
        }

        board[player[X]][player[Y]]=' ';
        player[X]=player[X]+direction[X];
        player[Y]=player[Y]+direction[Y];
        board[player[X]][player[Y]]=LETTER[player[ACTUAL]];

        return 0;

    }

    bool game_over(){                                                                        /*TERMINAR JUEGO*/

        return GAME_OVER;
    }

    void show_turn(int turn){                                                                /*MOSTRAR TURNO*/

        printf("Juega(%c):\n",LETTER[turn]);
    }

    int  shoot(int direction[2], char board [10][10], int player[3]){                        /*DISPARO*/

        int pos [2]={player[0],player[1]};
        int scan=1, contPlaces=0;
        char enemy = ' ';

        if(LETTER[player[ACTUAL]]=='A'){enemy='B';}else{ enemy='A';}

        while(scan){
            pos[X]=pos[X]+direction[X];
            pos[Y]=pos[Y]+direction[Y];

            if(contPlaces<5){
                if(pos[X]<0 || pos[Y]>9){
                    printf("Fallaste el Tiro \n");
                    scan=0;
                }

                if(board[pos[X]][pos[Y]]=='X'){
                    printf("Le Pegaste a un Árbol \n");
                    scan=0;
                }

                if(board[pos[X]][pos[Y]]==enemy){
                    scan=0; GAME_OVER=1;
                }
            }else{
                printf("Fallaste el Tiro \n");
                scan=0;
            }
        contPlaces++;
        }

        return 0;
    }

    char lowercase_to_uppercase(char letra){                                                 /*INGRESAR MAYÚSCULAS O MINÚSCULAS*/

        if(letra >= 'a' && letra <='z'){
                letra = letra - 'a' + 'A';
        }
        if(letra >= 'A' && letra <='Z'){
                return letra;
        }

        return '8';      
    }

/////////////////////////////////////////////////////////////////////////                    /*COMIENZA CÓDIGO DE SEGUNDA ENTREGA*/

    void game_menu(player players[10], actual_Players actualPlayer, int &contPlayers){       /*MENÚ DEL JUEGO*/

        int cont=0, breakWhile=0;
        char selection;
        printf("\n\nMENU PRINCIPAL\nSeleccione una opcion para continuar:\n\n");
        printf("(L)istado de jugadores \n(R)egistrar jugador \n(J)ugar \n(D)atos de jugador \n(S)alir\n\nOpcion:");
        do{
            if(cont>0){printf("Opcion Incorrecta, Intente de nuevo\n\n");}
            selection= read_letter();
            cont++;

            switch(selection){
            case 'L':
                printf("Listado de jugadores:\n\n");
                player_list(players, contPlayers);
                breakWhile++;
            break;
            case 'R':
                printf("Registrar jugador:\n\n");
                if(contPlayers<=9){
                    register_player(players, contPlayers);
                    contPlayers++;
                }else{printf("Demasiados Jugadores Ingresados, No Se Pueden Registrar Más\n");}
                breakWhile++;
            break;
            case 'J':
                printf("Jugar:\n\n");
                game_start(players, actualPlayer, contPlayers);
                breakWhile++;
            break;
            case 'D':
                printf("Datos del jugador:\n\n");
                player_data(players, actualPlayer, contPlayers);
                breakWhile++;
            break;
            case 'S':
                printf("Salir:\n\n");
                EXIT=1;
                breakWhile++;
            break;
            default:
                printf("Ingrese Aunque Sea Una Letra\n");
            break;
            }
        }while(!breakWhile);

    }

    void register_player(player players[10], int &contPlayers){                              /*REGISTRAR JUGADOR*/

        int contName=0, contNick=0, contLast=0, contAge=0, cont=0, same=1;
        do{
            if(contNick>0){
                printf("Alias Inválido, Recuerde Que Debe Ingresar Al Menos 1 Caracter y Máximo 5");
            }
            printf("\nIngrese su Alias: ");
            gets(players[contPlayers].nick);
            for(cont=0; cont<contPlayers; cont++){
                same= strcmp(players[contPlayers].nick, players[cont].nick);
                if(same == 0){
                    printf("El Usuario Ya Existe, Ingrese Un Alias Distinto\n");
                    contNick= -1;
                }
            }
            contNick++;
        }while((!(strlen(players[contPlayers].nick) < 6)) || same == 0);

        do{
            if(contName>0){
                printf("Nombre Inválido, Recuerde Que Debe Ingresar Al Menos 1 Caracter y Máximo 20");
            }
            printf("\nIngrese su Nombre: ");
            gets(players[contPlayers].name);
            contName++;
        }while(!(strlen(players[contPlayers].name) < 21));

        do{
            if(contLast>0){
                printf("Apellido Inválido, Recuerde Que Debe Ingresar Al Menos 1 Caracter y Máximo 20");
            }
            printf("\nIngrese su Apellido: ");
            gets(players[contPlayers].lastName);
            contLast++;
        }while(!(strlen(players[contPlayers].lastName) < 21));

        do{
            if(contAge>0){
                printf("Edad Inválida, Ingrese Nuevamente\n");
            }
            printf("\nIngrese su Edad: ");
            scanf("%d", &players[contPlayers].age);
            contAge++;
        }while(!(0 < players[contPlayers].age));
        printf("\n\n");
        getchar();
    }

    int search_player(player players[10], int contPlayers){                                  /*BUSCAR JUGADOR*/

        int cont=0, same=1;
        char nickname[5];
        printf("Ingrese Alias del Jugador: ");
        gets(nickname);
        for(cont=0; cont<contPlayers; cont++){
            same= strcmp(nickname, players[cont].nick);
            if(same == 0){
                return cont;
            }
        }
        printf("El Jugador No Está Registrado\n");
        return -1;
    }

    void player_data(player players[10], actual_Players actualPlayer, int contPlayers){      /*DATOS DE JUGADOR*/

        int cont=0;
        cont= search_player(players, contPlayers);
        printf("\nNombre: %s \n", players[cont].name);
        printf("\nApellido: %s \n", players[cont].lastName);
        printf("\nEdad: %i \n", players[cont].age);
        printf("\nPartidas Ganadas: %i \n", players[cont].wins);

    }

    void player_list(player players[10], int contPlayers){                                   /*LISTA DE JUGADORES*/

        int cont=0;
        for(cont=0; cont<contPlayers; cont++){
            printf("Alias: %s   ", players[cont].nick);
            printf("Partidas Ganadas: %i \n", players[cont].wins);
        }
    }

    bool exit_game(){                                                                        /*SALIR DEL JUEGO*/

        return EXIT;
    }

    char read_letter(){                                                                      /*LEE LETRA*/

        char letter;
        scanf("%c", &letter);
        letter= lowercase_to_uppercase(letter);
        getchar();
        return  letter;
    }

    void player_who_plays(player players[10], actual_Players &actualPlayer, int contPlayers){/*JUGADORES QUE VAN A JUGAR*/

        int cont=0;
        do{
            cont= search_player(players, contPlayers);
            printf("'%d'", cont);
            if(cont != -1){
                strcpy(actualPlayer.playerA, players[cont].nick);
            }
        }while(cont == -1);

        do{
            cont= search_player(players, contPlayers);
            if(cont != -1){
                strcpy(actualPlayer.playerB, players[cont].nick);
            }
        }while(cont == -1);
    }

    void add_wins(player players[10], actual_Players actualPlayer, int contPlayers, int AoB){/*AGREGA PARTIDAS GANADAS*/

        int cont=0, same=1;
        for(cont=0; cont<contPlayers; cont++){
            if(AoB== 0){
                same= strcmp(actualPlayer.playerA, players[cont].nick);
            }else{
                same= strcmp(actualPlayer.playerB, players[cont].nick);
            }
            if(same == 0){
                players[cont].wins++;





































