#include <menu.h>
#include <stdlib.h>
#include <stdio.h>
#include "fonctions.h"
#include <string.h>
#include <time.h>
#include <unistd.h>


#define ESCAPE 27

void initcurses()
{
	initscr();
    cbreak();
    noecho();
	initColors();			// initialisation couleurs
    init_pair(1,COLOR_RED,COLOR_BLACK);		//couleur titre menu
	curs_set(0); 			// initalisation curseur
    keypad(stdscr,TRUE);	// initialisation clavier
}

void creation_items(int n_choices, char *choices[], ITEM **my_items)
{
	int i;
	for(i = 0; i < n_choices; i++)
	{
		my_items[i] = new_item(choices[i], "");
	}
}

void titre_menu(WINDOW *win, char *titre)
{
	box(win, 0, 0);
	print_in_middle(win, 1, 0, 40, titre, COLOR_PAIR(1));
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 38);
	mvwaddch(win, 2, 39, ACS_RTEE);
	//mvprintw(LINES - 1, 0, "F2 pour quitter");
	refresh();
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int taille, x, y;
	float temp;
	
	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	taille = strlen(string);
	temp = (width - taille)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

void dep_menu(MENU *menu, WINDOW *win)
{
	int c;
	while(1)						// avant : while(c != KEY_F(2))
	{
			c = getch(); 
		    switch(c)
	        {	
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				if (highlight < 5)				highlight ++;
				
			break;
			
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				if (highlight > 0)				highlight --;
			break;
			
			case ENTER: /* Enter */
				choix_menu();
			break;
			}
                wrefresh(win);
	}	

}

void dep_menu_lvl(MENU *menu, WINDOW *win)
{
	int c;
	while(1)						// avant : while(c != KEY_F(2))
	{
			c = getch(); 
		    switch(c)
	        {	
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				if (highlight_lvl < 5)				highlight ++;
				
			break;
			
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				if (highlight_lvl > 0)				highlight --;
			break;
			
			case ENTER: /* Enter */
				choix_menu_lvl();
			break;
			}
                wrefresh(win);
	}	

}

void choix_menu()
{
	switch (highlight) 
	{	
	case 0 :			//Lancer le jeu
		clear();
		refresh();
		prog_princ();
	break;
	case 1 :			//Reprendre
		clear();
		refresh();
		reprendre();
		prog_princ();
	break;	
	case 2 :			//Choisir map
		clear();
		refresh();
		menu_lvl();
	break;	
	case 3 :			//Configuration
		clear();
		refresh();
		endwin();
		exit(0);
	break;	
	case 4 :			//Score
		clear();
		refresh();
		affich_score();
	break;
	case 5 :			//Quitter
		clear();
		refresh();
		endwin();
		exit(0);
	break;				
	}
}

void choix_menu_lvl()
{
	switch (highlight_lvl) 
	{	
	case 0 :			//Map 1
		clear();
		refresh();
		niv = 0;
		prog_princ();
	break;
	case 1 :			//Map 2
		clear();
		refresh();
		niv = 1;
		prog_princ();
	break;	
	case 2 :			//Map 3
		clear();
		refresh();
		niv = 2;
		prog_princ();
	break;	
	case 3 :			//Map 4
		clear();
		refresh();
		niv = 3;
		prog_princ();
	break;	
	case 4 :			//Quitter
		clear();
		refresh();
		endwin();
		exit(0);
	break;			
	}
}

void menu_lvl()
{	
	char *choiceslvl[] = {
                        "               Level 1                 ",
                        "               Level 2                 ", 
                        "               Level 3                 ",
                        "               Level 4                 ",
                        "               Quitter                 ",
                        (char *)NULL,
                  };				
	MENU *my_menu_lvl;
    WINDOW *my_menu_win_lvl;
    int lvl_choices = ARRAY_SIZE(choiceslvl);
    ITEM **my_items_lvl = calloc(lvl_choices, sizeof(ITEM *));
    int i;
    char *titre_lvl = "Choix du Niveau";
	
	initcurses();													// Initialize curses
    creation_items(lvl_choices, choiceslvl, my_items_lvl);					// Creation des choix du menu
    my_menu_lvl = new_menu(my_items_lvl);									// Creation du menu level
	my_menu_win_lvl = newwin(9, 40, LINES/2 - 5, COLS/2 - 19);			// ( hauteur,largeur,ypos,xpos)
	keypad(my_menu_win_lvl, TRUE);
	set_menu_win(my_menu_lvl, my_menu_win_lvl);
	set_menu_sub(my_menu_lvl, derwin(my_menu_win_lvl, 6, 38, 3, 1));
	titre_menu(my_menu_win_lvl, titre_lvl);									// Affichage du titre du menu
	post_menu(my_menu_lvl);
	wrefresh(my_menu_win_lvl);
	dep_menu(my_menu_lvl, my_menu_win_lvl);									// Deplacement dans le menu
	unpost_menu(my_menu_lvl);
	free_menu(my_menu_lvl);
	for(i = 0; i < lvl_choices; i++)
			free_item(my_items_lvl[i]);
	endwin();
}
/******************************************************************Labyrinthe*************************************************************************************/

