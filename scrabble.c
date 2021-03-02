#include "./lib/libgraphique.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define LFENETRE 1155
#define HFENETRE 958
#define LJEUCASE 42
#define HJEUCASE 44
#define LGRILLECASE 37
#define HGRILLECASE 41
#define ORIGINEGRILLEX 82
#define ORIGINEGRILLEY 37
#define NBCASES 16
#define FILEPOT "./Ressources/pot" // chemin du fichier pot
#define SIZEPOT 105     // taille large du pot
#define NBJOUEURSMAX 5     // taille du pot max
#define NBREJOUEURS 4
#define FILEDICO "./Ressources/dictionnaire_fr_ss_accents" // chemin du fichier dico
#define SIZEDICO 318895                        // taille du dico
#define MAXLENMOT 26                            // taille du mot max

typedef char *Plateau;
typedef char *Main;
typedef char *Pot;

typedef struct
	{
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	}Bouton;
	
typedef struct
	{
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	}Zone;
	
typedef struct
	{
	Point j1;
	Point j2;
	Point j3;
	Point j4;
	}Points;

Main tableauSauvegardeMainsJoueur [5][8] ;
Plateau plateauSauvegarde[NBCASES][NBCASES] = {NULL};

int chargePot(char *filepot, char tabpot[SIZEPOT][NBJOUEURSMAX]);
void affichageMainJoueur(int numeroJoueur, Main mainjoueurs[5][8]);
int dejaTire(int nbTire , int nbDejaTires[100]);
int tirageJetons(int nbDejaTires [100] , Main mainjoueurs[5][8] , char tabpot[SIZEPOT][NBJOUEURSMAX] , int nbjoueurs , int *nbLettresRestantes );
void afficherDeplacementJeton(Point clic , int testClic , int numeroJoueur , Main mainjoueurs[5][8] , Plateau plateau [NBCASES][NBCASES]);
int estVoisin (Point clic , int clicPrecedentX , int clicPrecedentY , char direction[5] , int numeroJoueur , Plateau plateau [NBCASES][NBCASES], Point premierJetonPoseDuTourParJoueur [5]);
int jeu (Main mainjoueurs[5][8], Plateau plateau [NBCASES][NBCASES], char tabpot[SIZEPOT][NBJOUEURSMAX], int nbLettresDistribuees, int nbLettresDepart, int nbLettresRestantes [1],  int nbjoueurs, int nbDejaTires [100], char tabdico[SIZEDICO][MAXLENMOT]);
Point tourJeu (Point clicPrecedent , int nbjoueurs , Main mainjoueurs[5][8] , char tabpot[SIZEPOT][NBJOUEURSMAX] , Zone zonePlateau , Point premier_clic , int numeroJoueur , int numeroJeton , Plateau plateau [NBCASES][NBCASES] , char direction [5], Point premierJetonPoseDuTourParJoueur [5]);
int menu(void);
Point origineJetonPourRepaint(Point premier_clic , int numeroJoueur);
void remiseAZeroDirection(int nbjoueurs , char direction[5]);
int quelJeton (int numeroJoueur , Point premier_clic);
void afficherPlateau(void);
int menu_joueur(void);
int testWordValue(char *plateau[NBCASES][NBCASES], Point premierJetonPoseDuTourParJoueur[5], int numeroJoueur, char direction[5]);
int pointInBouton(Point p, Bouton b);
int pointNotInZone(Point p, Zone z);
int pointInZone(Point p, Zone z);
void copieTableauMainJoueurs(Main tableauSauvegardeMainsJoueur [5][8] , Main mainjoueurs[5][8]);
void copieTableauPlateau(Plateau plateauSauvegarde[NBCASES][NBCASES] , Plateau plateau[NBCASES][NBCASES]);
void restaurationMainJoueur(Main mainjoueurs[5][8] , Main tableauSauvegardeMainsJoueur [5][8] , int numeroJoueur);
void restaurationPlateau(Plateau plateau[NBCASES][NBCASES] , Plateau plateauSauvegarde[NBCASES][NBCASES] );
void afficher_infos(int nbjoueurs, int score[4], int nbLettresRestantes [1]);
int motEstDansLeDictionnaire (Point premierJetonPoseDuTourParJoueur [5] , Plateau plateau [NBCASES][NBCASES] , int numeroJoueur , char tabdico[SIZEDICO][MAXLENMOT] , char direction [5]);
int chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT]);
void refreshJeu (char *plateau[NBCASES][NBCASES], char *mainjoueurs[5][8], int nbjoueurs);
int nbJetons(char *mainjoueurs[5][8], int numeroJoueur);
void afficherLettre(char path[40], Point p);
int maxScore(int score[4], int nbjoueurs);


Point origine = {0};

int main(int argc, char *argv[]){
	int nbjoueurs;
	ouvrir_fenetre(700,500);
    if(menu())
    	{
		nbjoueurs = menu_joueur();
        ouvrir_fenetre(LFENETRE,HFENETRE);
        afficherPlateau();
        srand(time(NULL));
		printf("\n");
		
		
		Main mainjoueurs[5][8] = {NULL};
		int nbLettresDepart ;
		int nbLettresDistribuees = 0 ;
		char tabpot[SIZEPOT][NBJOUEURSMAX];
		int nbDejaTires [100] = {-1}; // tableau contenant les nombres déjà tirés
		int nbmots;
		int nbLettresRestantes[2];
		*nbLettresRestantes = 100;
		char tabdico[SIZEDICO][MAXLENMOT]; 
		nbmots = chargeDico(FILEDICO,tabdico);

		nbLettresDepart = chargePot(FILEPOT,tabpot); // charge le pot grâce à un fichier et retourne le nombre de lettres

		if(nbLettresDepart==0)
			{
			fprintf(stderr,"chargePot: qqc s'est mal passé\n"); 
			return 1;
			}
		
		nbLettresDistribuees = tirageJetons(nbDejaTires , mainjoueurs , tabpot , nbjoueurs , nbLettresRestantes); // appel de la fonction tirageJeton pour remplir les mains des joueurs
		copieTableauMainJoueurs(tableauSauvegardeMainsJoueur , mainjoueurs);
		
		for(int numeroJoueur = 1 ; numeroJoueur <= nbjoueurs ; numeroJoueur++)
			affichageMainJoueur(numeroJoueur,mainjoueurs); // appel de la fonction affichageMainJoueur pour afficher les jetons des joueurs
		actualiser();
		
		Plateau plateau [NBCASES][NBCASES] = {NULL};
		
		int gagnant = 0;
		gagnant = jeu (mainjoueurs , plateau , tabpot, nbLettresDistribuees , nbLettresDepart , nbLettresRestantes , nbjoueurs , nbDejaTires , tabdico);
		if(gagnant != 0)
			{
			ouvrir_fenetre(700,500);
			if(gagnant == 1)
				afficher_image("./Ressources/Menu/menu-win-j1.bmp", origine);
			else if(gagnant == 2)
				afficher_image("./Ressources/Menu/menu-win-j2.bmp", origine);
			else if(gagnant == 3)
				afficher_image("./Ressources/Menu/menu-win-j3.bmp", origine);
			else
				afficher_image("./Ressources/Menu/menu-win-j4.bmp", origine);
			actualiser();
			attendre_clic();
			}		
		// fin de la session graphique
		fermer_fenetre();
		return 0;
		}
}

