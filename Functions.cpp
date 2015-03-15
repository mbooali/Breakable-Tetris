#include "stdio.h"
#include "SDL.h"
#include "time.h"

#define X 16
#define Y 13

struct object
{
	int type;
	int matrix[3][3];
	int x, y;
	int status;
};
typedef struct object movable;

void cpy( movable*m1, movable m2){
	for ( int i = 0; i < 3 ; i++)
	{
		for ( int j = 0 ;j < 3 ; j++)
			m1->matrix[i][j] = m2.matrix[i][j];
	}
	m1->type = m2.type;
	m1->x = X / 2 - 1;
	m1->y = 0;
	m1->status = 1;
}

SDL_Surface *load_image( char *filename ) 
{
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image
    loadedImage = SDL_LoadBMP( filename );
    
    //If nothing went wrong in loading the image
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old image
        SDL_FreeSurface( loadedImage );
    }
    
    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Make a temporary rectangle to hold the offsets
     SDL_Rect offset;
    
	//Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;
    
    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

void rndgen( movable *temp )
{

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			temp->matrix[i][j]=0;
	
	temp->type = rand()%4;
	switch(temp->type)
	{
		case 0:
			temp->matrix[1][0]=(rand()%2)+1;
			temp->matrix[1][1]=(rand()%2)+1;
			temp->matrix[2][0]=(rand()%2)+1;
			temp->matrix[2][1]=(rand()%2)+1;
			break;
		case 1:
			temp->matrix[0][1]=(rand()%2)+1;
			temp->matrix[1][1]=(rand()%2)+1;
			temp->matrix[2][1]=(rand()%2)+1;
			break;
		case 2:
			temp->matrix[1][0]=(rand()%2)+1;
			temp->matrix[1][1]=(rand()%2)+1;
			temp->matrix[1][2]=(rand()%2)+1;
			temp->matrix[2][1]=(rand()%2)+1;
			break;
		case 3:
			temp->matrix[1][0]=(rand()%2)+1;
			temp->matrix[1][1]=(rand()%2)+1;
			temp->matrix[2][1]=(rand()%2)+1;
			temp->matrix[2][2]=(rand()%2)+1;
	}
	temp->x = 16 / 2 - 1;
	temp->y = 0;
	temp->status = 1;
}