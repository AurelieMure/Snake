#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

#define SIZE_MAXI 200
#define LARGEUR 60 
#define HAUTEUR 30
#define TEMPS 200000


struct fruit{
    int fruitX;
    int fruitY;
};

struct fruitSpecial{
    int fruitspeX;
    int fruitspeY;
};

enum direction { STOP = 0, GAUCHE, DROITE, HAUT, BAS };

struct jeu{
    bool jeuEnCours;
    int teteX;
    int teteY;
    int score;
    int tailleQueue;
    direction d;
    fruit f;
    fruitSpecial fs;
    int mode;
    int random;
    bool specialFruitActive;
    int compteur;
};

jeu initialiser(jeu j, int queueX[], int queueY[]);
int dessiner(jeu j, int tabX[], int tabY[]);
direction entrerDirection(direction d);
jeu deplacer(jeu j, int queueX[], int queueY[],int temps);
fruit generfruit(jeu j, int queueX[], int queueY[]);
fruitSpecial generfruitSpe(jeu j, int queueX[], int queueY[]);
int userInput();
void afficherSnake();


int main() {
    int temps,TEMPS_MAX,ancienScore,TEMPS_MAX2;
    TEMPS_MAX = 100000;
    TEMPS_MAX2= 80000;
    temps = TEMPS;
    jeu j;
    int queueX[SIZE_MAXI], queueY[SIZE_MAXI];

    srand(time(NULL));
    afficherSnake();
    cout << "\n" <<"Entrez le mode que vous voulez:"<<endl; 
    cout << "1 = Facile" << endl;
    cout << "2 = Difficile "<< endl;
    cin >> j.mode;
    
    j = initialiser(j,queueX,queueY);

    while (j.jeuEnCours){
        dessiner(j,queueX,queueY);
        if(userInput()){
            j.d = entrerDirection(j.d);
        }
        j = deplacer(j,queueX,queueY,temps);
    
        // si utilisateur lance mode difficile
        if(j.mode == 2){
            // Si le score est pair alors le temps diminue
            if(j.score % 2 == 0 && j.score > 0 && j.score != ancienScore){          
                if (temps > TEMPS_MAX2){
                    temps -= 10000;  
                }
            }
            ancienScore = j.score;
            
            usleep(temps);
        }

        // Si l'utilisateur lance la mode facile ou autre touche 
        else{
            // Si le score est pair alors le temps diminue
            if(j.score%2 == 0 && j.score>0 && j.score != ancienScore){
                if(temps > TEMPS_MAX ){
                    temps -= 50;  
                }
            }
            ancienScore =j.score;
            usleep(temps);
        }
    }

    cout << "Game Over! Vous avez: " << j.score <<"$"<< endl;
    return 0;
}


jeu initialiser(jeu j, int queueX[], int queueY[]){
    /**
     Initialisation les variables
     */
    j.jeuEnCours = true;
    j.teteX = LARGEUR/2;
    j.teteY = HAUTEUR/2;
    j.score = 0;
    j.tailleQueue = 0;
    j.d = STOP;
    j.random = rand() %8 +1;
    j.specialFruitActive = false;
    j.compteur=100;


    // Regénère le fruit aléatoirement dans le cadre
    j.f = generfruit(j,queueX,queueY);
              
    return j;
}