// FONCTION QUI CHARGE LE TABLEAU A PARTIR D'UN FICHIER
int chargePot(char *filepot, char tabpot[SIZEPOT][NBJOUEURSMAX]) 
{
    FILE *f = fopen(filepot,"r"); // ouverture du fichier

    if (!f) // si ouverture ratée
        {
        fprintf(stderr,"fopen: problème d'ouverture du fichier '%s'\n'",filepot); 
        return 0;
        }

    char line[NBJOUEURSMAX]; // la ligne servant à la lecture du fichier

    int mots=0; // indice dans tabpot 
    while(fgets(line,NBJOUEURSMAX,f)) // tant que fgets ne renvoie pas nul (eof)
        {
        sscanf(line,"%s\n",tabpot[mots]); // je scanne line et écris dans tabpot
        mots++;
        }
    fclose(f); // fermeture du fichier
    
    return mots;
}

// FONCTION QUI AFFICHE ET GERE LE TOUT PREMIER MENU
int menu(void)
	{
	Point clic;
	Point curseur;
	Bouton start = {209,491,290,348};
	Bouton exit = {236,463,363,420};
	Zone boutons = {209,491,290,420};
	int noloop[3] = {0};
	afficher_image("./Ressources/Menu/menu.bmp",origine);
	actualiser();
	while(1)
		{
	 	curseur = deplacement_souris_a_eu_lieu();
		clic = clic_a_eu_lieu();
		if(pointInBouton(clic, start)) 
			return 1;
		else if(pointInBouton(clic, exit)) 
			return 0;
		if(noloop[0] == 0 && pointInBouton(curseur, start))
			{
			afficher_image("./Ressources/Menu/menu-start.bmp",origine);
			noloop[0] = 1;
			noloop[1] = 0;
			noloop[2] = 0;
			}
		else if(noloop[1] == 0 && pointInBouton(curseur, exit))
			{
			afficher_image("./Ressources/Menu/menu-exit.bmp",origine);
			noloop[0] = 0;
			noloop[1] = 1;
			noloop[2] = 0;
			}
		else if(noloop[2] == 0 && pointNotInZone(curseur, boutons))
			{
			afficher_image("./Ressources/Menu/menu.bmp",origine);
			noloop[0] = 0;
			noloop[1] = 0;
			noloop[2] = 1;
			}
		actualiser();
		traiter_evenements();
		}
	}
    
// FONCTION QUI AFFICHE LE PLATEAU
void afficherPlateau(void)
    {
    afficher_image("./Ressources/Plateau/plateau.bmp",origine);
    actualiser();
    }
    
// FONCTION QUI TIRE ALEATOIREMENT LES JETONS DES JOUEURS
int tirageJetons(int nbDejaTires [100] , Main mainjoueurs[5][8] , char tabpot[SIZEPOT][NBJOUEURSMAX] , int nbjoueurs , int *nbLettresRestantes)
	{
	int nbTire; // variable du nombre aléatoire qui va être tiré
	int nbLettresDistribuees = 100 - *nbLettresRestantes ;  
	printf("nbLettresDistribuees = %d\n" , nbLettresDistribuees);
	for(int numeroJetons = 0 ; numeroJetons<7 ; numeroJetons++)
		{	
		for(int numeroJoueur = 0 ; numeroJoueur < nbjoueurs ; numeroJoueur++)	// pour chaque joueur
			{
			if(mainjoueurs[numeroJoueur][numeroJetons] == NULL)
				{
				do
					{
					nbTire = ((rand())%(100)); // tirage aléatoire entre 0 et le nombre de lettres-1
					} 
					while(dejaTire(nbTire , nbDejaTires)); // tant que le nombre tiré a déjà été tiré
				
				nbDejaTires[nbLettresDistribuees] = nbTire ; // tableau contenant les nombres déjà tirés
				nbLettresDistribuees += 1; // incrémente la variable compteur pour le prochain tour de boucle
				mainjoueurs[numeroJoueur][numeroJetons] = tabpot[nbTire]; // affecte la lettre du pot correspondant au nombre tiré dans le tableau mainJoueurs dans la case numéroJoueur et numéroJeton  
					
				}
			}
		}
	nbLettresRestantes[0] = nbLettresRestantes[0] - nbLettresDistribuees ;
	return nbLettresDistribuees ; // return compteur pour connaitre le nombre de lettres restantes ( = nblettres - compteur)
	}

// FONCTION QUI AFFICHE LA MAIN D'UN JOUEUR
void affichageMainJoueur(int numeroJoueur, Main mainjoueurs[5][8])
	{
	int tailleCase = 50 ;
	int tailleTrait = 3 ;
	Point origineMainJoueur1 = {318, 875} ; // Point origine de la main du joueur1
	Point origineMainJoueur2 = {318, 42} ; // du joueur2
	Point origineMainJoueur3 = {42, 318} ; // du joueur3
	Point origineMainJoueur4 = {875, 318} ; // du joueur4
	char path [40] ; // variable qui correspond au chemin de l'image de la lettre
	int numeroJetons;
	
	for(numeroJetons = 0 ; numeroJetons < 7 ; numeroJetons ++) // boucle pour afficher les 7 jetons de chaque joueur
		{
		sprintf(path,"./Ressources/Lettres/%s.bmp",mainjoueurs[numeroJoueur-1][numeroJetons]); //joueur1
		if(numeroJoueur == 1)
			{
			afficherLettre(path,origineMainJoueur1);
			origineMainJoueur1.x += tailleCase - tailleTrait ; // augmente l'abscisse de la taile d'une case moins la taille d'un trait pour afficher le prochain jeton
			}
		if(numeroJoueur == 2)
			{
			afficherLettre(path,origineMainJoueur2);
			origineMainJoueur2.x += tailleCase - tailleTrait ; // augmente l'abscisse de la taile d'une case moins la taille d'un trait pour afficher le prochain jeton
			}
		if(numeroJoueur == 3)
			{
			afficherLettre(path,origineMainJoueur3);
			origineMainJoueur3.y += tailleCase - tailleTrait ; // augmente l'ordonnée de la taile d'une case moins la taille d'un trait pour afficher le prochain jeton
			}
		if(numeroJoueur == 4)
			{
			afficherLettre(path,origineMainJoueur4);
			origineMainJoueur4.y += tailleCase - tailleTrait ; // augmente l'ordonnée de la taile d'une case moins la taille d'un trait pour afficher le prochain jeton
			}
		}
	}
	
