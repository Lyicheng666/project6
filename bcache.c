// /*
// This is the implementation of the buffer cache module.
// You should make all of your changes here.
// Feel free to add any structures, types or helper functions that you need.
// */

// #include "bcache.h"
// #include "disk.h"

// #include <stdlib.h>
// #include <stdio.h>
// #include <pthread.h>
// #include <string.h>

// #define SIZE 4096
// pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t io_lock=PTHREAD_MUTEX_INITIALIZER;

// enum block_state
// {
// 	EMPTY,	  // Block is free
// 	WANTREAD, // Block is needed, read requested
// 	READING,  // Block is currently being read
// 	READY,	  // Block is ready for use
// 	DIRTY,	  // Block has been modified
// 	WRITING	  // Block is currently being written
// };
// /*
// A structure describing the entire buffer cache.
// You may add and modify this structure as needed.
// */
// struct block
// {
// 	int blocknum;
// 	enum block_state state;
// 	char data[SIZE];
// 	pthread_cond_t cond ;
// } ;
// struct bcache
// {
// 	struct disk *disk;    // The disk object underlying the cache.
// 	struct block *blocks;
// 	int memory_blocks;    // The total number of memory blocks in the cache.
// 	int nreads;           // A running count of read operations.
// 	int nwrites;          // A running count of write operations.
// };

// /*
// Create and initialize the buffer cache object.
// You may modify this function as needed to set things up.
// */
// // const char *getStateName(block_state state)
// // {
// // 	switch (state)
// // 	{
// // 	case EMPTY:
// // 		return "EMPTY";
// // 	case WANTREAD:
// // 		return "WANTREAD";
// // 	case READING:
// // 		return "READING";
// // 	case READY:
// // 		return "READY";
// // 	case DIRTY:
// // 		return "DIRTY";
// // 	case WRITING:
// // 		return "WRITING";
// // 	default:
// // 		return "UNKNOWN";
// // 	}
// // }

// struct block* find_victim_block_nostoring(struct bcache*bc,int new_num)
// {
// 	pthread_mutex_lock(&lock);
// 	struct block *victim = NULL;
// 	while (1)
// 	{
// 		int idx = rand() % bc->memory_blocks;
// 		if (bc->blocks[idx].state == READY)
// 		{
// 			victim = &bc->blocks[idx];
// 			printf("block %d is the victim\n", victim->blocknum);
// 			// victim->state=DIRTY;
// 			break;
// 		}
// 	}
// 	if (!victim)
// 	{
// 		pthread_mutex_unlock(&lock);
// 		return NULL;
// 	}
// 	else
// 	{
// 		// victim->state = DIRTY;
// 		// pthread_cond_broadcast(&victim->cond);
// 		// while (victim->state != READY)
// 		// {
// 		// 	pthread_cond_wait(&victim->cond, &lock);
// 		// }
// 		// printf("storing of the dirty block has finish\n");
// 		// after storing victim's info to disk,we proceed
// 		// load info to new block
// 		memset(victim->data, 0, SIZE);
// 		victim->blocknum = new_num;
// 		victim->state = WANTREAD;
// 		pthread_cond_broadcast(&victim->cond);
// 		pthread_mutex_unlock(&lock);
// 		return victim;
// 	}
// }

// struct block* find_victim_block(struct bcache*bc,int new_num)//the function for finding a victim block 
// {
// 	//we might want to store the victim's data on disk here
// 	pthread_mutex_lock(&lock);
// 	struct block*victim=NULL;
// 	while(1)
// 	{
// 		int idx=rand()%bc->memory_blocks;
// 		if(bc->blocks[idx].state==READY)
// 		{
// 			victim=&bc->blocks[idx];
// 			printf("block %d is the victim\n",victim->blocknum);
// 			//victim->state=DIRTY;
// 			break;
// 		}
// 	}
// 	if(!victim)
// 	{
// 		pthread_mutex_unlock(&lock);
// 		return NULL;
// 	}
// 	else
// 	{
// 		victim->state=DIRTY;
// 		pthread_cond_broadcast(&victim->cond);
// 		while(victim->state!=READY)
// 		{
// 			pthread_cond_wait(&victim->cond,&lock);
// 		}
// 		printf("storing of the dirty block has finish\n");
// 		//after storing victim's info to disk,we proceed
// 		//load info to new block
// 		 memset(victim->data, 0, SIZE);
// 		victim->blocknum=new_num;		
// 		victim->state=WANTREAD;
// 		pthread_cond_broadcast(&victim->cond);
// 		pthread_mutex_unlock(&lock);
// 		return victim;
// 	}
	