int dessiner(jeu j, int tabX[], int tabY[]){
    std::system("clear"); // Efface l'écran
    /**
     Permet de dessiner le jeu
     */
    int i;
    int k;
    int y,x;
    char c = ' ';

    // dessine le mur du haut
    for(i=0 ; i<LARGEUR; i++){
        cout <<"#";
    }
    cout <<endl;
    for(k=0 ; k<HAUTEUR ; k++){
        for(x=0 ; x<LARGEUR ; x++){ 
            c = ' ';
            // dessine les deux murs du milleu dans le mode difficile
            if(j.mode==2){
                if((x==LARGEUR/3 || x == LARGEUR*2/3)&& k>HAUTEUR/3 && k<HAUTEUR*2/3 ){
                c = '#';
                }
            }
            //dessine le mur
            if(x==0 || x == LARGEUR-1){
                c = '#';
            } 
            //dessiner le fruit si bonne coordonée 
            else if(x==j.f.fruitX && k==j.f.fruitY){
                c = '$';
            }   
            //dessine le fruit spécial         
            else if (j.random == 2 && x == j.fs.fruitspeX && k == j.fs.fruitspeY){
                c = '@';
            }
            // dessine la tete du serpent
            else if(x==j.teteX && k==j.teteY){
                c = 'O';
            }
            // dessine le corp du serpent
            else{
                for(i=0; i<j.tailleQueue;i++){
                    if(tabX[i]==x && tabY[i]==k){
                        c ='o';
                        break;
                    }
                }
            }std::cout<<c;  
        }   
        cout<< endl;
    }
    //dessine le mur du bas
    for(y=0; y<LARGEUR; y++){
        cout <<"#";
    }

    // affiche le score
    cout <<endl;
    cout <<"Score : " << j.score;
    cout <<endl;

    return 0;
}

direction entrerDirection(direction d){
    system("stty raw");
     /**
    Récupérer la direction saisie
    */
    char c;
    
    c = getchar();
    if((c=='z' || c=='Z') &&(d!=BAS)){
        d = HAUT;
    }
    if((c=='q' || c=='Q') && (d!=DROITE)){
        d = GAUCHE;
    }
    if((c=='d' || c=='D') && (d!=GAUCHE)){
        d = DROITE;
    }
    if((c=='s' || c=='S') && (d!=HAUT)){
        d = BAS;
    }
   
    system("stty sane");
    return d;

}

jeu deplacer(jeu j, int queueX[], int queueY[], int temps){
    int ancienTeteX, ancienTeteY;
    int i;
    int dure;
    dure = 100000;

    // Sauvegarde de la position actuelle de la tête
    ancienTeteX = j.teteX;
    ancienTeteY = j.teteY;

    // Déplacer le serpent selon la direction
    if (j.d == HAUT){
        j.teteY--;
    } 
    else if (j.d == BAS){
        j.teteY++;
    } 
    else if (j.d == GAUCHE){
        j.teteX--;
    } 
    else if (j.d == DROITE){
        j.teteX++;
    }

    // compteur qui baisse
    j.compteur--;

    // Condition d'arrêt si le serpent touche un bord
    if (j.teteX <= 0 || j.teteX >= LARGEUR-1 || j.teteY < 0 || j.teteY >= HAUTEUR){
        j.jeuEnCours = false;
    }

    // Condition d'arrêt si le serpent se touche
    for(i=0; i<j.tailleQueue; i++){
        if(j.teteX == queueX[i] && j.teteY == queueY[i])
            j.jeuEnCours = false;
    }

    // Condition d'arret si le serpent touche un des deux murs du milleu
    if(j.mode == 2){
        if ((j.teteX == LARGEUR/3 && j.teteY >= (HAUTEUR/3)+1 && j.teteY <= (HAUTEUR*2/3)-1) || (j.teteX == LARGEUR*2/3 && j.teteY >= (HAUTEUR/3)+1 && j.teteY <= (HAUTEUR*2/3)-1)){
        j.jeuEnCours = false;
        }
    }

    // Si la tête touche le fruit
    if (j.teteX == j.f.fruitX && j.teteY == j.f.fruitY){
        j.score++;
        j.tailleQueue++;        
        j.f=generfruit(j,queueX ,queueY);

    }

    //compteur jusqu'a 100 pour relancé le random
    if(j.compteur==0){
        j.random = rand() %8 +1;
        j.compteur=100;
    }
    
    //Si random = 2 genere le fruit spécial
    if(j.random==2 && j.specialFruitActive== false ){
        j.specialFruitActive = true;
        j.fs = generfruitSpe(j, queueX, queueY);
    }

    // Si le serpent touche fruit spécial
    if (j.teteX == j.fs.fruitspeX && j.teteY == j.fs.fruitspeY){
            j.score=j.score +5;
            j.tailleQueue++;
            j.specialFruitActive = false;  
            j.random = rand() %8 +1;  
    }

    // decalage de la queue à gauche
    for(i = j.tailleQueue-1; i>0; i--){
        queueX[i] = queueX[i-1];
        queueY[i] = queueY[i-1];
    }

    // le debut de la queue devient l'ancienne position de la tete
    queueX[0] = ancienTeteX;
    queueY[0] = ancienTeteY;

    return j;
}