// FOCNTION QUI AFFICHE LE JETON DANS LA GRILLE
void afficherDeplacementJeton(Point clic , int numeroJeton , int numeroJoueur , Main mainjoueurs[5][8] , Plateau plateau [NBCASES][NBCASES])
	{
	Point origineGrille = {130,130};
	clic.x = clic.x - ((clic.x-origineGrille.x)%47);
	clic.y = clic.y - ((clic.y-origineGrille.y)%47);
	int colonneGrille = ((clic.x-origineGrille.x)/47); /// test
	printf("colonne : %d\n",colonneGrille); /// debug
	int ligneGrille = ((clic.y-origineGrille.y)/47); /// test
	printf("ligne : %d\n",ligneGrille); /// debug
	char path [40] ; // variable qui correspond au chemin de l'image de la lettre
	sprintf(path,"./Ressources/Lettres/%s.bmp",mainjoueurs[numeroJoueur-1][numeroJeton]);
	clic.x = clic.x - ((clic.x-origineGrille.x)%47);
	clic.y = clic.y - ((clic.y-origineGrille.y)%47);
	afficherLettre(path,clic);
	plateau [ligneGrille][colonneGrille] = mainjoueurs[numeroJoueur-1][numeroJeton];
	mainjoueurs[numeroJoueur-1][numeroJeton] = NULL ;
	}
	
// FOCNTION QUI REGARDE SI LE NOMBRE ALEATOIRE QUI VIENT D'ETRE TIRE A DEJA ETAIT TIRE
int dejaTire(int nbTire , int nbDejaTires[100])
	{
	for(int i = 0 ; i < 100 ; i++)
		{
		if(nbTire == nbDejaTires[i])
			{
			return 1;
			}
		}
	return 0;
	}

// CALCULE LE SCORE DU MOT ECRIT PAR LE JOUEUR	
int testWordValue(char *plateau[NBCASES][NBCASES], Point premierJetonPoseDuTourParJoueur[5], int numeroJoueur, char direction[5])
	{
	char letterScore[27][3] = {{'a','1'}, //correspondance lettre/valeur
							   {'b','3'},
							   {'c','3'},
							   {'d','2'},
							   {'e','1'},
							   {'f','4'},
							   {'g','2'},
							   {'h','4'},
							   {'i','1'},
							   {'j','8'},
							   {'k','5'},
							   {'l','1'},
							   {'m','3'},
							   {'n','1'},
							   {'o','1'},
							   {'p','3'},
							   {'q','0'}, //char donc 1 caractère
							   {'r','1'},
							   {'s','1'},
							   {'t','1'},
							   {'u','1'},
							   {'v','4'},
							   {'w','4'},
							   {'x','8'},
							   {'y','4'},
							   {'z','0'}}; //char donc 1 caractère
	Point p;
	Point originePlateau = {128,128};
	int plateauBonus[NBCASES][NBCASES]; //initialisation du plateau bonus
	for(int l = 0; l < 15; l++)
		{
		for(int c = 0; c < 15; c++)
			{
			p.x = originePlateau.x + (c*47);
			p.y = originePlateau.y + (l*47);
			if(couleur_point(p) == 0xF0001D) //couleur rouge du plateau (triple word score)
				plateauBonus[l][c] = 4;
			else if(couleur_point(p) == 0xF893BA) //couleur rose du plateau (double word score)
				plateauBonus[l][c] = 3;
			else if(couleur_point(p) == 0x45ADF8) //couleur bleu foncé du plateau (triple letter score)
				plateauBonus[l][c] = 2;
			else if(couleur_point(p) == 0xA2F0EA) //couleur bleu ciel du plateau (double letter score)
				plateauBonus[l][c] = 1;
			else
				plateauBonus[l][c] = 0;
			}
		}
	
	int letterValue[15] = {0};
	int wordValue = 0;
	int pos = 0;
	int lenWord = 0;
	int multiplicateur = 1;
	int numeroLettre = 0;
	int l = premierJetonPoseDuTourParJoueur[numeroJoueur-1].y;
	int c = premierJetonPoseDuTourParJoueur[numeroJoueur-1].x;
	printf("l,c=%d;%d\n", l, c);
	if(direction[numeroJoueur-1] == 'h')
		{
		while(plateau[l][c-1])
			c-- ;
		while(plateau[l][c] != NULL)
			{
			numeroLettre = *plateau[l][c] - 'a';
			letterValue[pos] = (int)(letterScore[numeroLettre][1]) - '0';
			if(letterValue[pos] == 0)
				letterValue[pos] += 10; //Modification des 0 en 10 (dû au tableau de char)
			if(plateauBonus[l][c] == 1)
				letterValue[pos] *= 2; //Attribution du bonus *2 (pour la lettre)
			else if(plateauBonus[l][c] == 2)
				letterValue[pos] *= 3; //Attribution du bonus *3 (pour la lettre)
			else if(plateauBonus[l][c] == 3)
				multiplicateur *= 2; //Attribution du bonus *2 (pour le mot)
			else if(plateauBonus[l][c] == 4)
				multiplicateur *= 3; //Attribution du bonus *3 (pour le mot)
			c++;
			pos++;
			lenWord++; 
			}
		}
		
	if(direction[numeroJoueur-1] == 'v')
		{
		while(plateau[l-1][c])
			l-- ;
		while(plateau[l][c] != NULL)
			{
			numeroLettre = *plateau[l][c] - 'a';
			letterValue[pos] = (int)(letterScore[numeroLettre][1]) - '0';
			if(letterValue[pos] == 0)
				letterValue[pos] += 10; //Modification des 0 en 10 (dû au tableau de char)
			if(plateauBonus[l][c] == 1)
				letterValue[pos] *= 2; //Attribution du bonus *2 (pour la lettre)
			else if(plateauBonus[l][c] == 2)
				letterValue[pos] *= 3; //Attribution du bonus *3 (pour la lettre)
			else if(plateauBonus[l][c] == 3)
				multiplicateur *= 2; //Attribution du bonus *2 (pour le mot)
			else if(plateauBonus[l][c] == 4)
				multiplicateur *= 3; //Attribution du bonus *3 (pour le mot)
			l++;
			pos++;
			lenWord++;
			}
		}
		
	if(direction[numeroJoueur-1] == 'n')
		{
		while(plateau[l-1][c])
			l-- ;
		while(plateau[l][c] != NULL)
			{
			numeroLettre = *plateau[l][c] - 'a';
			letterValue[pos] = (int)(letterScore[numeroLettre][1]) - '0';
			if(letterValue[pos] == 0)
				letterValue[pos] += 10; //Modification des 0 en 10 (dû au tableau de char)
			if(plateauBonus[l][c] == 1)
				letterValue[pos] *= 2; //Attribution du bonus *2 (pour la lettre)
			else if(plateauBonus[l][c] == 2)
				letterValue[pos] *= 3; //Attribution du bonus *3 (pour la lettre)
			else if(plateauBonus[l][c] == 3)
				multiplicateur *= 2; //Attribution du bonus *2 (pour le mot)
			else if(plateauBonus[l][c] == 4)
				multiplicateur *= 3; //Attribution du bonus *3 (pour le mot)
			l++;
			pos++;
			lenWord++;
			}


		for(int i = 0; i < lenWord; i++)
			wordValue += letterValue[i];
		wordValue *= multiplicateur;
			
		while(plateau[l][c-1])
			c-- ;
		while(plateau[l][c] != NULL)
			{
			numeroLettre = *plateau[l][c] - 'a';
			letterValue[pos] = (int)(letterScore[numeroLettre][1]) - '0';
			if(letterValue[pos] == 0)
				letterValue[pos] += 10; //Modification des 0 en 10 (dû au tableau de char)
			if(plateauBonus[l][c] == 1)
				letterValue[pos] *= 2; //Attribution du bonus *2 (pour la lettre)
			else if(plateauBonus[l][c] == 2)
				letterValue[pos] *= 3; //Attribution du bonus *3 (pour la lettre)
			else if(plateauBonus[l][c] == 3)
				multiplicateur *= 2; //Attribution du bonus *2 (pour le mot)
			else if(plateauBonus[l][c] == 4)
				multiplicateur *= 3; //Attribution du bonus *3 (pour le mot)
			c++;
			pos++;
			lenWord++;
			}

		int wordValue2 = 0;
		for(int i = 0; i < lenWord; i++)
			wordValue2 += letterValue[i];
		wordValue2 *= multiplicateur;
		
		if(wordValue > wordValue2)
			return wordValue;
		else 
			return wordValue2;

		}
	if(lenWord >= 2)
		for(int i = 0; i < lenWord; i++)
			wordValue += letterValue[i];
	wordValue *= multiplicateur; 
	return wordValue;
	}
	