void initColors()
{ 
    start_color();
    init_pair(1,COLOR_CYAN,COLOR_CYAN);		//mur
    init_pair(2,COLOR_WHITE,COLOR_MAGENTA);
    init_pair(3,COLOR_WHITE,COLOR_RED);			//couleur curseur 
    init_pair(4, COLOR_YELLOW, COLOR_YELLOW); //couleur mur
    init_pair(5, COLOR_WHITE, COLOR_BLUE); //couleur curseur
    init_pair(6, COLOR_WHITE, COLOR_WHITE); //couleur couloir
    init_pair(7, COLOR_RED, COLOR_RED); //sortie
    init_pair(8, COLOR_GREEN, COLOR_GREEN);	// couleur mur
}

void initLab(char *niveau)
{
	 FILE *fichier;  														// recupere le fichier ou est stocker le labyrinthe
    fichier=fopen(niveau,"r");
   
    if(fichier == NULL)
        printf("Erreur lors de l'ouverture du fichier\n"); 					// si le fichier est introuvable, affiche une erreur
    else
    {
        initColors();														// initialise les couleurs du labyrinthe
        while (!feof(fichier))  
        {
            for(nb_ligne=0;nb_ligne<=ymax;nb_ligne++)		
            {
                for(nb_col=1;nb_col<=xmax;nb_col++)
                {
                    fscanf(fichier,"%c",&tab[nb_ligne][nb_col]);
                    switch (tab[nb_ligne][nb_col]){
						case '1':											//affichage des murs
							mur();
						break;
						
						case '0':											//affichage des chemins
							chemins();
                        break;
                        
                        case 'E':											//affichage de l'entrée
							entree();
						break;
						
						case 'S':											//affichage de la sortie
							sortie();
						break;
					}
                    
                    refresh();       
                }
                fscanf(fichier,"%c",&chaine);
            }
        }
        fclose(fichier);   													// ferme le fichier.txt du labyrinthe
    }
}

void depLab()
{
	char *quit = "Appuyez sur F2 pour afficher le menu";
	int taille = strlen(quit);
	keypad(stdscr,TRUE);
	curs_set(0);
    nb_ligne = xInit ;
	nb_col = yInit ;
	moveCursor((LINES/2)-(ymax/2) + nb_ligne, (COLS/2)-(xmax/2) + nb_col);
	mvprintw(LINES - 1,(COLS / 2) - (taille / 2), quit);
	start = time(0);
	while (key != KEY_F(2) && (tab[nb_ligne][nb_col] != 'S'))
	{
		trace_cursor();
		switch (key)
		{
			case KEY_RIGHT:
				droite();
			break;
			
			case KEY_LEFT:
				gauche();					   
			break;
			
			case KEY_DOWN:
				bas();
			break;
			
			case KEY_UP:
				haut();
			break;
		}
		key=getch();
		move((LINES/2)-(ymax/2) + nb_ligne, (COLS/2)-(xmax/2) + nb_col);
		refresh();
	}
	if (key == KEY_F(2))
	{
		save();
		clear();
		refresh();
		menu_princ();
		save();
	}
	else
	{
		fin();
	}
}

void moveCursor(int nb_ligne, int nb_col) 								//fonction déplacement curseur 
{
	int couleur = 0;
	if (niv == 0) couleur = 5;
	if (niv == 1) couleur = 3;
	if (niv == 2) couleur = 2;
	if (niv == 3) couleur = 5;
    move(nb_ligne,nb_col);
    wattrset (stdscr, COLOR_PAIR(couleur));
    addch(perso); 														//curseur dessin
    wattroff (stdscr, COLOR_PAIR(couleur));
}

