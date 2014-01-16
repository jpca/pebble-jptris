#include <pebble.h>
#include "jptris.h"

static Window *window;
static Layer *grid_layer;
/* next layout
static Layer *next_piece_layer;
static TextLayer *info_layer;
*/

/*========================== globales ===========================*/

static struct piece p[NB_EXTRA], pc, next; /* piece courante et suiv. */

static unsigned char grille[LARGEUR_GRILLE][HAUTEUR_GRILLE];

static long   	score,ascore,tempo;

static char    	nb_piece=NB_PIECE; //NB_EXTRA;

static GContext *shared_ctx = 0;

/************************ dŽfinition de pieces ***************************/

unsigned char barre[4][4][4]= { {       /* 1ere matrice de la piece 0 */
			{0,1,0,0},      /* 1ere ligne de la 1ere matrice */
			{0,1,0,0},      /* 2nde ... */
			{0,1,0,0},
			{0,1,0,0}       /* derniere ligne */
		       },
		       {                /* 2nde matrice */
		        {0,0,0,0},
		        {1,1,1,1},
		        {0,0,0,0},
		        {0,0,0,0}},
		       {
		        {0,1,0,0},
		        {0,1,0,0},
		        {0,1,0,0},
		        {0,1,0,0}}, /* 3eme */
		       {
		        {0,0,0,0},
		        {1,1,1,1},
		        {0,0,0,0},
		        {0,0,0,0}}  /* 4eme */
		      };

unsigned char cube[4][4][4]= { { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
			 { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
			 { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} },
			 { {0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }
		       };
	/* le T */
unsigned char mT[4][4][4]={ { {0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0} },
			{ {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} },
			{ {0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0} },
			{ {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }
		    };

	/* le L inverse */
unsigned char mL[4][4][4]={ { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
		      { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} },
		      { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} },
		      { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }
		    };

	/* le L */
unsigned char mL2[4][4][4]={ { {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} },
		       { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} },
		       { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} },
		       { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} }
		     };

/* nouvelles pieces */
unsigned char new_piece1[4][4][4]= { {      /* 1ere matrice de la piece 0 */
			{0,0,0,0},      /* 1ere ligne de la 1ere matrice */
			{0,1,1,0},      /* 2nde ... */
			{0,1,0,0},
			{0,0,0,0}       /* derniere ligne */
		       },{
			{0,0,0,0},
			{0,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		       },{
			{0,0,0,0},
			{0,0,1,0},
			{0,1,1,0},
			{0,0,0,0}
		       },{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,0,0,0}}
		       };
