/****************************************************************************
 *
 * (c) Jean-Paul CARMONA, 1994-2014
 *
 * - Header pour JPTRIS -
 *
 ****************************************************************************/

#ifndef __JPTRIS__
	#define __TETRIS__

	typedef struct piece    {
		unsigned char matrice[4][4][4],
		n_mat,          /* matrice courante */
		x,y;			/* position grille */
	} TetrisPiece;

	static unsigned char level = 0;

	int  move_piece(int,int,int,GContext *ctx);	/* met matrice pc dans grille */
	void init_game(unsigned char, GContext *ctx);	/* initialise toutes les variables du jeux */


	/*------------------------------- def. de l'ecran */
	#define LARGEUR         144
	#define HAUTEUR         168

	/*------------------------------- def. de la grille de jeu */
	#define TAILLE_CUBE     6
	#define LARGEUR_GRILLE  12      /* 10 + 2 bords */
	#define HAUTEUR_GRILLE  21      /* 20 + 1 bord (bas) */

	#define GRILLE_X0       10
	#define GRILLE_Y0       10
	#define GRILLE_X1       (GRILLE_X0 + (LARGEUR_GRILLE * TAILLE_CUBE))
	#define GRILLE_Y1       (GRILLE_Y0 + (HAUTEUR_GRILLE * TAILLE_CUBE))

	/*------------------------------- repères */
	#define NEXT_X          (GRILLE_X1 + 20)
	#define NEXT_Y          GRILLE_Y0
	#define SCORE_X         NEXT_X
	#define SCORE_Y			90
	#define LEVEL_X         NEXT_X
	#define LEVEL_Y         115

	#define DEB_X           5       /* position de départ des pieces */
	#define DEB_Y           0
	#define DEB_MAT         0

	#define MIN_I           1       /* bornes des positions (i,j) */
	#define MIN_J           0
	#define MAX_I           (LARGEUR_GRILLE - 2)
	#define MAX_J           (HAUTEUR_GRILLE - 2)

	/*------------------------------- divers */
	#define MAX_TEMPO       20
	#define LEVEL_CHG       500
	#define LINE_PT         50
	#define PIECE_PT        10
	#define VICT_PT         2000
	#define NB_PIECE        5
	#define NB_EXTRA		10
	#define NB_LEVEL        1       /* suivant nb de vitesses possibles */
	#define NB_SCORE        5

	#define FALSE 		0
	#define TRUE		1

	/* move_piece values */
	#define FLOOR		0
	#define WALL		1
	#define MOVE		2
	#define USER_FAIL	-1


#endif
