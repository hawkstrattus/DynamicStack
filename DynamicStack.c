#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define FRAME_LIMIT 50 // max amount of stack frames

// add documentation, create SO and header, upload to github


/* Structure Definitions */

struct Frame
{
	void* data;
	size_t DataSize;
	struct Frame* next;
};

struct StackManager
{
	char StackID[30];
	int FrameCount;
	struct Frame* top;
};


/* Function Prototypes */

struct StackManager* CreateStack(const char* StackID);
struct Frame* Push(struct StackManager*, void* data, size_t DataSize);
void* Pop(struct StackManager*, int PopCount);
bool DeleteStack(struct StackManager*);


/* Functions */

struct StackManager* CreateStack(const char* StackID)
{
	// creating an instance of StackManager called Manager
	struct StackManager* manager = calloc(1, sizeof(struct StackManager));	// allocating memory for our stack manager on the heap

	// error checking NodeManager
	if (manager == NULL)
	{
		perror("Failed to allocate memory for StackManager");
		return NULL;
	}

	strncpy(manager->StackID, StackID, sizeof(manager->StackID) - 1);
	struct Frame* BaseFrame = calloc(1, sizeof(struct Frame));

	if (BaseFrame == NULL)
	{
		perror("Failed to allocate memory");
		return NULL;
	}

	manager->FrameCount = 1;
	BaseFrame->next = NULL;
	manager->top = BaseFrame;

	return manager;
}




struct Frame* Push(struct StackManager* StackManager, void* data, size_t DataSize)
{
	// error checking StackManager
	if (StackManager == NULL)
	{
		perror("StackManager in push is null");
		return NULL;
	}

	// error checking data
	if (data == NULL)
	{
		perror("data in push is null");
		return NULL;
	}

	int FrameCount = StackManager->FrameCount;

	if (FrameCount >= FRAME_LIMIT)
	{
		fprintf(stderr, "\nInvalid FrameCount\n");
		return NULL;
	}

	struct Frame* StackFrame = calloc(1, sizeof(struct Frame));
	StackFrame->next = StackManager->top;
	StackManager->top = StackFrame;
	StackFrame->data = data;
	StackManager->FrameCount += 1;
	StackFrame->DataSize = DataSize;

	return StackManager->top;
	
}

void* Pop(struct StackManager* StackManager, int PopCount)
{
	// error checking StackManager
	if (StackManager == NULL)
	{
		perror("StackManager in pop is null");
		return NULL;
	}

	int FrameCount = StackManager->FrameCount;

	if (PopCount > FrameCount || PopCount <= 0)
	{
		perror("PopCount Invalid");
		return NULL;
	}


	if (PopCount == 1)
	{
		struct Frame* OldTop = StackManager->top;
		void* DataReturn = calloc(1, OldTop->DataSize);
        if (DataReturn == NULL) // error handling for calloc
        {
            perror("Memory allocation failed");
            return NULL;
        }

		memcpy(DataReturn, StackManager->top->data, StackManager->top->DataSize);
		StackManager->top = StackManager->top->next;
		free(OldTop);

		StackManager->FrameCount -= PopCount;

		return DataReturn;

	}
	else
	{	

		void** DataReturnArray = calloc(PopCount, sizeof(void*));
		if (DataReturnArray == NULL) // error handling for calloc
        {
            perror("Memory allocation failed");
            return NULL;
        }
		for (int i = 0; i < PopCount; i++)
		{
			struct Frame* OldTopMulti = StackManager->top;
			DataReturnArray[i] = calloc(1, OldTopMulti->DataSize);

			if (DataReturnArray[i] == NULL) // error handling for calloc
            {
                perror("Memory allocation failed");
                // freeing any previously allocated memory
                for (int j = 0; j < i; j++)
                {
                	free(DataReturnArray[j]);
                }

                free(DataReturnArray);
                return NULL;
            }

			memcpy(DataReturnArray[i], StackManager->top->data, StackManager->top->DataSize);
			StackManager->top = StackManager->top->next;
			free(OldTopMulti);
		}
		return DataReturnArray;
	}

	StackManager->FrameCount -= PopCount;

	return NULL;

}


bool DeleteStack(struct StackManager* StackManager)
{
	// error checking StackManager
	if (StackManager == NULL)
	{
		perror("StackManager in DeleteStack is null");
		return NULL;
	}

	while (StackManager->top != NULL)
	{
		struct Frame* OldTop = StackManager->top;
		StackManager->top = StackManager->top->next;
		free(OldTop);
	}

	if (StackManager->top == NULL)
	{
		free(StackManager);
	}
	else
	{
		return false;
	}


	return true;
}





int main()
{

	char mytext[] = "testowesto";
	int meh = 234;
	int welo = 9699;
	struct StackManager* MyStack = CreateStack("lelo");
	Push(MyStack, mytext, sizeof(mytext));
	Push(MyStack, &meh, sizeof(meh));
	Push(MyStack, &welo, sizeof(meh));
	printf("%d\n", *(int*)MyStack->top->data);
	printf("%s\n", MyStack->StackID);
	printf("%d\n", MyStack->FrameCount);
	printf("%p\n", MyStack->top);
	printf("%p\n", MyStack->top->next);
	printf("%p\n", MyStack->top->next->next);
	printf("%p\n", MyStack->top->next->next->next);

	void* DataReturn = Pop(MyStack, 1);
	printf("%d\n", *(int*)DataReturn);
	void** DataReturnArray = Pop(MyStack, 2);
	printf("%d\n", *(int*)DataReturnArray[0]);
	printf("%s\n", (char*)DataReturnArray[1]);
	free(DataReturn);
	free(DataReturnArray[0]);
	free(DataReturnArray[1]);
	free(DataReturnArray);

	bool success = DeleteStack(MyStack);
	printf("\n%d\n", success);

	return 0;
}