// FONCTION QUI TESTE SI LE JETON RESPECTE SI LES CONDITIONS NECESSAIRES POUR ETRE POSE LA OU LE JOUER A CLIQUE DANS LA GRILLE
int estVoisin (Point clic , int clicPrecedentX , int clicPrecedentY , char direction[5] , int numeroJoueur , Plateau plateau [NBCASES][NBCASES] , Point premierJetonPoseDuTourParJoueur [5])
	{
	Point origineGrille = {130,130};
	clic.x = clic.x - ((clic.x-origineGrille.x)%47);
	clic.y = clic.y - ((clic.y-origineGrille.y)%47);
	int colonneClic = ((clic.x-origineGrille.x)/47);
	int ligneClic = ((clic.y-origineGrille.y)/47);
	
	int z = 0 ;
	
	clicPrecedentX = clicPrecedentX - ((clicPrecedentX-origineGrille.x)%47);
	clicPrecedentY = clicPrecedentY - ((clicPrecedentY-origineGrille.y)%47);
	int colonneClicPrecedent = ((clicPrecedentX-origineGrille.x)/47);
	int ligneClicPrecedent = ((clicPrecedentY-origineGrille.x)/47);;
		
	if(plateau[ligneClic][colonneClic] == NULL && colonneClic == 7 && ligneClic == 7 && (direction[numeroJoueur-1] != 'v') && (direction[numeroJoueur-1] != 'h') && (direction[numeroJoueur-1] != 'n')) // premier jeton de la partie
		{
		direction[numeroJoueur-1] = 'n' ;
		premierJetonPoseDuTourParJoueur [numeroJoueur-1].x = colonneClic ;
		premierJetonPoseDuTourParJoueur [numeroJoueur-1].y = ligneClic ;
		return 1;
		}
		
	if( (direction[numeroJoueur-1] != 'v') && (direction[numeroJoueur-1] != 'h') && (colonneClic == colonneClicPrecedent + 1) && (ligneClic == ligneClicPrecedent) && plateau[ligneClic][colonneClic] == NULL)
		{
		direction[numeroJoueur-1] = 'h' ; // le joueur place son mot à l'horizontale
		return 1;
		}
	else if( (direction[numeroJoueur-1] != 'v') && (direction[numeroJoueur-1] != 'h') && (ligneClic == ligneClicPrecedent + 1) && (colonneClic == colonneClicPrecedent) && plateau[ligneClic][colonneClic] == NULL)
		{
		direction[numeroJoueur-1] = 'v' ; // le joueur place son mot à la verticale
		return 1 ;
		}
	
	if(plateau[ligneClic][colonneClic] == NULL && plateau[ligneClic][colonneClic-1] != NULL)
		{
		z = colonneClic - colonneClicPrecedent ; //distance en cases entre les deux clics
		if ((direction[numeroJoueur-1] == 'h') && (colonneClicPrecedent + z == colonneClic) && ( z > 0 && z < 15 ) && (ligneClic == ligneClicPrecedent))
			return 1 ;
		else if ((direction[numeroJoueur-1] == 'n') && (colonneClicPrecedent + z == colonneClic) && ( z > 0 && z < 15 ) && (ligneClic == ligneClicPrecedent))
			{
			direction[numeroJoueur-1] = 'h' ;
			return 1 ;
			}
		}
	
	if(plateau[ligneClic][colonneClic] == NULL && plateau[ligneClic-1][colonneClic] != NULL) 
		{
		z = ligneClic - ligneClicPrecedent ; //distance en cases entre les deux clics
		if ((direction[numeroJoueur-1] == 'v') && (ligneClicPrecedent + z == ligneClic) && ( z > 0 && z < 15 ) && (colonneClic == colonneClicPrecedent))
			return 1 ;
		else if ((direction[numeroJoueur-1] == 'n') && (ligneClicPrecedent + z == ligneClic) && ( z > 0 && z < 15 ) && (colonneClic == colonneClicPrecedent))
			{
			direction[numeroJoueur-1] = 'v' ;
			return 1 ;
			}
		}
	
	if(plateau[ligneClic][colonneClic] == NULL && direction[numeroJoueur-1] == 'a')
		{
		premierJetonPoseDuTourParJoueur [numeroJoueur-1].x = colonneClic ;
		premierJetonPoseDuTourParJoueur [numeroJoueur-1].y = ligneClic ;
		direction[numeroJoueur-1] = 'n';
		return 1;
		}
	
	return 0 ;
	}


// FONCTION QUI REGARDE SI LE CLIC EST SUR UN BOUTON
int pointInBouton(Point p, Bouton b) //Capte si le point donné est dans la zone du bouton
	{
	if(p.x >= b.xMin && p.x <= b.xMax && p.y >= b.yMin && p.y <= b.yMax) 
		return 1;
	else
		return 0;
	}

