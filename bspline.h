#ifndef __BSPLINE_H__
#define __BSPLINE_H__
#include "bspline.h"
// ÏñËØµÄ×ø±ê
struct point {
  int	x;				
  int	y;
};

void	bspline(int, int, point *, point *, int);
double	blend(int, int, int *, double);
void	compute_intervals(int *, int, int);
void	compute_point(int *, int, int, double, point *, point *);

#endif