// }

// struct bcache * bcache_create( struct disk *d, int memory_blocks )
// {
// 	struct bcache *bc = malloc(sizeof(*bc));
// 	bc->disk = d;
// 	bc->nreads = 0;
// 	bc->nwrites = 0;
// 	bc->memory_blocks = memory_blocks;
// 	bc->blocks = malloc(sizeof(struct block) * memory_blocks);
// 	for(int i=0;i<memory_blocks;i++)
// 	{
// 		bc->blocks[i].blocknum = -1; // Indicates block is EMPTY
// 		bc->blocks[i].state = EMPTY;
// 		pthread_cond_init(&bc->blocks[i].cond, NULL);
// 		//bc->blocks[i].cond=PTHREAD_COND_INITIALIZER;
// 	}
// 	return bc;
// }

// /*
// Read a block through the buffer cache.
// This is a dummy implementation that calls disk_read directly.
// It will work only for one thread, and it won't be particularly fast.
// Instead, it should manipulate the data structure and wait for the scheduler.
// */
// //each block has its own cond?
// //I/O scheduler is a background of multiple threads
// void bcache_read( struct bcache *bc, int blocknum, char *data )
// {
// 	printf("the program wants to read block%d\n",blocknum);
// 	pthread_mutex_lock(&lock);	
// 	struct block*ptr=NULL;
// 	for(int i=0;i<bc->memory_blocks;i++)
// 	{
// 		if(bc->blocks[i].blocknum==blocknum)//if block is in cache,just read it?
// 		{
// 			// while (bc->blocks[i].state!=FREE)
// 			// {
// 			// 	pthread_cond_wait(&bc->blocks[i].cond, &lock);//wait till it is available
// 			// }
// 			// bc->blocks[i].state=WANTREAD;
// 			// break;
// 			// memcpy(data, ptr->data, SIZE);
// 			printf("block %d is in cache,we read it\n", blocknum);
// 			ptr = &bc->blocks[i];
// 			if(ptr->state==READY)//if ready,just read data from cache block
// 			{
// 				memcpy(data, ptr->data, SIZE);
// 				bc->nreads++;
// 				pthread_mutex_unlock(&lock);
// 				return;
// 			}
// 			else//if not,read data from disk
// 			{
// 				ptr->state=WANTREAD;
// 				pthread_cond_broadcast(&ptr->cond);
// 				while(ptr->state!=READY)
// 				{
// 					pthread_cond_wait(&ptr->cond, &lock);//wait till it is available					
// 				}
// 				memcpy(data, ptr->data, SIZE);
// 				bc->nreads++;
// 				pthread_mutex_unlock(&lock);
// 				return;
// 			}
// 			// while(bc->blocks[i].state!=READY)
// 			// {
// 			// 	pthread_cond_wait(&bc->blocks[i].cond, &lock);//wait till it is available
// 			// }
// 			// pthread_cond_broadcast(&bc->blocks[i].cond);			
// 			// // data=ptr->data;
// 			// memcpy(data, ptr->data, SIZE);
// 			// bc->nreads++;
// 			// pthread_mutex_unlock(&lock);
// 			// return;			
// 		}
// 	}	
// 	printf("block %d is not in cache,placing it in cache\n",blocknum);
// 	if(!ptr)//block is not in cache,so put it in cache and read its data from disk
// 	{
// 		for (int i = 0; i < bc->memory_blocks; i++)
// 		{
// 			if (bc->blocks[i].state == EMPTY)//pick a empty block,use disk_read to read data
// 			{
// 				//memcpy(data, bc->blocks[i].data, sizeof(bc->blocks[i].data));
// 				// ptr = &bc->blocks[i];
// 				// bc->blocks[i].blocknum=blocknum;
// 				// bc->blocks[i].state = WANTREAD;				
// 				// //what about copying condition var?
				
