#include "modisg.h"

#include <linux/mutex.h> /* mutex */
#include <linux/slab.h> /* kfree */

#define MAXLISTSIZE 1000

/* структура для хранения пакетов */
struct SPackListEl {
	unsigned char *m_pucBuf;
	unsigned int m_uiSize;
  unsigned int m_uiBufSize;
};
struct SPackList {
  struct SPackListEl m_msoPackList[MAXLISTSIZE];
	int m_iHead;
	int m_iTail;
	struct mutex m_soMutex;
};

struct SPackList g_soPackList;

void modisg_list_init(void)
{
  memset(&g_soPackList, 0, sizeof(g_soPackList));
  g_soPackList.m_iHead = g_soPackList.m_iTail = -1;
	mutex_init(&g_soPackList.m_soMutex);
}

void modisg_list_add(void *p_pData, unsigned int p_uiDataLen)
{
  if(0 == p_uiDataLen)
    return;

	if(0 == mutex_trylock(&g_soPackList.m_soMutex)) {
		return;
	} else {
    if(g_soPackList.m_iHead == -1) {
      g_soPackList.m_iHead = 0;
      g_soPackList.m_iTail = 0;
      if(g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize) {
        if(g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize < p_uiDataLen) {
          kfree(g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_pucBuf);
          g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_pucBuf = kmalloc(p_uiDataLen, __GFP_FS | GFP_ATOMIC);
          g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize = p_uiDataLen;
        }
      } else {
        g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_pucBuf = kmalloc(p_uiDataLen, __GFP_FS | GFP_ATOMIC);
        g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize = p_uiDataLen;
      }
      if(NULL == g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_pucBuf) {
        g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiSize = 0;
        g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize = 0;
        g_soPackList.m_iHead = g_soPackList.m_iTail = -1;
        goto unlock_and_exit;
      }
      memcpy(g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_pucBuf, p_pData, p_uiDataLen);
      g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiSize = p_uiDataLen;
    } else {
      ++g_soPackList.m_iTail;
      g_soPackList.m_iTail %= MAXLISTSIZE;
      if(g_soPackList.m_iHead == g_soPackList.m_iTail) {
        g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize = 0;
        ++g_soPackList.m_iHead;
        g_soPackList.m_iHead %= MAXLISTSIZE;
      }
      if(g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize) {
        if(g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize < p_uiDataLen) {
          kfree(g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_pucBuf);
          g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_pucBuf = kmalloc(p_uiDataLen, __GFP_FS | GFP_ATOMIC);
          g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize = p_uiDataLen;
        }
      } else {
        g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_pucBuf = kmalloc(p_uiDataLen, __GFP_FS | GFP_ATOMIC);
        g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize = p_uiDataLen;
      }
      if(NULL == g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_pucBuf) {
        g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiSize = 0;
        g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiBufSize = 0;
        if(g_soPackList.m_iTail == 0)
          g_soPackList.m_iTail = MAXLISTSIZE - 1;
        else
          --g_soPackList.m_iTail;
        goto unlock_and_exit;
      }
      g_soPackList.m_msoPackList[g_soPackList.m_iTail].m_uiSize = p_uiDataLen;
    }
  }
unlock_and_exit:
	mutex_unlock(&g_soPackList.m_soMutex);
}

void modisg_list_read(unsigned char *p_pucBuf, ssize_t *p_pstBufSize)
{
	if(0 == mutex_trylock(&g_soPackList.m_soMutex)) {
		*p_pstBufSize = 0;
		return;
	}
	if(0 == *p_pstBufSize || NULL == p_pucBuf) {
		*p_pstBufSize = 0;
		goto unlock_and_exit;
	}
  if(g_soPackList.m_iHead == -1) {
		*p_pstBufSize = 0;
		goto unlock_and_exit;
  }
  if(g_soPackList.m_iHead == g_soPackList.m_iTail) {
    if(g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_pucBuf) {
      memcpy(p_pucBuf, g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_pucBuf, g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize);
      *p_pstBufSize = g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize;
      g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize = 0;
    } else {
      *p_pstBufSize = 0;
    }
    g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize = 0;
    g_soPackList.m_iHead = g_soPackList.m_iTail = -1;
  } else {
    if(g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_pucBuf) {
      memcpy(p_pucBuf, g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_pucBuf, g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize);
      *p_pstBufSize = g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize;
      g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize = 0;
    } else {
      *p_pstBufSize = 0;
    }
    g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize = 0;
    ++g_soPackList.m_iHead;
    g_soPackList.m_iHead %= MAXLISTSIZE;
  }
unlock_and_exit:
	mutex_unlock(&g_soPackList.m_soMutex);
}

void modisg_list_clean(void)
{
	mutex_lock(&g_soPackList.m_soMutex);
  if(g_soPackList.m_iHead != -1) {
    while(1) {
      if(g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_pucBuf) {
        kfree(g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_pucBuf);
        g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_pucBuf = NULL;
      }
      g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiSize = 0;
      g_soPackList.m_msoPackList[g_soPackList.m_iHead].m_uiBufSize = 0;
      if(g_soPackList.m_iHead == g_soPackList.m_iTail) {
        break;
      }
      ++g_soPackList.m_iHead;
      g_soPackList.m_iHead %= MAXLISTSIZE;
    }
  }
	mutex_unlock(&g_soPackList.m_soMutex);
	mutex_destroy(&g_soPackList.m_soMutex);
}