// FONCTION QUI REGARDE SI LE POINT N'EST DANS UNE CERTAINE ZONE
int pointNotInZone(Point p, Zone z) //Capte si le point donné n'est pas dans la zone donnée
	{
	if(p.x < z.xMin || p.x > z.xMax || p.y < z.yMin || p.y > z.yMax) 
		return 1;
	else
		return 0;
	}

// FONCTION QUI REGARDE SI LE POINT EST DANS UNE CERTAINE ZONE
int pointInZone(Point p, Zone z) //Capte si le point donné est dans la zone donnée
	{
	if(p.x >= z.xMin && p.x <= z.xMax && p.y >= z.yMin && p.y <= z.yMax) 
		return 1;
	else
		return 0;
	}

// FONCTION QUI AFFICHE LES MENUS AVANT ET APRES LA PARTIE
int menu_joueur(void)
	{
	Point curseur;
	Point clic;
	Bouton deux = {209,267,384,442};
	Bouton trois = {321,379,383,441};
	Bouton quatre = {433,491,384,442};
	Zone boutons = {209,491,383,442};
	int noloop[4] = {0};
	afficher_image("./Ressources/Menu/menu-joueurs.bmp",origine);
	actualiser();
	while(1)
		{
		curseur = deplacement_souris_a_eu_lieu();
		clic = clic_a_eu_lieu();
		if(pointInBouton(clic, deux))
			return 2;
		else if(pointInBouton(clic, trois))
			return 3;
		else if(pointInBouton(clic, quatre))
			return 4;
		if(noloop[0] == 0 && pointInBouton(curseur, deux))
			{
			afficher_image("./Ressources/Menu/menu-joueurs-2.bmp",origine);
			noloop[0] = 1;
			noloop[1] = 0;
			noloop[2] = 0;
			noloop[3] = 0;
			}
		else if(noloop[1] == 0 && pointInBouton(curseur, trois))
			{
			afficher_image("./Ressources/Menu/menu-joueurs-3.bmp",origine);
			noloop[0] = 0;
			noloop[1] = 1;
			noloop[2] = 0;
			noloop[3] = 0;
			}
		else if(noloop[2] == 0 && pointInBouton(curseur, quatre)) 
			{
			afficher_image("./Ressources/Menu/menu-joueurs-4.bmp",origine);
			noloop[0] = 0;
			noloop[1] = 0;
			noloop[2] = 1;
			noloop[3] = 0;
			}
		else if(noloop[3] == 0 && pointNotInZone(curseur, boutons))
			{
			afficher_image("./Ressources/Menu/menu-joueurs.bmp",origine);
			noloop[0] = 0;
			noloop[1] = 0;
			noloop[2] = 0;
			noloop[3] = 1;
			}
		actualiser();
		traiter_evenements();
		}
	}

// FONCTION QUI AFFCIHE LES INFOS TELLES QUE LES LETTRES RESTANTES ET LES SCORES DES JOUEURS
void afficher_infos(int nbjoueurs, int score[4], int nbLettresRestantes [1])
	{
	Point clearScore = {1035, 352};
	Point clearLettresRestantes = {935,558};
	afficher_image("./Ressources/Plateau/zoneScore.bmp",clearScore);						//clear	
	afficher_image("./Ressources/Plateau/zoneLettresRestantes.bmp",clearLettresRestantes);	//clear
	char scoreJoueur[4];
	Point pJoueurs = {1037, 353};
	for(int i = 1; i <= nbjoueurs; i++) //affichage des scores
		{
		sprintf(scoreJoueur, "%d", score[i-1]);  //conversion int en string
		afficher_texte(scoreJoueur, 20, pJoueurs, noir);
		pJoueurs.y += 21;
		}
	char lettresPot[4];
	Point pLettres = {962, 569};
	sprintf(lettresPot,"%d", nbLettresRestantes[0]); //conversion int en string
	afficher_texte(lettresPot, 20, pLettres, noir); //affichage du nombre de lettres restantes
	actualiser();
	}
	
// FONCTION QUI REAFFICHE TOUS LES ELEMENTS DES MAINS DES JOUEURS ET DU PLATEAU (SI LE MOT DU JOUEUR EST FAUX)
void refreshJeu(char *plateau[NBCASES][NBCASES], char *mainjoueurs[5][8], int nbjoueurs)
	{
	Points origines = {{318, 875}, {318, 42}, {42, 318}, {875, 318}};
	Point p;
	char path[40];
	Point origineZonePlateau = {123,123};
	afficher_image("./Ressources/Plateau/zonePlateau.bmp",origineZonePlateau);
	for(int l = 0; l < 15; l++)
		{ 
		for(int c = 0; c < 15; c++)
			{
			if(plateau[l][c] != NULL)
				{
				sprintf(path,"./Ressources/Lettres/%s.bmp", plateau[l][c]);
				p.x = 130 + (c * 47) ;
				p.y = 130 + (l * 47) ;
				afficherLettre(path, p);
				}
			}
		}
	Point jX;
	for (int j = 0; j < nbjoueurs; j++)
		{	
		for(int cell = 0; cell < 7; cell++)
			{
			if(mainjoueurs[j][cell] != NULL)
				{
				sprintf(path, "./Ressources/Lettres/%s.bmp", mainjoueurs[j][cell]);
				if(j == 0)
					{
					jX.x = origines.j1.x + (cell * 47);
					jX.y = origines.j1.y;
					}
				else if(j == 1)
					{
					jX.x = origines.j2.x + (cell * 47);
					jX.y = origines.j2.y;
					}
				else if(j == 2)
					{
					jX.x = origines.j3.x;
					jX.y = origines.j3.y + (cell * 47);
					}
				else
					{
					jX.x = origines.j4.x;
					jX.y = origines.j4.y + (cell * 47);
					}
				afficherLettre(path, jX);
				}
			}
		}
	}	

