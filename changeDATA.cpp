#include "ChangeDATA.h"

extern HANDLE gSemaphore;

void changeDATA(SemaphoreDataHolder&holder)
{
	WaitForSingleObject(gSemaphore, INFINITE);
	for (int i = 0; i < holder.getCollection().size(); ++i)
	{
		holder.getCollection()[i].setFirstVal(holder.getMessage());
	}
	ReleaseSemaphore(gSemaphore, 2,NULL);
}