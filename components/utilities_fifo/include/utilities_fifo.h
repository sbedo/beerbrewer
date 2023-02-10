#ifndef UTILITIES_FIFO_H_
#define UTILITIES_FIFO_H_
#include <stdint.h>
#include <stdbool.h>

#define FIFO_SIZE_MAX       (65534)

typedef enum {
	FiFoRetVal_OK,
	FiFoRetVal_Error_Input,
	FiFoRetVal_Error_FiFoIsFull,
	FiFoRetVal_Error_FiFoEmpty,
	FiFoRetVal_End
} FiFoRetVal;

typedef struct _FiFoInstance{
	uint8_t* pWorkarea;
	uint16_t  tail;
	uint16_t  head;
	uint16_t  size;
	uint16_t itemSize;
	uint16_t count;
	//! only for debug -> find optimal size
	uint16_t maxcount;
	bool     isInited;
}FiFoInstance;

FiFoRetVal FiFo_Init(FiFoInstance* i_fifoInstance, uint8_t* i_workarea, uint16_t i_fifoSize_items, uint16_t i_itemSize_bytes);
FiFoRetVal FiFo_AddItem(FiFoInstance* i_fifoInstance, void* i_newItem);
FiFoRetVal FiFo_GetItem(FiFoInstance* i_fifoInstance, void* o_item);
FiFoRetVal FiFo_PeekItem(FiFoInstance* i_fifoInstance, void* o_item);
FiFoRetVal FiFo_Clear(FiFoInstance* i_fifoInstance);

#endif /* UTILITIES_FIFO_H_ */