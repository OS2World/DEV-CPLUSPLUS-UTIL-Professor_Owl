#define     NULL    0
#include    <os2.h>
//
#include	<io.h>
#include    "setupdat.h"
#include    "warnbeep.h"


extern SETUPDATA    UserSetup;

//
// Make Warning Beep If Option Is Enabled
//
void WarnBeep(void)
{
	if(UserSetup.WarnBeep)
	{
		DosBeep(750,125);
	}
}