void trace_cursor()
{
	int couleur = 0;
	if (niv == 0) couleur = 5;
	if (niv == 1) couleur = 3;
	if (niv == 2) couleur = 2;
	if (niv == 3) couleur = 5;
	wattrset (stdscr, COLOR_PAIR(couleur));
}

void mur()
{
	int couleur = 0;
	if (niv == 0) couleur = 8;
	if (niv == 1) couleur = 4;
	if (niv == 2) couleur = 1;
	if (niv == 3) couleur = 7;
	attron(COLOR_PAIR(couleur));
	mvprintw((LINES/2)-(ymax/2) + nb_ligne,(COLS/2)-(xmax/2) + nb_col," ");
	attroff(COLOR_PAIR(couleur));
}

void chemins()
{
	attron(COLOR_PAIR(6));
	mvprintw((LINES/2)-(ymax/2) + nb_ligne,(COLS/2)-(xmax/2) + nb_col," ");
	attroff(COLOR_PAIR(6));
}

void entree()
{
	mvprintw((LINES/2)-(ymax/2) + nb_ligne,(COLS/2)-(xmax/2) + nb_col," ");
	if(is_reload == 0)
	{
		xInit = nb_ligne; 								// initialisation de la position du curseur
		yInit = nb_col;									// initialisation de la position du curseur
	}
	is_reload = 0;
}

void sortie()
{
	int couleur;
	if( niv == 3) couleur = 8;
	else couleur = 7;
	attron(COLOR_PAIR(couleur));
	mvprintw((LINES/2)-(ymax/2) + nb_ligne,(COLS/2)-(xmax/2) + nb_col," ");
	attroff(COLOR_PAIR(couleur));	
}

void droite()
{
	if ( xmax - 1 > nb_col)
	{
		if (tab[nb_ligne][nb_col +1 ] != '1')
		{
			perso = '>';
			addch (' ');
			nb_col ++;
			moveCursor((LINES/2)-(ymax/2) + nb_ligne, (COLS/2)-(xmax/2) + nb_col);
			compteur ++;
		}
	}
}

void gauche()
{
	if (0 < nb_col)
	{
		if (tab[nb_ligne][nb_col - 1] != '1')
		{
			perso = '<';
			addch (' ');
			nb_col --;
			moveCursor((LINES/2)-(ymax/2) + nb_ligne, (COLS/2)-(xmax/2) + nb_col);
			compteur ++;
		}
	}
}

void bas()
{
	if (  ymax - 1 > nb_ligne)
	{
		if (tab[nb_ligne + 1][nb_col] != '1')
		{
			perso = 'v';
			addch (' ');
			nb_ligne ++;
			moveCursor((LINES/2)-(ymax/2) + nb_ligne, (COLS/2)-(xmax/2) + nb_col);
			compteur ++;
		}
	}
}

void haut()
{
	if (0 < nb_ligne)
	{
		if (tab[nb_ligne - 1][nb_col] != '1')
		{
			perso = '^';
			addch (' ');
			nb_ligne --;
			moveCursor((LINES/2)-(ymax/2) + nb_ligne, (COLS/2)-(xmax/2) + nb_col);
			compteur ++;
		}
	}
}

void fin()
{
	if(tab[nb_ligne][nb_col] == 'S')
	{
		stop = time(0);
		total_time = (stop - start);
		clear();
		refresh();
		while (key != 'o' && key != 'O' && key != KEY_F(2))
		{
			affich_fin();
		}
		if(key == 'o' || key == 'O')
		{
			if (niv < nb_map - 1)
			{
				tab_score(compteur);
				perso = 'O';
				compteur = 0;
				niv++;
				prog_princ();
			}
		}
		if (key == KEY_F(2))
		{
			tab_score(compteur);
			clear();
			refresh();
			menu_princ();
		}
	}
}

void affich_fin()
{
	char *succes = "Vous avez gagné !";
	char *mouv = "Score : %d";
	char *temps = "Temps : %d s";
	char *continuer = "Niveau Suivant ?";
	char *choose = "(o) Oui  /  (F2)  Menu";
	char *quit = "Appuyez sur F2 pour afficher le menu";
	if(niv >= nb_map - 1)
	{
		continuer = "Vous avez fini le jeu, bravo !";
		choose = "";
	}
	int taille = strlen(succes);
	int taille2 = strlen(quit);
	int taille3 = strlen(continuer);
	int taille4 = strlen(choose);
	int taille5 = strlen(mouv) + 3;
	int taille6 = strlen(temps);
	attron(A_BOLD);
	mvprintw(LINES/2 - 2, (COLS / 2) - (taille / 2), succes);
	attroff(A_BOLD);
	mvprintw(LINES/2 - 1, (COLS / 2) - (taille5 / 2), mouv, compteur);
	mvprintw(LINES/2, (COLS / 2) - (taille6 / 2), temps, total_time);
	mvprintw(LINES/2 + 1, (COLS / 2) - (taille3 / 2), continuer);
	mvprintw(LINES/2 + 2, (COLS / 2) - (taille4 / 2), choose);
	mvprintw(LINES - 1,(COLS / 2) - (taille2 / 2), quit);
	key = getch();
	refresh();
}