/**************/
unsigned char new_piece2[4][4][4]= { {      /* 1ere matrice de la piece 0 */
			{1,0,0,0},      /* 1ere ligne de la 1ere matrice */
			{1,1,1,0},      /* 2nde ... */
			{0,0,1,0},
			{0,0,0,0}       /* derniere ligne */
		       },{
			{0,1,1,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		       },{
			{1,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		       },{
			{0,1,1,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}}
		       };

/**************/
unsigned char new_piece3[4][4][4]= { {      /* 1ere matrice de la piece 0 */
			{1,0,0,0},      /* 1ere ligne de la 1ere matrice */
			{0,1,0,0},      /* 2nde ... */
			{0,0,1,0},
			{0,0,0,0}       /* derniere ligne */
		       },{
			{0,0,1,0},
			{0,1,0,0},
			{1,0,0,0},
			{0,0,0,0}
		       },{
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,0}
		       },{
			{0,0,1,0},
			{0,1,0,0},
			{1,0,0,0},
			{0,0,0,0}}
		       };

/**************/
unsigned char new_piece4[4][4][4]= { {      /* 1ere matrice de la piece 0 */
			{0,0,1,0},      /* 1ere ligne de la 1ere matrice */
			{1,1,1,0},      /* 2nde ... */
			{1,0,0,0},
			{0,0,0,0}       /* derniere ligne */
		       },{
			{1,1,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		       },{
			{0,0,1,0},
			{1,1,1,0},
			{1,0,0,0},
			{0,0,0,0}
		       },{
			{1,1,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,0,0,0}}
		       };

/**************/
unsigned char new_piece5[4][4][4]= { {      /* 1ere matrice de la piece 0 */
			{0,1,1,0},      /* 1ere ligne de la 1ere matrice */
			{0,1,0,0},      /* 2nde ... */
			{0,1,1,0},
			{0,0,0,0}       /* derniere ligne */
		       },{
			{0,0,0,0},
			{1,1,1,0},
			{1,0,1,0},
			{0,0,0,0}
		       },{
			{1,1,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		       },{
			{1,0,1,0},
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}}
		       };


/************************ definition de grilles *************************/
/*
//! Represents "clear" or transparent.
GColorClear = ~0,
//! Represents black.
GColorBlack = 0,
//! Represents white.
GColorWhite = 1,
*/
unsigned char grille_init[NB_LEVEL][HAUTEUR_GRILLE][LARGEUR_GRILLE]={
	{/* 1er niveau */
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1}
	}
/* placer ici les niveaux suivants */
};

/*======================= Déclaration des fonctions =========================*/

void display_info(GContext *ctx);        			/* informe des scores et du niveau */
void display_grid();    								/* mise ˆ jour de la grille */
void display_next(GContext *ctx);       			/* affiche la piece qui suivra */
int random(int max);								/* retourne un nombre alŽatoire entre 0 et max */
void efface_ligne();      							/* quand une ligne et pleine */
void rectfill(int,int,int,int,int,GContext *ctx);	/* trace un carrŽ de la couleur courante */
void display_end(GContext *ctx);					/* informe de la fin de jeu */

static AppTimer *timer;
static int game_init_done=FALSE;
static int end_game = FALSE;

static void grid_layer_update_callback(Layer *me, GContext *ctx) {
	
	if(game_init_done == FALSE)
	{
		shared_ctx = ctx;
		init_game(0,ctx);
		game_init_done=TRUE;
	}

	// fait descendre la piece d'un case au lieu d'un pixel, pas de tempo par niveau possible :-(
	if(move_piece(0,0,1,ctx) == USER_FAIL)
	{
		end_game = TRUE;
		display_end(ctx);
		display_info(ctx);
		return;
	}

	display_grid(ctx);
	display_info(ctx);
	display_next(ctx);
}

static void timer_callback(void *data) {
	
	int next_call = 900-(level*100); /* le temps raccourci avec le niveau, millisecondes */
	if(next_call < 100)
		next_call = 100; // plafond

	layer_mark_dirty(grid_layer); // appel grid_layer_update_callback()
	if(end_game == FALSE)
		timer = app_timer_register(next_call, timer_callback, NULL);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *ctx) {
	// deplace la piece ˆ gauche
	move_piece(0,-1,0,shared_ctx);
	layer_mark_dirty(grid_layer);
}
static void select_click_handler(ClickRecognizerRef recognizer, void *ctx) {
	// fait tourner la piece
	move_piece(1,0,0,shared_ctx);
	layer_mark_dirty(grid_layer);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *ctx) {
	// deplace la piece ˆ droite
	move_piece(0,1,0,shared_ctx);
	layer_mark_dirty(grid_layer);
}

static void click_config_provider(void *ctx) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_frame = layer_get_frame(window_layer);
  grid_layer = layer_create(window_frame);
  layer_set_update_proc(grid_layer, grid_layer_update_callback);
  layer_add_child(window_layer, grid_layer);
}