//FONCTION QUI SERT A REUNIR TOUTES LES FONCTIONS NECESSAIRE AU JEU ET ORGANISER LE DEROULEMENT DE LA PARTIE
int jeu (Main mainjoueurs[5][8] , Plateau plateau [NBCASES][NBCASES] , char tabpot[SIZEPOT][NBJOUEURSMAX], int nbLettresDistribuees , int nbLettresDepart , int nbLettresRestantes [1] , int nbjoueurs , int nbDejaTires [100] , char tabdico[SIZEDICO][MAXLENMOT])
	{
	int numeroJoueur = 1;
	int numeroJeton;
	int score[4] = {0};
	int scoreMax = score[0];
	int noloop[4] = {0};

	Bouton fermer = {37, 66, 37, 66};
	Bouton sauver = {127, 156, 37, 66};
	Bouton tester = {217, 246, 37, 66};
	
	Zone boutons = {37, 246, 37, 66};
	Zone zoneJoueur1 = {313, 642, 870, 920};
	Zone zoneJoueur2 = {313, 642, 37, 87};
	Zone zoneJoueur3 = {37, 87, 313, 642};
	Zone zoneJoueur4 = {870, 920, 313, 642};
	Zone zonePlateau = {125,833,125,833};

	Point posBoutons[3] = {{37, 37}, {127, 37}, {217, 37}};
	Point premier_clic;
	Point deuxieme_clic;
	Point curseur;

	char direction [5] = {'p','a','a','a'};
	Point positionJetonATester[5] = { {0,0} , {0,0} , {0,0} , {0,0} } ;
	Point clicPrecedent = {0};
	Point premierJetonPoseDuTourParJoueur[5];
	nbLettresRestantes[0] = nbLettresDepart-nbLettresDistribuees;
	afficher_infos(nbjoueurs, score, nbLettresRestantes);
	
	while(scoreMax < 200)
		{
		premier_clic = clic_a_eu_lieu();
		curseur = deplacement_souris_a_eu_lieu();
		//Tests emplacement du clic
		if(pointInZone(premier_clic, zoneJoueur1) && (numeroJoueur == 1) && (couleur_point(premier_clic)!= blanc))
			{
			numeroJeton = quelJeton(numeroJoueur , premier_clic);
			clicPrecedent = tourJeu (clicPrecedent , nbjoueurs , mainjoueurs , tabpot , zonePlateau , premier_clic , numeroJoueur , numeroJeton , plateau , direction , premierJetonPoseDuTourParJoueur);
			}
			
		else if(pointInZone(premier_clic, zoneJoueur2) && (numeroJoueur == 2) && (couleur_point(premier_clic)!= blanc)) 
			{
			numeroJeton = quelJeton(numeroJoueur , premier_clic);
			clicPrecedent = tourJeu (clicPrecedent , nbjoueurs , mainjoueurs , tabpot , zonePlateau , premier_clic , numeroJoueur , numeroJeton , plateau , direction , premierJetonPoseDuTourParJoueur);
			}
			
		else if(pointInZone(premier_clic, zoneJoueur3) && (numeroJoueur == 3) && (couleur_point(premier_clic)!= blanc)) 
			{
			numeroJeton = quelJeton(numeroJoueur , premier_clic);
			clicPrecedent = tourJeu (clicPrecedent , nbjoueurs , mainjoueurs , tabpot , zonePlateau , premier_clic , numeroJoueur , numeroJeton , plateau , direction , premierJetonPoseDuTourParJoueur);
			}
			
		else if(pointInZone(premier_clic, zoneJoueur4) && (numeroJoueur == 4) && (couleur_point(premier_clic)!= blanc)) 
			{
			numeroJeton = quelJeton(numeroJoueur , premier_clic);
			clicPrecedent = tourJeu (clicPrecedent , nbjoueurs , mainjoueurs , tabpot , zonePlateau , premier_clic , numeroJoueur , numeroJeton , plateau , direction , premierJetonPoseDuTourParJoueur);
			}
		else if(pointInBouton(premier_clic, tester))
			{
			if(nbJetons(mainjoueurs, numeroJoueur) == 7) //Passer tour si le joueur possède tous ses jetons
				{
				if(numeroJoueur == nbjoueurs) //Tour complet
					{
					remiseAZeroDirection(nbjoueurs , direction);
					tirageJetons(nbDejaTires , mainjoueurs , tabpot , nbjoueurs , nbLettresRestantes);
					copieTableauMainJoueurs(tableauSauvegardeMainsJoueur , mainjoueurs);
					for(int numeroJoueur = 1 ; numeroJoueur <= nbjoueurs ; numeroJoueur++)
					affichageMainJoueur(numeroJoueur,mainjoueurs); // appel de la fonction affichageMainJoueur pour afficher les jetons des joueurs
					numeroJoueur = 1;
					}
				else //Tour lambda
					numeroJoueur++;
				}
			else if(motEstDansLeDictionnaire(premierJetonPoseDuTourParJoueur, plateau, numeroJoueur, tabdico, direction))
				{
				printf("mot existe\n");
				copieTableauPlateau( plateauSauvegarde ,plateau);
				score[numeroJoueur-1] += testWordValue(plateau, premierJetonPoseDuTourParJoueur, numeroJoueur, direction);
				if(nbJetons(mainjoueurs, numeroJoueur) == 0)
					score[numeroJoueur-1] += 50; //Bonus de 50 points si toutes les lettres sont posées
				afficher_infos(nbjoueurs, score, nbLettresRestantes);
				scoreMax = maxScore(score, nbjoueurs);
				if(numeroJoueur == nbjoueurs) //Tour complet
					{
					remiseAZeroDirection(nbjoueurs , direction);
					tirageJetons(nbDejaTires , mainjoueurs , tabpot , nbjoueurs , nbLettresRestantes);
					copieTableauMainJoueurs(tableauSauvegardeMainsJoueur , mainjoueurs);
					printf("nbLettresRestantes = %d\n" , nbLettresRestantes[0]); //debug
					for(int numeroJoueur = 1 ; numeroJoueur <= nbjoueurs ; numeroJoueur++)
						affichageMainJoueur(numeroJoueur,mainjoueurs); // appel de la fonction affichageMainJoueur pour afficher les jetons des joueurs
					score[numeroJoueur-1] += testWordValue(plateau, premierJetonPoseDuTourParJoueur, numeroJoueur, direction);
					afficher_infos(nbjoueurs, score, nbLettresRestantes);
					numeroJoueur = 1;
					}
				else //Tour lambda
					numeroJoueur++;
				}
			else //Si le mot n'est pas dans le dictionnaire
				{
				printf("mot existe pas\n");
				remiseAZeroDirection(nbjoueurs , direction);
				restaurationMainJoueur(mainjoueurs , tableauSauvegardeMainsJoueur , numeroJoueur);
				restaurationPlateau(plateau , plateauSauvegarde);
				refreshJeu (plateau, mainjoueurs, nbjoueurs);
				}
			}
		else if(pointInBouton(premier_clic, fermer))
			{
			return 0; //On arrête le jeu
			}
		//Tests emplacement du curseur
		if(noloop[0] == 0 && pointInBouton(curseur, fermer))
			{
			afficher_image("./Ressources/Plateau/Boutons/fermerActif.bmp", posBoutons[0]);
			afficher_image("./Ressources/Plateau/Boutons/sauverInactif.bmp", posBoutons[1]);
			afficher_image("./Ressources/Plateau/Boutons/testerInactif.bmp", posBoutons[2]);
			noloop[0] = 1;
			noloop[1] = 0;
			noloop[2] = 0;
			noloop[3] = 0;
			}
		else if(noloop[1] == 0 && pointInBouton(curseur, sauver))
			{
			afficher_image("./Ressources/Plateau/Boutons/fermerInactif.bmp", posBoutons[0]);
			afficher_image("./Ressources/Plateau/Boutons/sauverActif.bmp", posBoutons[1]);
			afficher_image("./Ressources/Plateau/Boutons/testerInactif.bmp", posBoutons[2]);
			noloop[0] = 0;
			noloop[1] = 1;
			noloop[2] = 0;
			noloop[3] = 0;
			}
		else if(noloop[2] == 0 && pointInBouton(curseur, tester)) 
			{
			afficher_image("./Ressources/Plateau/Boutons/fermerInactif.bmp", posBoutons[0]);
			afficher_image("./Ressources/Plateau/Boutons/sauverInactif.bmp", posBoutons[1]);
			afficher_image("./Ressources/Plateau/Boutons/testerActif.bmp", posBoutons[2]);
			noloop[0] = 0;
			noloop[1] = 0;
			noloop[2] = 1;
			noloop[3] = 0;
			}
		else if(noloop[3] == 0 && pointNotInZone(curseur, boutons))
			{
			afficher_image("./Ressources/Plateau/Boutons/fermerInactif.bmp", posBoutons[0]);
			afficher_image("./Ressources/Plateau/Boutons/sauverInactif.bmp", posBoutons[1]);
			afficher_image("./Ressources/Plateau/Boutons/testerInactif.bmp", posBoutons[2]);
			noloop[0] = 0;
			noloop[1] = 0;
			noloop[2] = 0;
			noloop[3] = 1;
			}
		traiter_evenements();
		actualiser();
		}
	int gagnant = 0; //Tests pour savoir quel joueur à le plus de points
	if(score[0] == scoreMax)
		gagnant = 1;
	else if(score[1] == scoreMax)
		gagnant = 2;
	else if(score[2] == scoreMax)
		gagnant = 3;
	else 
		gagnant = 4;
	attendre_clic();
	return gagnant;
	}
	
