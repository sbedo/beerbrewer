#include <stddef.h>
#include <string.h>
#include "utilities_fifo.h"

FiFoRetVal FiFo_Init(FiFoInstance* i_fifoInstance, uint8_t* i_workarea, uint16_t i_fifoSize_items, uint16_t i_itemSize_bytes)
{
	FiFoRetVal retVal = FiFoRetVal_OK;

	if((i_fifoInstance != NULL) && (i_workarea != NULL) && (i_fifoSize_items != 0))
	{
		memset(i_fifoInstance,0,sizeof(FiFoInstance));

		i_fifoInstance->size      = i_fifoSize_items;
		i_fifoInstance->itemSize  = i_itemSize_bytes;
		i_fifoInstance->pWorkarea = i_workarea;
		i_fifoInstance->count = 0;
		i_fifoInstance->maxcount = 0;

		memset(i_fifoInstance->pWorkarea, 0, i_fifoInstance->itemSize*i_fifoInstance->size);

		i_fifoInstance->isInited = true;
	}
	else
	{
		retVal = FiFoRetVal_Error_Input;
	}
	return retVal;
}

FiFoRetVal FiFo_AddItem(FiFoInstance* i_fifoInstance, void* i_newItem)
{
	FiFoRetVal retVal        = FiFoRetVal_OK;
	uint32_t   newItemOffset = 0;

	if((i_fifoInstance != NULL) && (i_newItem != 0))
	{
	    if( i_fifoInstance->count < i_fifoInstance->size )
        {
            newItemOffset = i_fifoInstance->head*i_fifoInstance->itemSize;
            memcpy(i_fifoInstance->pWorkarea+newItemOffset , i_newItem , i_fifoInstance->itemSize);

            i_fifoInstance->head++;
            if(i_fifoInstance->head >= i_fifoInstance->size)
            {
                i_fifoInstance->head = 0;
            }

            //! Increase count and check if it is higher than actual maxcount
            if( ++(i_fifoInstance->count) > i_fifoInstance->maxcount )
            {
                i_fifoInstance->maxcount = i_fifoInstance->count;
            }
        }
        else
		{
			retVal = FiFoRetVal_Error_FiFoEmpty;
		}
	}
	else
	{
		retVal = FiFoRetVal_Error_Input;
	}

	return retVal;
}

FiFoRetVal FiFo_GetItem(FiFoInstance* i_fifoInstance, void* o_item)
{
	FiFoRetVal retVal     = FiFoRetVal_OK;
	uint32_t   itemOffset = 0;

	if((i_fifoInstance != NULL) && (o_item != 0) )
	{
		//if(i_fifoInstance->tail != i_fifoInstance->head)  // This does not work when the fifo is full
		if( i_fifoInstance->count > 0 )
		{
			itemOffset = i_fifoInstance->tail*i_fifoInstance->itemSize;
			memcpy(o_item, i_fifoInstance->pWorkarea+itemOffset,i_fifoInstance->itemSize);

			i_fifoInstance->tail++;
			if(i_fifoInstance->tail >= i_fifoInstance->size)
			{
				i_fifoInstance->tail = 0;
			}
			//! Decreace count -> we should not be here if count is zero!
			--(i_fifoInstance->count);
		}
		else
		{
			retVal = FiFoRetVal_Error_FiFoEmpty;
		}
	}
	else
	{
		retVal = FiFoRetVal_Error_Input;
	}

	return retVal;
}

FiFoRetVal FiFo_PeekItem(FiFoInstance* i_fifoInstance, void* o_item)
{
    FiFoRetVal retVal     = FiFoRetVal_OK;
    uint32_t   itemOffset = 0;

    if((i_fifoInstance != NULL) && (o_item != 0) )
    {
        if( i_fifoInstance->tail != i_fifoInstance->head )
        {
            itemOffset = i_fifoInstance->tail * i_fifoInstance->itemSize;
            memcpy( o_item, i_fifoInstance->pWorkarea + itemOffset, i_fifoInstance->itemSize );
        }
        else
        {
            retVal = FiFoRetVal_Error_FiFoEmpty;
        }
    }
    else
    {
        retVal = FiFoRetVal_Error_Input;
    }

    return retVal;
}

uint16_t FiFo_GetCount(FiFoInstance* i_fifoInstance)
{
    uint16_t retVal = 0;
    if(i_fifoInstance != NULL)
    {
        retVal = i_fifoInstance->count;
    }
    return retVal;
}

uint16_t FiFo_GetMaxCount(FiFoInstance* i_fifoInstance)
{
    uint16_t retVal = 0;
    if(i_fifoInstance != NULL)
    {
        retVal = i_fifoInstance->maxcount;
    }
    return retVal;
}

FiFoRetVal FiFo_Clear(FiFoInstance* i_fifoInstance)
{
    FiFoRetVal retVal = FiFoRetVal_OK;
    if( i_fifoInstance != NULL )
    {
        i_fifoInstance->count = 0;
        //i_fifoInstance->maxcount = 0;
        i_fifoInstance->head = 0;
        i_fifoInstance->tail = 0;
    }
    else
    {
        retVal = FiFoRetVal_Error_Input;
    }
    return retVal;
}