static void window_unload(Window *window) {
  layer_destroy(grid_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  window_set_background_color(window, GColorBlack);

  srand(time(NULL)); // initialise le gŽnŽrateur d'alŽa
  timer = app_timer_register(1000 /* milliseconds */, timer_callback, NULL);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop(); // Pass execution to Pebble OS
  deinit();
}

/* deplace une piece, pour q'une nouvelle piece soit en position de dŽpart donner rot=tx=ty=0) */
int move_piece(int rot, int tx, int ty, GContext *ctx) {
	unsigned int   	i,j,             /* parcours de grille */
					x,y,                    /* coord. calcul‚ */
					nv_mat, nv_x, nv_y,     /* nouvelle position */
					mvt = MOVE;             /* mouvement: 0=bloque,1=bords,2=ok */

	char    get_next = FALSE;       /* prise "next" */

	//APP_LOG(APP_LOG_LEVEL_DEBUG, "execute method: move_piece(%i,%i,%i,%p)", rot, tx, ty, ctx);
	
	/*** initialisation d'une nouvelle piece (ancien next) ***/
	if((rot == 0) && (tx == 0) && (ty == 0)) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "nouvelle piece");
		pc.x = DEB_X;  /* 1ere position */
		pc.y = DEB_Y;
		pc.n_mat = DEB_MAT;
		return mvt;
	}

	nv_mat = pc.n_mat + rot;
	if(nv_mat >= 4)
		nv_mat = 0;
	nv_x = pc.x + tx;
	nv_y = pc.y + ty;

	//APP_LOG(APP_LOG_LEVEL_DEBUG, "futur emplacement pc: x=%i, y=%i, rot=%i)", nv_x, nv_y, nv_mat);

	/*** test de validite du mouvement ***/

	for(i = 0 ; (i<4) && (mvt == MOVE) ; i++)
	{
		for(j = 0 ; (j<4) && (mvt == MOVE); j++)
		{
			if(((nv_x+i < MIN_I) || (nv_x+i > MAX_I)) &&
			   (pc.matrice[nv_mat][i][j] != 0))
				mvt = WALL;        /* bord touche */
			else if((pc.matrice[nv_mat][i][j] != 0) &&
				(grille[nv_x+i][nv_y+j] != 0))
				mvt = FLOOR;        /* bas occupe */
		}
	}
	
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "mvt=%i",mvt);

	if(mvt == MOVE)		/* deplace la piece */
	{
		/* efface l'ancienne position de la piece inutile le make_dirty peeble efface tout
		x = (GRILLE_X0 + (pc.x * TAILLE_CUBE));

		for(i = 0 ; i<4 ; i++)
		{
			y = (GRILLE_Y0 + (pc.y * TAILLE_CUBE));

			for(j = 0 ; j<4 ; j++)
			{
				if(pc.matrice[pc.n_mat][i][j] != 0)
					rectfill(x+1, y+1, x+TAILLE_CUBE-1, y+TAILLE_CUBE-1, GColorBlack, ctx);

				y += TAILLE_CUBE;
			}
			x += TAILLE_CUBE;
		}
		*/

		pc.n_mat = nv_mat;     /* mise a jour de la piece courante */
		pc.x = nv_x;
		pc.y = nv_y;

		/* affiche la piece a sa nouvelle position */
		x = (GRILLE_X0 + ((pc.x+1) * TAILLE_CUBE));

		for(i = 0 ; i<4 ; i++)
		{
			y = (GRILLE_Y0 + (pc.y * TAILLE_CUBE));

			for(j = 0 ; j<4 ; j++)
			{
				rectfill(x+1,y+1,x+TAILLE_CUBE-1,y+TAILLE_CUBE-1,pc.matrice[pc.n_mat][i][j],ctx); // 0 dans la matrice = 0 dans la grille
				y += TAILLE_CUBE;
			}
			x += TAILLE_CUBE;
		}

		//APP_LOG(APP_LOG_LEVEL_DEBUG, "reaffichage piece ok");
	}
	else if(mvt == FLOOR) /* si piece courante bloquee */
	{
		/*** test de prise de la piece suivante ***/
		for(i = 0 ; (i<4) && (get_next == FALSE) ; i++)
		{
			for(j = 3 ; (j>0) && (get_next == FALSE) ; j--)
			{
				if((pc.matrice[pc.n_mat][i][j] != 0) &&
				   (grille[pc.x+i][pc.y+j+1] != 0))
					get_next = TRUE;
			}
		}

		if(get_next == TRUE)    /* prise de la piece suivante */
		{
			if(pc.y == DEB_Y)
				return USER_FAIL;

			/*** remplis la grille avec pc ***/
			for( i = 0 ; i<4 ; i++) {
				for(j = 0 ; j<4 ; j++) {
					if(pc.matrice[pc.n_mat][i][j] != 0)
						grille[pc.x+i][pc.y+j] = pc.matrice[pc.n_mat][i][j];
				}
			}
			efface_ligne();

			/*** gestion du score ***/
			score += PIECE_PT;
			if(score > 200)
				nb_piece=NB_EXTRA; // pieces supplementaires a partir de 200 pts
			display_info(ctx);

			/*** prise du "next" ***/
			pc = next;
			next = p[random(nb_piece)];
			move_piece(0,0,0,ctx);
		}
	}

	return mvt;
}

