#include "AtomicBlockQueue.h"

void FileWrite(AtomicQueue<CommandBlock>& queue)
{
	while (true)
	{
		queue.pop().print();
	}
}