// 				// pthread_cond_signal(&ptr->cond);
// 				// printf("block %d put in cache array %d\n", blocknum, i);
// 				// break;
// 				ptr=&bc->blocks[i];
// 				ptr->blocknum=blocknum;
// 				ptr->state=WANTREAD;
// 				pthread_cond_broadcast(&ptr->cond);
// 				//wait for io handler read from disk
// 				while (ptr->state != READY)
// 				{
// 					pthread_cond_wait(&ptr->cond, &lock); // wait till it is available
// 				}
// 				memcpy(data, ptr->data, SIZE);
// 				bc->nreads++;
// 				pthread_mutex_unlock(&lock);
// 				return;
// 			}
// 		}
// 	}
// 	if(!ptr)//we cannot find a block that is empty,then we should find a victim
// 	{		
// 		pthread_mutex_unlock(&lock);
// 		ptr=find_victim_block(bc,blocknum);
// 		//before we evict a victim,we should store its info back to disk
// 		//consider what we should do after finding a victim
// 		if(ptr)//found a victim
// 		{
// 			pthread_mutex_lock(&lock);
// 			// memset(ptr->data, 0, SIZE);
// 			// ptr->blocknum=blocknum;		
// 			// ptr->state=WANTREAD;
// 			while (ptr->state != READY)
// 			{
// 				pthread_cond_wait(&ptr->cond, &lock); // wait till it is available
// 			}
// 			//pthread_cond_broadcast(&ptr->cond);
// 			// data=ptr->data;
// 			memcpy(data, ptr->data, SIZE);
// 			bc->nreads++;
// 			pthread_mutex_unlock(&lock);
// 			return;
// 		}
// 		else//fails to find victim
// 		{
// 			printf("block not found\n");
// 			return;
// 		}
		
// 	}			
// 	/* count the number of reads as we go */	
// }
// /*
// Write a block through the buffer cache.
// Like bcache_read, this is a dummy implementation.
// */
// void bcache_write( struct bcache *bc, int blocknum, const char *data )
// {
// 	//case 1:writing to a block that is not present in cache, we probably need to create a new block and write data into it
// 	//case 2:writing to a block that exists,we want to check if it is READY or DIRTY
// 	//if is dirty,we need to store its data and then write
// 	printf("program wants to write to block%d\n",blocknum);
// 	pthread_mutex_lock(&lock);
// 	struct block*ptr=NULL;
// 	for(int i=0;i<bc->memory_blocks;i++)
// 	{
// 		if(bc->blocks[i].blocknum==blocknum)//find it and check if it is free
// 		{
// 			printf("writing to block%d in block array %d\n",blocknum,i);
// 			while (bc->blocks[i].state!=READY)
// 			{
// 				pthread_cond_wait(&bc->blocks[i].cond, &lock);//wait till it is available
// 			}
// 			bc->blocks[i].state=DIRTY;
// 			ptr = &bc->blocks[i];
// 			bc->nwrites++;
// 			break;
// 		}
// 	}
// 	if(!ptr)//block does not exist,try to find a free one
// 	{
// 		for (int i = 0; i < bc->memory_blocks; i++)
// 		{
// 			if (bc->blocks[i].state ==EMPTY)
// 			{
// 				printf("putting block %d in char array %d\n",blocknum,i);
// 				ptr = &bc->blocks[i];
// 				strncpy(ptr->data, data, sizeof(ptr->data) - 1);
// 				ptr->data[sizeof(ptr->data) - 1] = '\0';
// 				ptr->blocknum = blocknum;
// 				ptr->state = DIRTY;
// 				pthread_cond_signal(&ptr->cond);
// 				bc->nwrites++;
// 				pthread_mutex_unlock(&lock);
// 				return;				
// 			}
// 		}
// 	}
// 	if(!ptr)
// 	{
// 		pthread_mutex_unlock(&lock);
// 		ptr=find_victim_block(bc,blocknum);
// 		pthread_mutex_lock(&lock);
// 	}
// 	if(ptr)
// 	{
// 		//memcpy(bc->blocks[i].data,data,sizeof(data));
// 		strncpy(ptr->data,data,sizeof(ptr->data)-1);
// 		ptr->data[sizeof(ptr->data)-1]='\0';
// 		ptr->blocknum=blocknum;
// 		ptr->state = DIRTY;
// 		pthread_cond_signal(&ptr->cond);
// 		bc->nwrites++;
// 		pthread_mutex_unlock(&lock);
// 		return;
// 	}
// 	else
// 	{
// 		printf("victim block not found\n");
// 		pthread_mutex_unlock(&lock);
// 		return;
// 	}
// 	//memcpy(ptr->data, data, sizeof(data));//write data to the block
// 	//ptr->data=data;//return immediately
// 	// memcpy(data, bc->blocks[i].data, sizeof(bc->blocks[i].data));

