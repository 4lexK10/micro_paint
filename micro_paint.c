#include <stdio.h>
#include <string.h>
#include <math.h> 
#include <stdlib.h>
#include <unistd.h>

typedef struct s_shape
{
	char	c;
	float	X;
	float	Y;
	float	r;
	char	color;
}	t_shape;

typedef struct s_zone
{
	int	W;
	int	H;
	char	color;
}	t_zone;


static void	error(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		write(1, &str[i], 1);
	write(1, "\n", 1);
}

static void	f_free(char **ptr)
{
	if (!ptr || !(*ptr))
		return ;
	free(*ptr);
	*ptr = NULL;
}

static	int	in_circle(int x, int y, t_shape circle)
{
	float distance;

	distance = (sqrtf((x - circle.X) * (x - circle.X) + (y - circle.Y) * (y - circle.Y))); 
	if (distance < circle.r + 1.0000000)
	{
		if (distance >= circle.r)
			return (2);
		return (1);
	}
}

static void	fill_drawing(char *drawing, t_shape circle, t_zone zone)
{
	int	x;
	int	y;
	int	ret;

	y = -1;
	while (++y < zone.H)
	{
		x = -1;
		while (++x < zone.W)
		{
			ret = in_circle(x, y, circle);
			if ((ret == 2 && circle.c == 'c') || (ret && circle.c == 'C'))
				drawing[zone.W * y + x];
		}
	}
}

static int	get_drawing(FILE *file, char *drawing, t_zone zone)
{
	t_shape	circle;

	memset(&circle, 0, sizeof(t_shape));
	while (fscanf(file, "%c %f %f %f %c\n", &circle.c, &circle.X, &circle.Y, &circle.r, &circle.color) == 5)
	{
		if (circle.c != 'c' && circle.c != 'C')
			return (1);
		fill_drawing(drawing, circle, zone);
	}
	return (0);
}

static void	draw_shape(char *drawing, t_zone zone)
{
	int	x;	
	int	y;

	y = -1;
	while (++y < zone.H)
	{
		x = -1;
		while (++x < zone.W)
			write(1, &drawing[zone.W * y + x], 1);
		write(1, "\n", 1);
	}
}

int main(int ac, char **av)
{
	FILE	*file;
	t_zone	zone;
	char	*drawing;

	if (ac != 2)
		return (error("Error: argument"), 1);
	memset(&zone, 0, sizeof(t_zone));
	if ((file = (fopen(av[1], "r"))) == NULL)
		return (error("Error: Operation file corrupted"), 1);
	if (fscanf(file, "%d %d %c\n", &zone.W, &zone.H, &zone.color) != 3)
		return (error("Error: Operation file corrupted"), 1);
	if (zone.W <= 0 || zone.W > 300 || zone.H <= 0 || zone.H > 300)
		return (error("Error: Operation file corrupted"), 1);
	drawing = (char *)malloc(sizeof(char) * zone.W * zone.H);
	if (!drawing)
		return (1);
	if (get_drawing(file, drawing, zone))
		return (f_free(&drawing), error("Error: Operation file corrupted"), 1);
	draw_shape(drawing, zone);
	f_free(&drawing);
	return (0);
}



