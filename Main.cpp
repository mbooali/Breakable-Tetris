#include "stdio.h"
#include <iostream.h>
#include "SDL.h"
#include "time.h"
#include <conio.h>


#define X 16
#define Y 13

//The attributes of the screen
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

struct object
{
	int type;
	int matrix[3][3];
	int x, y;
	int status;
};
typedef struct object movable;

void cpy( movable*m1, movable m2);
SDL_Surface *load_image( char *filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
void rndgen( movable *temp );
void show (int map[Y][X] ,SDL_Surface* screen,movable m2, movable m3 );
int fall(int map[Y][X], movable *m);
void left(int map[Y][X], movable *m);
void right( int map[Y][X], movable *m );
void rot( int map[Y][X], movable *m );
void insert( int map[Y][X], movable m );
int remove( int map[Y][X]);

int main( int argc, char *args[] )
{
	//The surfaces that will be used
	SDL_Surface *background = NULL;
	SDL_Surface *screen = NULL;
	int map[Y][X]={0}, end,totaltime=0,score=0,p,bonus,delaytime=400,high,swip,totaltime2=0,n=0;
	char pname[16]={0},hname[16]={0};
	printf("please Enter your name\n\t");
	scanf("%s",pname);
	FILE *file;
	file = fopen("setting.txt","r");
	fscanf(file,"%s\n%d\n%d\n%d\n%d",hname,&high,&swip,&p,&bonus);
	fclose(file);
	//change random mermory
	srand(time(0));
	
	//Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;
    }
    
    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    
    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return 1;    
    }
    
    //Set the window caption
    SDL_WM_SetCaption( "Tetris", NULL );
    
    //Load the images
    background = load_image( "Background.bmp" );
    
    //Apply the background to the screen
    apply_surface( 0, 0, background, screen );
	SDL_Event event;
        
	movable m1, m2, m3;
//	map[7][7]=1;
//	map[8][9] = 2;

	rndgen(&m1);
	rndgen(&m2);
	rndgen(&m3);
    //Uint32 previousTime;
	insert(map,m1);
	bool done = false;
    while (!done)
    {
        //previousTime = SDL_GetTicks();

        // message processing loop
        do{
			if(SDL_PollEvent(&event) && m1.status )
		    {
				// check for messages
				switch (event.type)
				{
					// exit if the window is closed
					case SDL_QUIT:
						done = true;
						break;
					
					// check for keypresses
					case SDL_KEYDOWN:
						switch (event.key.keysym.sym)
						{
							// exit if ESCAPE is pressed
							case SDLK_ESCAPE:
								done = true;
								break;
							// moving the block
							case SDLK_LEFT:
								left(map,&m1);
								printf("maraz");
								break;
							case SDLK_RIGHT:
								right(map,&m1);
								printf("maraz");
								break;
							case SDLK_UP:
								rot(map,&m1);
								insert(map, m1);
								printf("maraz");
								break;
							case SDLK_DOWN:
								fall(map,&m1);
								printf("maraz");
								break;
						}
						break;
				} // end switch
			}
		show(map,screen,m2,m3);
		SDL_Delay(delaytime);
		totaltime+=delaytime;
		totaltime2+=delaytime;
		if(totaltime>60000)
		{
			delaytime-=30;
			totaltime=0;
		}


		end = fall(map,&m1);
		} while( end );
      	n=remove(map);
		score+=n*p+(n-1)*bonus;
		if(score>high)
		{
			high=score;
			strcpy(hname,pname);
		}
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		printf( " %s (highscore): %d\n%s\ntime:00:%d:%d\n",hname,high,pname,totaltime2/60,totaltime2%60);
		if(m1.y < 3)
			done = true;
	
		cpy(&m1, m2);
		cpy(&m2, m3);
		rndgen(&m3);
		insert(map, m1);
	}
	SDL_Surface *lose = load_image( "lose.bmp" );
		apply_surface( 0, 0, lose, screen );
	SDL_Flip( screen );
	SDL_Delay(12000);

	return 0;
}

void show (int map[Y][X] ,SDL_Surface* screen, movable m2, movable m3)
{
	SDL_Surface *red,*white,*background;
	//Load the images
	red = load_image( "esi1.bmp" );
    white = load_image( "esi.bmp" );
	background = load_image( "background.bmp" );

	//Apply the background to the screen
    apply_surface( 0, 0, background, screen );

	int i,j;
	for(i=3; i<13; i++)
	{
		for(j=0; j<16; j++)
		{
			if(map[i][j]==1)
				apply_surface( j*27+74, (i-3)*21+202, red, screen );
			else if(map[i][j]==2)
				apply_surface( j*27+74, (i-3)*21+202, white, screen );
		}
	}
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			if(m2.matrix[i][j]==1)
				apply_surface( j*27+54, (i-3)*21+100, red, screen );
			else if(m2.matrix[i][j]==2)
				apply_surface( j*27+54, (i-3)*21+100, white, screen );
		}
	}


	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			if(m3.matrix[i][j]==1)
				apply_surface( j*27+257, (i-3)*21+100, red, screen );
			else if(m3.matrix[i][j]==2)
				apply_surface( j*27+257, (i-3)*21+100, white, screen );
		}
	}

	SDL_Flip( screen );

	SDL_FreeSurface(red);
	SDL_FreeSurface(white);
	SDL_FreeSurface(background);
}