// 	/* Just do the write.  Unsafe! */
// 	// disk_write(bc->disk,blocknum,data);

// 	/* count the number of writes as we go */
// 	pthread_mutex_unlock(&lock);
// }

// /*
// Block until all dirty blocks in the buffer cache have been cleaned.
// This needs to be implemented.
// */

// void bcache_sync( struct bcache *bc )
// {
// 	for (int i = 0; i < bc->memory_blocks; i++)
// 	{
// 		pthread_mutex_lock(&lock);
// 		struct block *b = &bc->blocks[i];
// 		while(b->state==DIRTY)
// 		{
// 			pthread_cond_wait(&b->cond,&lock);
// 		}
// 		pthread_mutex_unlock(&lock);
// 	}
// }

// /*
// This is the function that will run the I/O scheduler.
// This needs to be implemented.
// */

// void * bcache_io_scheduler( void *vbc )
// {
// 	struct bcache *bc = (struct bcache *)vbc;
// 	while(1)
// 	{		
// 		for(int i=0;i<bc->memory_blocks;i++)
// 		{
// 			pthread_mutex_lock(&lock);
// 			struct block*b=&bc->blocks[i];
// 			switch(b->state){
// 				case WANTREAD:
// 					b->state=READING;
// 					//pthread_mutex_unlock(&lock);
// 					printf("reading data from disk to block%d\n", b->blocknum);
// 					disk_read(bc->disk, b->blocknum, b->data);
// 					//pthread_mutex_lock(&lock);
// 					b->state=READY;
// 					printf("reading has completed\n");
// 					//pthread_cond_broadcast(&b->cond);
// 					pthread_cond_signal(&b->cond);
// 					pthread_mutex_unlock(&lock);
// 					break;
// 				case DIRTY:
// 					b->state = WRITING;
// 					//pthread_mutex_unlock(&lock);
// 					printf("writing data from block%d to disk\n", b->blocknum);
// 					disk_write(bc->disk, b->blocknum, b->data);
// 					b->state=READY;
// 					pthread_cond_signal(&b->cond);
// 					pthread_mutex_unlock(&lock);
// 					break;
// 				default:
// 					pthread_mutex_unlock(&lock);
// 					break;
// 			}
// 		}
// 		//pthread_mutex_unlock(&lock);
// 	}
// }

// /*
// These functions just return basic information about the buffer cache,
// and you shouldn't need to change them.
// */

// /* Return the number of memory blocks in the buffer cache. */

// int bcache_memory_blocks( struct bcache *bc )
// {
// 	return bc->memory_blocks;
// }

// /* Return the number of blocks in the underlying disk. */

// int bcache_disk_blocks( struct bcache *bc )
// {
// 	return disk_nblocks(bc->disk);
// }

// /* Return the number of reads performed on this buffer cache. */

// int bcache_nreads( struct bcache *bc )
// {
// 	return bc->nreads;
// }

// /* Return the number of writes performed on this buffer cache. */

// int bcache_nwrites( struct bcache *bc )
// {
// 	return bc->nwrites;
// }

#include "bcache.h"
#include "disk.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define SIZE 4096
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t io_lock = PTHREAD_MUTEX_INITIALIZER;

