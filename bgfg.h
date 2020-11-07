#ifndef __BGFG_H
#define __BGFG_H

void bg(int jobnum, jobs background[], int *Cstatus);
void fg(int jobnum, jobs background[], jobs *fgdetails, int *Cstatus);

#endif