//FONCTION QUI SERT A ORGANISER LE TOUR D UN JOUEUR
Point tourJeu (Point clicPrecedent , int nbjoueurs , Main mainjoueurs[5][8] , char tabpot[SIZEPOT][NBJOUEURSMAX] , Zone zonePlateau , Point premier_clic , int numeroJoueur , int numeroJeton , Plateau plateau [NBCASES][NBCASES] , char direction [5] , Point premierJetonPoseDuTourParJoueur [5])
	{
	Point deuxieme_clic ;
	
	premier_clic = origineJetonPourRepaint(premier_clic , numeroJoueur);
	deuxieme_clic = clic_a_eu_lieu();
	while( (deuxieme_clic.x == -1) && (deuxieme_clic.y == -1) )
		{
		deuxieme_clic = clic_a_eu_lieu();
		if( (pointInZone(deuxieme_clic, zonePlateau)) && (estVoisin(deuxieme_clic , clicPrecedent.x , clicPrecedent.y , direction , numeroJoueur, plateau , premierJetonPoseDuTourParJoueur)) ) // s'il y a eu un clic dans la grille
			{
			afficherDeplacementJeton(deuxieme_clic , numeroJeton , numeroJoueur , mainjoueurs , plateau);
			clicPrecedent.x = deuxieme_clic.x ;
			clicPrecedent.y = deuxieme_clic.y ;
			dessiner_rectangle(premier_clic , 44 , 44 , blanc);
			actualiser();
			}
		traiter_evenements();
		}
	return clicPrecedent ;
	}


// FONCTION QUI DONNE LE NUMERO DU JETON (DANS LA MAIN DU JOUEUR)
int quelJeton (int numeroJoueur , Point premier_clic)
	{
	int numeroJeton ;
	if(numeroJoueur == 1 || numeroJoueur == 2)
		numeroJeton = (premier_clic.x - ((premier_clic.x-316)%47))/47-6;
	else 
		numeroJeton = (premier_clic.y - ((premier_clic.y-316)%47))/47-6;
	
	return numeroJeton ;
	}

// FONCTION QUI REMET A 0 LES DIRECTIONS DANS LESQUELLES LES JOUEURS AVAIENT PLACE LEURS JETONS
void remiseAZeroDirection(int nbjoueurs , char direction[5])
	{
	for(int i = 0 ; i < nbjoueurs ; i++)
		direction[i] = 'a' ;
	}
	
// FONCTION QUI DONNE LE POINT ORIGINE DE LA CASE A REPEINDRE DANS LA MAIN DU JOUEUR UNE FOIS QUE LE JETON A ETE PLACE DANS LA GRILLE
Point origineJetonPourRepaint(Point premier_clic , int numeroJoueur)
	{
	Point origineJeton ;
	if(numeroJoueur == 1)
		{
		origineJeton.x = premier_clic.x - ((premier_clic.x-316)%47);
		origineJeton.y = premier_clic.y - ((premier_clic.y-873)%47);
		}
	if(numeroJoueur == 2)
		{
		origineJeton.x = premier_clic.x - ((premier_clic.x-316)%47);
		origineJeton.y = premier_clic.y - ((premier_clic.y-40)%47);
		}
	if(numeroJoueur == 3)
		{
		origineJeton.x = premier_clic.x - ((premier_clic.x-40)%47);
		origineJeton.y = premier_clic.y - ((premier_clic.y-316)%47);
		}
	if(numeroJoueur == 4)
		{
		origineJeton.x = premier_clic.x - ((premier_clic.x-873)%47);
		origineJeton.y = premier_clic.y - ((premier_clic.y-316)%47);
		}
		
	return origineJeton;
	}
	
// FONCTION QUI COMPTE LE NOMBRE DE JETONS RESTANTS DNS LA MAIN DU JOUEUR
int nbJetons(char *mainjoueurs[5][8], int numeroJoueur)
	{
	int compteur = 0;
	for(int pos = 0; pos < 7; pos++) //Position dans la main du joueur
		{
		if(mainjoueurs[numeroJoueur-1][pos] != NULL)
			compteur++;
		}
	return compteur;
	}
	
// FONCTION QUI COPIE LE TABLEAU CONTENANT LES MAINS DES JOUEURS DANS UN TABLEAU DE SAUVEGARDE (SI LE MOT DU JOUEUR EST FAUX)
void copieTableauMainJoueurs(Main tableauSauvegardeMainsJoueur [5][8] , Main mainjoueurs[5][8])
	{
	for(int i = 0 ; i < 4 ; i++)
		for(int j = 0 ; j < 7 ; j++)
			{
			tableauSauvegardeMainsJoueur[i][j] = mainjoueurs[i][j] ;
			printf("tableauSauvegardeMainsJoueur[%d][%d] =%s\n",i ,j , tableauSauvegardeMainsJoueur[i][j]);
			}
	}
	
// FONCTION QUI COPIE LE TABLEAU CONTENANT LES JETONS PLACES SUR LA GRILLE DANS UN TABLEAU DE SAUVEGARDE (SI LE MOT DU JOUEUR EST FAUX)
void copieTableauPlateau(Plateau plateauSauvegarde[NBCASES][NBCASES] , Plateau plateau[NBCASES][NBCASES])
	{
	for(int i = 0 ; i < NBCASES-1 ; i++)
		for(int j = 0 ; j < NBCASES-1 ; j++)
			plateauSauvegarde[i][j] = plateau[i][j] ;
	}
	
