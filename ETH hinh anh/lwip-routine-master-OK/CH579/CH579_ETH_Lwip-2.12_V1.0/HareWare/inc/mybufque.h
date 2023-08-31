

#ifndef   __MY_BUF_QUE_H__
#define   __MY_BUF_QUE_H__

#include "parameter_setting.h"

//定义队列元素缓冲区长度
#define   MQE_BUF_SIZE    1064 //1088

//定义队列元素个数
#define   BQE_SIZE     2 


struct  _e_buf
{
	u8    buf[MQE_BUF_SIZE];  //缓冲区
	u16   buf_len;  //缓冲区中有效数据长度
	u32   timestamp; //该缓冲区接收到数据的时间戳
	//u8    data_source; //数据来源
};

typedef struct  _e_buf   e_buf_t;


typedef struct  _my_buf_que
{
	e_buf_t  e_buf[BQE_SIZE];   //定义队列元素

	u8  head_index;  //指向下一个待读取的缓冲区
	u8  tail_index;  //指向下一个带写入的缓冲区

	u8  e_buf_num;   //队列元素有效个数	
}my_buf_que_t;


extern my_buf_que_t  g_eth_buf_que;   //的定义网口缓冲区队列



extern  void     init_my_buf_que(my_buf_que_t *p_my_buf_que);
extern  e_buf_t  *get_valid_e_from_que_head(my_buf_que_t *p_my_buf_que);
extern  e_buf_t  *get_idle_e_from_que_tail(my_buf_que_t *p_my_buf_que);
extern  void     e_buf_num_add_one(my_buf_que_t *p_my_buf_que);
extern  void     e_buf_num_sub_one(my_buf_que_t *p_my_buf_que);





#endif 


