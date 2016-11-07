________________

Buffer Manager
________________

Team Members
_____________

Rushit Pandya (Leader) - A20381916 - rpandya4@hawk.iit.edu
Subhdeep Roy - A20358508 - sroy7@hawk.iit.edu
Dutt Patel - A20380571 - dpatel106@hawk.iit.edu
Rumin Shah - A20369998 - rshah90@hawk.iit.edu
__________

File List
__________

README.txt
dberror.c
dberror.h
storage_mgr.c
storage_mgr.h
test_helper.h
Makefile
buffer_mgr.c
buffer_mgr_stat.c
test_assign2_1.c
test_assign2_2.c
buffer_mgr.h
buffer_mgr_stat.h
dt.h
____

Aim
____

To implement a buffer manager that manages a fixed number of pages in memory that represent pages from a page file managed by the storage manager. The memory pages managed by the buffer manager are called page frames or in simple words called frames. The combination of a page file as well as the page frames that stores pages from that file is referred to as a Buffer Pool. The buffer managers has a major responsibility of being able to handle more than one buffer pool that are open and that too at the same time. However, only one buffer pool for each page file can exists. One page replacement strategy like FIFO, LFU, and LRU is used by the buffer pool which at the time of its initialization is determined. 
_________________________

Installation Instruction
_________________________

1. Go the path in the command prompt to the directory where the extracted files of zip folder named "assign2_group27.zip" are present.
2. Run the command: make clean
3. Run the command: make
4. Run the command for testing test_assign2.c: ./test_assign2 
5. To remove object files, run command: make clean 
6. We also provided a page implementation strategy called LFU (Least Frequently Used) in additional test cases file named "test_assign2_2.c". To run those test cases use the command: ./test_assign2_2.
____________________

Data Structure Used
____________________

1. Linked List
It is the main data structure that holds all the other data structures. Also the two data structures given below is connected by Linked List in such a way that given the information of BM_BufferPoolInfo gives all the information regarding the PageFrames.

2. BM_BufferPoolInfo Structure
Keeps track of all the file pointers as well maintains page related statistics of buffer manager.

3.PageFrame Structure
Keeps information of page frames that are obtained from any given page number and other associated variables.
_______________________

Functions Descriptions
_______________________
_______________________

Buffer Pool Functions
_______________________

initBufferPool()
This function creates a new buffer pool with "numPages" page frames using the page replacement strategy "strategy". The pool is used to cache pages from the page file with name "pageFileName". Initially, all page frames are empty. The page file is already exist, i.e., this method does not generate a new page file. "stratData" is used to pass parameters for the page replacement strategy.

shutdownBufferPool()
This function destroys a buffer pool. It free up all the resources like the memory allocated for page frames associated with buffer pool. If there exist any dirty pages in the buffer pool, then they are written back to disk before destroying the pool. It is an error to shutdown a buffer pool that has pinned pages.

forceFlushPool()
This function causes all the dirty pages with fix count 0 from the buffer pool to be written back to disk.
_______________________

Additional Functions
_______________________

PoolSize()
This function returns the total size of the pool.

PageFrame *findPage()
This function returns the desired page that is used by the buffer manager. Basically the PageNumber is used to identify the required frame.

int GetEmptyFrame()
This function is used to get an empty frame from the pool if there exist any. It will simply check for any empty frame available from the pool, if not then it will return -1 which indicate that the buffer is full. Due to the full buffer, the buffer manager will have to use any replacement strategy to make space for new page to arrive. 

void ChangeCount()
This function is used to find a page that has to be replaced. Basically the buffer manager will look for a page that is used for least amount of time and was arrived in the pool much earlier before any other page.

PageFrame *findCount()
This function returns the frame that has to be replaced from the pool.

int replacementStrategy()
This function os used to replace the page from the pool with the new page that is required. Replacement strategies like LRU or FIFO is used. It also checks it dirty condition before removing the page from the pool. If the page is dirty (is_dirty value=1) then the writeBlock function writes its content back to the disk and update the status flags or else it simply reads and update the status flags.

int getHighestCount()
This function returns the count of page frame with highest count. This page frame count indicated which page needs to be replaced next if required by the buffer manager. 

PageFrame *findLowFreqCnt()
This function returns the frame that has the lowest frequency count or accesses for very less amount of time.

int LFU()
This function is an implementation of a replacement strategy called LFU or Least Frequently Used. It simply replaces the page from the buffer pool that is used least frequently. Here also, if the page is dirty then it is first written back to the disk and then replaced or else read normally and then update the corresponding flags.
_________________________

Page Management Functions
_________________________

pinPage()
This function pins the page with given page number pageNum. The buffer manager is responsible to set the pageNum field of the page handle passed to the method. Similarly, the data field should point to the page frame the page is stored in the area in memory storing the content of the page.

unpinPage()
This function unpins the page page. The pageNum field of page should be used to figure out which page to unpin.

markDirty()
This function marks a page as dirty.

forcePage()
This function writes the current content of the page back to the page file on the disk.
____________________

Statistics Functions
____________________

getFrameContents()
This function returns an array of PageNumbers of size numPages where the ith element is the number of the page stored in the ith page frame. An empty page frame is represented using the constant NO_PAGE.

getDirtyFlags()
This function returns an array of boolean values of size numPages where the ith element is TRUE if the page stored in the ith page frame is dirty. Empty page frames are considered as clean.

getFixCounts()
This function returns an array of integers of size numPages where the ith element is the fix count of the page stored in the ith page frame. Return 0 for empty page frames.

getNumReadIO()
This function returns the number of pages that have been read from disk since a buffer pool has been initialized. The buffer manager initialize this statistic at pool creating time and update whenever a page is read from the page file into a page frame.

getNumWriteIO()
This function returns the number of pages written to the page file since the buffer pool has been initialized.
_______________________

Additional Error Codes 
_______________________

The error codes below were included in Storage Manager:
#define RC_NUMBER_OF_PAGES_EXISTS 5
#define RC_FILE_READ_ERROR 6
#define RC_WRITE_OUT_OF_BOUND_INDEX 7
#define RC_FILE_ALREADY_PRESENT 8

The error codes below are included in Buffer Manager:
#define RC_BUFFER_POOL_NOT_INIT 8
#define RC_BUFFER_POOL_ALREADY_INIT 9
#define RC_BUFFER_POOL_ERROR_IN_PINPAGE 10
#define RC_BUFFER_POOL_PINPAGE_PRESENT 11
#define RC_BUFFER_POOL_PAGE_INUSE 12
#define RC_BUFFER_POOL_ERROR_IN_UNPINPAGE 13
#define RC_BUFFER_POOL_ERROR_IN_FORCEPAGE 14
#define RC_BUFFER_POOL_ERROR_IN_MARKDIRTY 15
#define RC_BUFFER_POOL_NULL 16

_____________

Extra Credit 
_____________

1. Implementation of LFU(Least Frequently Used) page replacement strategy.
2. All the functions of buffer manager are thread-safe. So multiple clients can concurrently access the buffer manager.
_________________

Additional Files
_________________

test_assign2_2.c
This file contains additional test cases for testing. It contains a page replacement strategy called LFU or Least Frequently Used. Note that both the test cases efficiently ensures that the buffer pool are thread safe. 
________________

No Memory Leaks 
________________

All the test cases are tested with Valgrind tool and no memory leaks exists. 
___________

Test Cases 
___________

testLFU()
This function tests the Least Frequently Used page replacement strategy.

testFIFO()
This function tests the First In First Out page replacement strategy.

testLRU()
This function tests the Least Recently Used page replacement strategy.