/* genere un nombre aleatoire entre 0 et max */
int random(int max) {
	return rand() % max;
}

/* efface chaque ligne complete */
void efface_ligne() {
	
	unsigned int   i,j,k,nb_ligne=0;
	char    efface = FALSE;

	//APP_LOG(APP_LOG_LEVEL_DEBUG, "execute method: efface_ligne");

	/*** cherche ligne ˆ effacer ***/
	for(j = pc.y; j <= MAX_J ; j++)
	{
		efface = TRUE;

		for(i = 0 ; (i <= MAX_I) && (efface == TRUE) ; i++)
		{
			if(grille[i][j] == 0)
				efface = FALSE;
		}

		if(efface == TRUE)
		{
			nb_ligne++;
			vibes_short_pulse();
			for(k = j ; k > MIN_J ; k--)
				for(i = MIN_I ; i <= MAX_I ; i++)
					grille[i][k] = grille[i][k-1];
		}
	}

	/* gestion du score */
	if(nb_ligne > 0)
	{
		score += (nb_ligne * nb_ligne * LINE_PT);

		if(score >= ascore+LEVEL_CHG)
		{
			level++;
			ascore += LEVEL_CHG;
		}

		/* rajout de lignes ˆ l'adversaire dans une prochaine version :-)
		if(nb_ligne >= 2)
		{
			if(nb_ligne < 4)        / * 2 = 1l, 3 = 2l, 4 = 4l * /
				nb_ligne--;
			
			TODO : envoi de nb ligne à l'autre joueur
		}
		*/
	}
}

void display_next(GContext *ctx) {
	unsigned int i,j,x,y,color;

	x = NEXT_X;

	for(i = 0 ; i<4 ; i++)
	{
		y = NEXT_Y;
		for(j = 0 ; j<4 ; j++)
		{
			color = next.matrice[0][i][j];
			rectfill(x+1,y+1,x+TAILLE_CUBE-1,y+TAILLE_CUBE-1,color,ctx);
			y += TAILLE_CUBE;
		}
		x += TAILLE_CUBE;
	}
}

void display_grid(GContext *ctx) {
	unsigned char i,j;
	unsigned int x,y=GRILLE_Y0,color;

	//APP_LOG(APP_LOG_LEVEL_DEBUG, "execute method: display_grid");

	for(j = 0 ; j<HAUTEUR_GRILLE ; j++)         /* haut -> bas */
	{
		x = GRILLE_X0 + TAILLE_CUBE;
		for(i = 0 ; i<LARGEUR_GRILLE ; i++) /* gauche -> droite */
		{
			color = grille[i][j];
			//APP_LOG(APP_LOG_LEVEL_DEBUG, "grille[%i,%i]=%i)", i, j, color);
			if(color != 0)
				rectfill(x+1,y+1,x+TAILLE_CUBE-1,y+TAILLE_CUBE-1,color,ctx);

			x += TAILLE_CUBE;
		}
		y += TAILLE_CUBE;
	}
}