void choix_lvl(int choix)
{
	char *lv1 = "files/lv1.txt";
	char *lv2 = "files/lv2.txt";
	char *lv3 = "files/lv3.txt";
	char *lv4 = "files/lv4.txt";
	
	switch (choix)
		{
			case 0:
				choix_pseudo();
				laby = lv1;
			break;
			
			case 1:
				laby = lv2;
			break;
			
			case 2:
				laby = lv3;
			break;
			case 3:
				laby = lv4;
			break;
		}
}
void tab_score(int score)
{
	char *tab_score = "files/score.txt";
	FILE *fichier;  														// recupere le fichier ou est stocker le labyrinthe
    fichier=fopen(tab_score,"a");
   
    if(fichier == NULL)
        printf("Erreur lors de l'ouverture du fichier\n"); 					// si le fichier est introuvable, affiche une erreur
    else
    {
		fprintf(fichier,"%s\t%d\t%d\t%d\n", pseudo, niv + 1, score, total_time);
	}
    fclose(fichier);
}

void affich_score()
{
	int level, point, temps;
	int i = 1;
	char nom[50] = {'\0'};
	char *titre = "Pseudo | Niveau  | Score	 | Temps";
 char *afficher = "%s     | %d      | %d     | %d   ";
	int taille = strlen(titre);
	int taille2 = strlen(afficher) + 2;
	char *tab_score = "files/score.txt";
	FILE *fichier;
	fichier = fopen(tab_score,"r");
	mvprintw(LINES/2 , (COLS / 2) - (taille / 2), titre);
	while (!feof(fichier))  
	{
		fscanf(fichier,"%s\t%d\t%d\t%d\n", nom, &level, &point, &temps);
		mvprintw(LINES/2 + i, (COLS / 2) - (taille2 / 2), afficher, nom, level, point, temps);
		i++;
		
	}
    fclose(fichier);   
}

void save()
{
	char *save = "files/save.txt";
	FILE *fichier;  														// recupere le fichier ou est stocker le labyrinthe
    fichier=fopen(save,"w");
   
    if(fichier == NULL)
        printf("Erreur lors de l'ouverture du fichier\n"); 					// si le fichier est introuvable, affiche une erreur
    else
    {
		fprintf(fichier,"%d¤%d¤%d¤%d\n",niv, nb_ligne, nb_col, compteur);
	}
    fclose(fichier);
}

void reprendre()
{
	is_reload = 1;
	char *save = "files/save.txt";
	FILE *fichier;  														// recupere le fichier ou est stocker le labyrinthe
    fichier=fopen(save,"r");
   
    if(fichier == NULL)
        printf("Erreur lors de l'ouverture du fichier\n"); 					// si le fichier est introuvable, affiche une erreur
    else
    {
		fscanf(fichier,"%d¤%d¤%d¤%d\n",&niv, &xInit, &yInit, &compteur);
	}
    fclose(fichier);
}
	

void choix_pseudo()
{
	clear();
	refresh();
	char *choix = "Entrez votre pseudo (4 caractère max):";
	char *quit = "Appuyez sur F2 pour afficher le menu";
	int taille = strlen(choix);
	int taille2 = strlen(quit);
	attron(A_BOLD);
	mvprintw(LINES/2 , (COLS / 2) - (taille / 2), choix);
	attroff(A_BOLD);
	mvprintw(LINES - 1,(COLS / 2) - (taille2 / 2), quit);
	//getstr(pseudo);
	mvscanw(LINES/2 + 1, (COLS / 2) + taille, "%s", &pseudo);
	refresh();
}
	
void prog_princ()
{
	choix_lvl(niv);														// Choix de la map.
	initLab(laby);														// Initialise le Labyrinthe
	depLab();												// Deplacement dans le labyrinthe
}