enum block_state
{
	EMPTY,	  // Block is free
	WANTREAD, // Block is needed, read requested
	READING,  // Block is currently being read
	READY,	  // Block is ready for use
	DIRTY,	  // Block has been modified
	WRITING	  // Block is currently being written
};
/*
A structure describing the entire buffer cache.
You may add and modify this structure as needed.
*/
struct block
{
	int blocknum;
	enum block_state state;
	char data[SIZE];
	pthread_cond_t cond;
};
struct bcache
{
	struct disk *disk; // The disk object underlying the cache.
	struct block *blocks;
	int memory_blocks; // The total number of memory blocks in the cache.
	int nreads;		   // A running count of read operations.
	int nwrites;	   // A running count of write operations.
};

int no_avail_block(struct bcache *bc)
{
	for (int i = 0; i < bc->memory_blocks; i++)
	{
		if (bc->blocks[i].state==READY)
		{
			break;
			return 0;
		}
	}
	return 1;
}
struct block *find_victim_block(struct bcache *bc, int new_num) // the function for finding a victim block
{
	// we might want to store the victim's data on disk here
	pthread_mutex_lock(&lock);
	struct block *victim = NULL;
	while (1)
	{
		int idx = rand() % bc->memory_blocks;
		if (bc->blocks[idx].state == READY)
		{
			victim = &bc->blocks[idx];
			printf("block %d is the victim\n", victim->blocknum);
			// victim->state=DIRTY;
			pthread_mutex_unlock(&lock);
			return victim;
		}
		while(no_avail_block(bc))
		{
			pthread_cond_wait(&bc->blocks[idx].cond,&lock);
		}
		
	}
	
	
}
struct bcache *bcache_create(struct disk *d, int memory_blocks)
{
	struct bcache *bc = malloc(sizeof(*bc));
	bc->disk = d;
	bc->nreads = 0;
	bc->nwrites = 0;
	bc->memory_blocks = memory_blocks;
	bc->blocks = malloc(sizeof(struct block) * memory_blocks);
	for (int i = 0; i < memory_blocks; i++)
	{
		bc->blocks[i].blocknum = -1; // Indicates block is EMPTY
		bc->blocks[i].state = EMPTY;
		pthread_cond_init(&bc->blocks[i].cond, NULL);
	}
	return bc;
}
/*
Read a block through the buffer cache.
This is a dummy implementation that calls disk_read directly.
It will work only for one thread, and it won't be particularly fast.
Instead, it should manipulate the data structure and wait for the scheduler.
*/
// each block has its own cond?
// I/O scheduler is a background of multiple threads
void bcache_read(struct bcache *bc, int blocknum, char *data)
{
	printf("the program wants to read block%d\n", blocknum);
	pthread_mutex_lock(&lock);
	struct block *ptr = NULL;
	for (int i = 0; i < bc->memory_blocks; i++)
	{
		if (bc->blocks[i].blocknum == blocknum) // if block is in cache,just read it?
		{
			// while (bc->blocks[i].state!=FREE)
			// {
			//  pthread_cond_wait(&bc->blocks[i].cond, &lock);//wait till it is available
			// }
			// bc->blocks[i].state=WANTREAD;
			// break;
			// memcpy(data, ptr->data, SIZE);
			ptr = &bc->blocks[i];
			if (ptr->state == READY) // if ready,just read data from cache block
			{
				printf("block %d is in cache,we read it\n", blocknum);
				memcpy(data, ptr->data, SIZE);
				bc->nreads++;
				pthread_mutex_unlock(&lock);
				return;
			}
			else// if not,read data from disk
			{
				ptr->state = WANTREAD;
				while (ptr->state != READY)
				{
					pthread_cond_wait(&ptr->cond, &lock); // wait till it is available
				}
				memcpy(data, ptr->data, SIZE);
				bc->nreads++;
				pthread_mutex_unlock(&lock);
				return;
			}
			// while(bc->blocks[i].state!=READY)
			// {
			//  pthread_cond_wait(&bc->blocks[i].cond, &lock);//wait till it is available
			// }
			// pthread_cond_broadcast(&bc->blocks[i].cond);
			// // data=ptr->data;
			// memcpy(data, ptr->data, SIZE);
			// bc->nreads++;
			// pthread_mutex_unlock(&lock);
			// return;
		}
	}
	printf("block %d is not in cache,placing it in cache\n", blocknum);
	if (!ptr) // block is not in cache,so put it in cache and read its data from disk
	{
		for (int i = 0; i < bc->memory_blocks; i++)
		{
			if (bc->blocks[i].state == EMPTY) // pick a empty block,use disk_read to read data
			{
				// memcpy(data, bc->blocks[i].data, sizeof(bc->blocks[i].data));
				//  ptr = &bc->blocks[i];
				//  bc->blocks[i].blocknum=blocknum;
				//  bc->blocks[i].state = WANTREAD;
				//  //what about copying condition var?
				// pthread_cond_signal(&ptr->cond);
				printf("block %d put in cache array %d\n", blocknum, i);
				// break;
				ptr = &bc->blocks[i];
				ptr->blocknum = blocknum;
				ptr->state = WANTREAD;
				// wait for io handler read from disk
				while (ptr->state != READY)
				{
					pthread_cond_wait(&ptr->cond, &lock); // wait till it is available
				}
				memcpy(data, ptr->data, SIZE);
				bc->nreads++;
				pthread_mutex_unlock(&lock);
				return;
			}
		}
	}
	if (!ptr) // we cannot find a block that is empty,then we should find a victim
	{
		pthread_mutex_unlock(&lock);
		ptr = find_victim_block(bc, blocknum);
		// before we evict a victim,we should store its info back to disk
		// consider what we should do after finding a victim
		if (ptr) // found a victim
		{		
			pthread_mutex_lock(&lock);	
			// ptr->state=DIRTY;
			// while(ptr->state!=READY)
			// {
			// 	pthread_cond_wait(&ptr->cond,&lock);
			// }
			ptr->blocknum=blocknum;
			memset(ptr->data, 0, SIZE);
			ptr->state=WANTREAD;
			while (ptr->state != READY)
			{
				pthread_cond_wait(&ptr->cond, &lock); // wait till it is available
			}
			// pthread_cond_broadcast(&ptr->cond);
			//  data=ptr->data;
			memcpy(data, ptr->data, SIZE);
			bc->nreads++;
			pthread_mutex_unlock(&lock);
			return;
		}
		else // fails to find victim
		{
			printf("block not found\n");
			return;
		}
	}
	/* count the number of reads as we go */
}
/*
Write a block through the buffer cache.
Like bcache_read, this is a dummy implementation.
*/
void bcache_write(struct bcache *bc, int blocknum, const char *data)
{
	// case 1:writing to a block that is not present in cache, we probably need to create a new block and write data into it
	// case 2:writing to a block that exists,we want to check if it is READY or DIRTY
	// if is dirty,we need to store its data and then write
	printf("program wants to write to block%d\n", blocknum);
	pthread_mutex_lock(&lock);
	struct block *ptr = NULL;
	for (int i = 0; i < bc->memory_blocks; i++)
	{
		if (bc->blocks[i].blocknum == blocknum) // find it and check if it is free
		{
			printf("writing to block%d in block array %d\n", blocknum, i);
			while (bc->blocks[i].state != READY)
			{
				pthread_cond_wait(&bc->blocks[i].cond, &lock); // wait till it is available
			}
			bc->blocks[i].state = DIRTY;
			ptr = &bc->blocks[i];
			bc->nwrites++;
			break;
		}
	}
	if (!ptr) // block does not exist,try to find a free one
	{
		for (int i = 0; i < bc->memory_blocks; i++)
		{
			if (bc->blocks[i].state == EMPTY)
			{
				// memcpy(data, bc->blocks[i].data, sizeof(bc->blocks[i].data));//write to the block
				// memcpy(bc->blocks[i].data,data,sizeof(data));
				//  bc->blocks[i].data=data;
				//  bc->blocks[i].blocknum=blocknum;
				//  bc->blocks[i].state=DIRTY;
				// bc->blocks[i].state = READY;
				// what about copying condition var?
				printf("putting block %d in char array %d\n", blocknum, i);
				ptr = &bc->blocks[i];
				strncpy(ptr->data, data, sizeof(ptr->data) - 1);
				ptr->data[sizeof(ptr->data) - 1] = '\0';
				ptr->blocknum = blocknum;
				ptr->state = DIRTY;
				pthread_cond_signal(&ptr->cond);
				bc->nwrites++;
				pthread_mutex_unlock(&lock);
				return;
			}
		}
	}
	if (!ptr)
	{
		pthread_mutex_unlock(&lock);
		ptr = find_victim_block(bc, blocknum);
		pthread_mutex_lock(&lock);
	}
	if (ptr)
	{
		// memcpy(bc->blocks[i].data,data,sizeof(data));
		strncpy(ptr->data, data, sizeof(ptr->data) - 1);
		ptr->data[sizeof(ptr->data) - 1] = '\0';
		ptr->blocknum = blocknum;
		ptr->state = DIRTY;
		pthread_cond_signal(&ptr->cond);
		bc->nwrites++;
		pthread_mutex_unlock(&lock);
		return;
	}
	else
	{
		printf("victim block not found\n");
		pthread_mutex_unlock(&lock);
		return;
	}
	// memcpy(ptr->data, data, sizeof(data));//write data to the block
	// ptr->data=data;//return immediately
	//  memcpy(data, bc->blocks[i].data, sizeof(bc->blocks[i].data));

	/* Just do the write.  Unsafe! */
	// disk_write(bc->disk,blocknum,data);

	/* count the number of writes as we go */
	pthread_mutex_unlock(&lock);
}