// FONCTION QUI RESTAURE LES MAINS DES JOUEURS (SI LE MOT DU JOUEUR EST FAUX)
void restaurationMainJoueur(Main mainjoueurs[5][8] , Main tableauSauvegardeMainsJoueur [5][8] , int numeroJoueur)
	{
	for(int i = 0 ; i < 7 ; i++)
		mainjoueurs[numeroJoueur-1][i] = tableauSauvegardeMainsJoueur[numeroJoueur-1][i];
	}
	
// FONCTION QUI RESTAURE LE PLATEAU (SI LE MOT DU JOUEUR EST FAUX)
void restaurationPlateau(Plateau plateau[NBCASES][NBCASES] , Plateau plateauSauvegarde[NBCASES][NBCASES] )
	{
	for(int i = 0 ; i < NBCASES-1 ; i++)
		for(int j = 0 ; j < NBCASES-1 ; j++)
			plateau[i][j] = plateauSauvegarde[i][j];
	}
	
//FONCTION QUI DETECTE UN MOT SUR LA GRILLE ET REGARDE SI CELUI-CI EST DANS LE DICTIONNAIRE
int motEstDansLeDictionnaire (Point premierJetonPoseDuTourParJoueur [5] , Plateau plateau [NBCASES][NBCASES] , int numeroJoueur , char tabdico[SIZEDICO][MAXLENMOT] , char direction [5])
	{
	char mot [16];
	if (direction[numeroJoueur - 1] == 'h')
		{
		while( (plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x-1]) != NULL)
			{
			premierJetonPoseDuTourParJoueur[numeroJoueur-1].x -=1 ;
			printf("premierJetonPoseDuTourParJoueur[numeroJoueur-1].x -=1 \n"); ///debug 
			}
		printf("plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x] = %s \n" , plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]); ///debug
		strcpy(mot , plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]);
		while(plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x+1] != NULL)
			{
			premierJetonPoseDuTourParJoueur[numeroJoueur-1].x += 1 ;
			strcat(mot , plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]);
			}
		}
	else
		{
		int i = 0;
		while( (plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y-1][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]) != NULL)
			{
			premierJetonPoseDuTourParJoueur[numeroJoueur-1].y -=1 ;
			printf("premierJetonPoseDuTourParJoueur[numeroJoueur-1].y -=1 \n"); ///debug 
			}
		printf("plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x] = %s \n" , plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]); ///debug
		strcpy(mot , plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]);
		while(plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y+1][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x] != NULL)
			{
			premierJetonPoseDuTourParJoueur[numeroJoueur-1].y += 1 ;
			strcat(mot , plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]);
			i++;
			}
		if(i <= 1)
			{
			while( (plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x-1]) != NULL)
				{
				premierJetonPoseDuTourParJoueur[numeroJoueur-1].x -=1 ;
				printf("premierJetonPoseDuTourParJoueur[numeroJoueur-1].x -=1 \n"); ///debug 
				}
			printf("plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x] = %s \n" , plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]); ///debug
			strcpy(mot , plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]);
			while(plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x+1] != NULL)
				{
				premierJetonPoseDuTourParJoueur[numeroJoueur-1].x += 1 ;
				strcat(mot , plateau[premierJetonPoseDuTourParJoueur[numeroJoueur-1].y][premierJetonPoseDuTourParJoueur[numeroJoueur-1].x]);
				}
			}
		}
		
	int debut = 0 ;
	int fin = SIZEDICO - 2 ;
	int milieu = (debut + fin) / 2 ;
	while( (debut <= fin) && ((strcmp(mot , tabdico[milieu])) != 0) )
		{
			if( (strcmp(mot , tabdico[milieu])) > 0 )
				{
				printf("tabdico[milieu] = %s \n" , tabdico[milieu]);
				printf("debut = milieu + 1 \n"); ///debug
				debut = milieu + 1 ;
				}
			else 
				{
				printf("tabdico[milieu] = %s \n" , tabdico[milieu]);
				printf("fin = milieu - 1 \n"); ///debug
				fin = milieu - 1 ;
				}
			milieu = (debut + fin) / 2 ;
		}
	if((strcmp(mot , tabdico[milieu])) == 0)
		{
		printf("mot : %s\n" , mot); ///debug
		return 1 ; 
		}
	else
		{
		printf("mot : %s\n" , mot); ///debug
		return 0 ;
		}
	}
	
	
// FOCNTION QUI CHARGE LE DICTIONNAIRE GRACE A UN FICHIER
	int chargeDico(char *filedico, char tabdico[SIZEDICO][MAXLENMOT])
{
    FILE *f = fopen(filedico,"r"); // ouverture du fichier

    if (!f) // si ouverture ratée
        {
        fprintf(stderr,"fopen: problème d'ouverture du fichier '%s'\n'",filedico);
        return 0;
        }

    char line[MAXLENMOT]; // la ligne servant à la lecture du fichier

    int mots=0; // indice dans tabdico 
    while(fgets(line,MAXLENMOT,f)) // tant que fgets ne renvoie pas nul (eof)
        {
        sscanf(line,"%s\n",tabdico[mots]); // je scanne line et écris dans tabdico
        mots++;
        }
    fclose(f); // fermeture du fichier
    
    return mots;
}

// FONCTION QUI SERT A AFFICHER LES LETTRES SUR LA GRILLE (POUR EVITER LES COINS BLANCS DES JETONS)
void afficherLettre(char path[40], Point p) //Afficher les lettres bmp sous forme d'une fausse transparence
    {
    Point bordureHaute = {p.x + 2, p.y};
    Point bordureBasse = {p.x + 2, p.y + 39};
    Point bordureDroite = {p.x + 39, p.y + 2};
    Point bordureGauche = {p.x, p.y + 2};
    p.x++;
    p.y++;
    afficher_image(path, p); //Afficher lettre (38x38)
    afficher_image("./Ressources/Lettres/bordureHorizontale.bmp", bordureHaute); //Afficher la bordure haute
    afficher_image("./Ressources/Lettres/bordureHorizontale.bmp", bordureBasse); //Afficher la bordure basse
    afficher_image("./Ressources/Lettres/bordureVerticale.bmp", bordureDroite); //Afficher la bordure droite
    afficher_image("./Ressources/Lettres/bordureVerticale.bmp", bordureGauche); //Afficher la bordure gauche
    }
    
// FONCTION QUI CHERCHE LE SCORE MAXIMUM DANS LE TABLEAU SCORE
int maxScore(int score[4], int nbjoueurs)
	{
	int scoreMax = score[0];
	for(int j = 2; j <= nbjoueurs; j++)
		{
		if(score[j-1] > scoreMax)
			scoreMax = score[j-1];
		}
	return scoreMax;
	}