fruit generfruit(jeu j, int queueX[], int queueY[]){
    int i;
    bool b;
    fruit fr;
    
    do {
        b = false;
        // Regénère le fruit aléatoirement dans le cadre
        fr.fruitX = (rand() % (LARGEUR - 2)) + 1;
        fr.fruitY = (rand() % (HAUTEUR - 2)) + 1;

        // parcours liste de la queue 
        for (i = 0; i < j.tailleQueue; i++) {
            // verifie si la fruit est sur la queue
            if (queueX[i] == fr.fruitX && queueY[i] == fr.fruitY){
                b = true;
            }
            //verifie si le fruit est sur l'un des deux murs du mode difficile
            if(j.mode==2){
                if((fr.fruitX == LARGEUR/3 && fr.fruitY >= (HAUTEUR/3)+1 && fr.fruitY <= (HAUTEUR*2/3)-1) || (fr.fruitX == LARGEUR*2/3 && fr.fruitY >= (HAUTEUR/3)+1 && j.teteY <= (HAUTEUR*2/3)-1)){
                b = true;
                }
            }
        }
    }while (j.teteX == fr.fruitX && j.teteY == fr.fruitY || b == true);
    return fr;
}

fruitSpecial generfruitSpe(jeu j, int queueX[], int queueY[]){
    int i;
    bool b;
    fruitSpecial frSpe;
    
    do {
        b = false;
        // Regénère le fruit aléatoirement dans le cadre
        frSpe.fruitspeX = (rand() % (LARGEUR - 2)) + 1;
        frSpe.fruitspeY = (rand() % (HAUTEUR - 2)) + 1;

        // parcours liste de la queue 
        for (i = 0; i < j.tailleQueue; i++){
            // verifie si la fruit est sur la queue
            if (queueX[i] == frSpe.fruitspeX && queueY[i] == frSpe.fruitspeY){
                b = true;
            }
            //verifie si le fruit est sur l'un des deux murs du mode difficile
            if (j.mode==2){
                if ((frSpe.fruitspeX == LARGEUR/3 && frSpe.fruitspeY >= (HAUTEUR/3)+1 && frSpe.fruitspeY <= (HAUTEUR*2/3)-1) || (frSpe.fruitspeX == LARGEUR*2/3 && frSpe.fruitspeY >= (HAUTEUR/3)+1 && j.teteY <= (HAUTEUR*2/3)-1)){
                b = true;
                }
            }
        }
    }while (j.teteX == frSpe.fruitspeX && j.teteY == frSpe.fruitspeY || b == true);
    return frSpe;
}


//http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
int userInput(){
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
    ungetc(ch, stdin);
    return 1;
    }

    return 0;
}

void afficherSnake(){
    // affcihe snake pour le menu
    printf("  ####   #      #   ######   #   #  #####   \n");
    printf(" #       # #    #   #    #   #  #   #     \n");
    printf("  ###    #  #   #   ######   ##     ###  \n");
    printf("     #   #    # #   #    #   #  #   #  \n");
    printf(" ####    #      #   #    #   #   #  #####  \n");
}