/*
Block until all dirty blocks in the buffer cache have been cleaned.
This needs to be implemented.
*/

void bcache_sync(struct bcache *bc)
{
	for (int i = 0; i < bc->memory_blocks; i++)
	{
		pthread_mutex_lock(&lock);
		struct block *b = &bc->blocks[i];
		while (b->state == DIRTY)
		{
			pthread_cond_wait(&b->cond, &lock);
		}
		pthread_mutex_unlock(&lock);
	}
}

/*
This is the function that will run the I/O scheduler.
This needs to be implemented.
*/

void *bcache_io_scheduler(void *vbc)
{
	
	struct bcache *bc = (struct bcache *)vbc;
	while (1)
	{
		for (int i = 0; i < bc->memory_blocks; i++)
		{
			pthread_mutex_lock(&lock);
			struct block *b = &bc->blocks[i];
			switch (b->state)
			{
			case WANTREAD:
				b->state = READING;
				// pthread_mutex_unlock(&lock);
				printf("reading data from disk to block%d\n", b->blocknum);
				disk_read(bc->disk, b->blocknum, b->data);
				// pthread_mutex_lock(&lock);
				b->state = READY;
				printf("reading has completed\n");
				// pthread_cond_broadcast(&b->cond);
				pthread_cond_broadcast(&b->cond);
				pthread_mutex_unlock(&lock);
				break;
			case DIRTY:
				b->state = WRITING;
				// pthread_mutex_unlock(&lock);
				printf("writing data from block%d to disk\n", b->blocknum);
				disk_write(bc->disk, b->blocknum, b->data);
				b->state = READY;
				pthread_cond_broadcast(&b->cond);
				pthread_mutex_unlock(&lock);
				break;
			default:
				pthread_mutex_unlock(&lock);
				break;
			}
		}
		// pthread_mutex_unlock(&lock);
	}
}

/*
These functions just return basic information about the buffer cache,
and you shouldn't need to change them.
*/

/* Return the number of memory blocks in the buffer cache. */

int bcache_memory_blocks(struct bcache *bc)
{
	return bc->memory_blocks;
}

/* Return the number of blocks in the underlying disk. */

int bcache_disk_blocks(struct bcache *bc)
{
	return disk_nblocks(bc->disk);
}

/* Return the number of reads performed on this buffer cache. */

int bcache_nreads(struct bcache *bc)
{
	return bc->nreads;
}

/* Return the number of writes performed on this buffer cache. */

int bcache_nwrites(struct bcache *bc)
{
	return bc->nwrites;
}