int fall(int map[Y][X], movable *m)
{
	int temp,chek,i,j,flag=0;
	for(i = 0; i < 3; i++)
	{
		chek = -5;
		for(j = 0; j < 3; j++)
		{
			if(m->matrix[j][i] != 0)
				chek = j;
		}
		if( ( chek != -5 && map[chek + m->y + 1][i+m->x] != 0 ) || m->y == Y - 3 )
		{
			m->status = 0;	
			break;
		}
	}
	if(m->status == 1)
		(m->y)++;
	
	for( i=Y-1; i>=1; i--)
	{
		for( j=0; j<X; j++)
		{
			if(map[i][j]==0 && map[i-1][j]!=0)
			{
				temp=map[i-1][j];
				map[i-1][j]=0;
				map[i][j]=temp;
				flag = 1;
			}
		}
	}
	return flag;
}


void left(int map[Y][X], movable *m)
{
	int i, j, chap, chek=1;
	for(i = 0; i < 3; i++)
	{
		chap = -5;
		for(j = 2; j > -1; j--)
		{
			if(m->matrix[i][j] != 0)
				chap = j;
		}
		if(chap != -5 && map[chap + m->y][j + m->x - 1] != 0 )
			chek=0;
	}
	if( chek )
	{
		for(j = 0; j < 3; j++)
		{
			for(i = 0; i < 3; i++)
			{
				if(m->matrix[j][i] != 0)
				{
					map[m->y + j][m->x + i - 1] = m->matrix[j][i];
					map[m->y + j][m->x + i] = 0;
				}
			}
		}
		m->x--;
	}
}


void right( int map[Y][X], movable *m )
{
	int i, j, rast, chek=1;
	for(i = 0; i < 3; i++)
	{
		rast = -5;
		for(j = 0; j < 3; j++)
		{
			if(m->matrix[i][j] != 0)
				rast = j;
		}
		if(rast != -5 && map[rast + m->y][j + m->x + 1] != 0 )
			chek=0;
	}
	if( chek )
	{
		for(j = 0; j < 3; j++)
		{
			for(i = 2; i > -1; i--)
			{
				if(m->matrix[j][i] != 0)
				{
					map[m->y + j][m->x + i + 1] = m->matrix[j][i];
					map[m->y + j][m->x + i] = 0;
				}
			}
		}
		m->x++;
	}
}

void rot( int map[Y][X], movable *m ){
	movable temp;
	int temp2[3][3]={0},i,j,temp1;
	temp.x = m->x;
	temp.y = m->y;
	if ( m->type ==0 )
	{
		temp1 = m->matrix[1][0];
		m->matrix[1][0] = m->matrix[2][0];
		m->matrix[2][0] = m->matrix[2][1];
		m->matrix[2][1] = m->matrix[1][1];
		m->matrix[1][1] = temp1;
	}
	else
	{
		for( i = 0; i < 3; i++ )
			for( j = 0; j < 3; j++ )
				temp2[2-j][i] = m->matrix[i][j];

		for( i = 0; i < 3; i++ )
			for( j = 0; j < 3; j++ )
				m->matrix[i][j] = temp2[i][j];
	}
}


void insert( int map[Y][X], movable m )
{
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			map[m.y+i][m.x+j]=m.matrix[i][j];
}
int remove( int map[Y][X])
{	
	int num=0,i,j,map1[Y][X]={0};
	
	for(i=12; i>=4; i-- )
	{
		for(j=0; j<16; j++)
		{
			if( (map[i][j]==1 && map[i-1][j]==1 && map[i][j+1]==1 && map[i-1][j+1]==1) || (map[i][j]==2 && map[i-1][j]==2 && map[i][j+1]==2 && map[i-1][j+1]==2))
			{
				map1[i][j]=map1[i-1][j]=map1[i][j+1]=map1[i-1][j+1]=1;
				map[i][j]=map[i-1][j]=map[i][j+1]=map[i-1][j+1]=0;
			}
		}
	}

	for(i=0; i<12; i++ )
	{
		for(j=0; j<16; j++)
		{
			if(map1[i][j]==1)
				num++;
		}
	}
	return num;
}