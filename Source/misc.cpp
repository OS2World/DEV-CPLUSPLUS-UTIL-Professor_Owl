#include	<owl\point.h>


int PointInRect(TPoint *p, TRect *r)
{
	if(p->x >= r->left && p->x <= r->right
		&& p->y >= r->top && p->y <= r->bottom)
	{
		return(TRUE);
	}
	return(FALSE);
}