void init_game(unsigned char niv, GContext *ctx) {
	unsigned char i,j,k;

	//APP_LOG(APP_LOG_LEVEL_DEBUG, "execute method: init_game");

	/*** initialisation des grilles ***/

	for(i = 0 ; i < LARGEUR_GRILLE ; i++)               /* intŽrieur */
		for(j = 0 ; j < HAUTEUR_GRILLE ; j++) {
			grille[i][j] = grille_init[niv][j][i];
		}

	/*** init des pieces ***/
	for(i=0 ; i<4 ; i++)
		for(j=0 ; j<4 ; j++)
			for(k=0 ; k<4 ; k++) {
				/* barre */             p[0].matrice[i][j][k] = barre[i][j][k];
				/* cube */              p[1].matrice[i][j][k] = cube[i][j][k];
				/* T */             	p[2].matrice[i][j][k] = mT[i][j][k];
				/* L */                 p[3].matrice[i][j][k] = mL[i][j][k];
				/* L ˆ l'envers */    	p[4].matrice[i][j][k] = mL2[i][j][k];


				/* nouvelles pieces */
				p[5].matrice[i][j][k] = new_piece1[i][j][k];
				p[6].matrice[i][j][k] = new_piece2[i][j][k];
				p[7].matrice[i][j][k] = new_piece3[i][j][k];
				p[8].matrice[i][j][k] = new_piece4[i][j][k];
				p[9].matrice[i][j][k] = new_piece5[i][j][k];

			}

	/*** var. joueurs ***/
	tempo = 0;
	level = 0;
	score = 0;
	ascore = 0;

	/*** dŽmarrage ***/
	pc = p[random(nb_piece)];
	next = p[random(nb_piece)];
	move_piece(0,0,0,ctx);

	/* debug
	move_piece(0,-1,0,ctx);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "0 -1 ok");

	move_piece(0,1,0,ctx);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "0 1 ok");

	move_piece(1,0,0,ctx);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "1 0 ok");
	*/
}

void display_info(GContext *ctx) {
	char *text="Score 99999999";

	graphics_context_set_text_color(ctx, GColorWhite);
	graphics_draw_text(	ctx,
						"Score",
						fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
						GRect(SCORE_X,SCORE_Y-15,50,20),
						GTextOverflowModeTrailingEllipsis,
						GTextAlignmentLeft,
						NULL);

	//snprintf(text,strlen(text),"Scr %.5ld",score);
	snprintf(text,strlen(text),"%.5ld",score);
	graphics_context_set_text_color(ctx, GColorWhite);
	graphics_draw_text(	ctx,
						text,
						fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
						GRect(SCORE_X,SCORE_Y,50,20),
						GTextOverflowModeTrailingEllipsis,
						GTextAlignmentLeft,
						NULL);

	if(level>0)
		snprintf(text,strlen(text),"Good!");
	else if(level >1)
		snprintf(text,strlen(text),"Extra!");
	else
		snprintf(text,strlen(text)," ");
	graphics_draw_text(	ctx,
						text,
						fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
						GRect(LEVEL_X,LEVEL_Y,50,20),
						GTextOverflowModeTrailingEllipsis,
						GTextAlignmentLeft,
						NULL);
}

void rectfill(int x1, int y1, int x2, int y2, int color, GContext *ctx) {

	//APP_LOG(APP_LOG_LEVEL_DEBUG, "rectfill(%i,%i,%i,%i,%i)", x1, y1, x2, y2, color);

	graphics_context_set_fill_color(ctx, color);
	int w = x2 -x1;
	int h = y2 - y1;
	graphics_fill_rect(ctx,GRect(x1,y1,w,h),1,GCornersAll);
}

void display_end(GContext *ctx) {
	char *text="Game Over";

	graphics_context_set_text_color(ctx, GColorWhite);
	graphics_draw_text(	ctx,
						text,
						fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
						GRect(20,35,100,40),
						GTextOverflowModeTrailingEllipsis,
						GTextAlignmentCenter,
						NULL);
}
