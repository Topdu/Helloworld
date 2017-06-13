#include "stdio.h"
#include<io.h>
#include "string.h"
#include "ElevatorSimulation.h"
#include "DataImport.h"
#include "DataModel.h"
#include "malloc.h"
#include "ResponseCalculate.h"


extern int usercall_list_len;
extern USERCALL *usercall_list;//用户指令数组首地址，数组根据用户指令多少动态生成
extern SYSPARAM param_array[32];//系统参数数组
extern ELEVATORSTATE elevator_b,elevator_a;
extern int time ;
extern RESPONSELISTHEADNODE *response_list;//电梯响应指令队列，用于指示当前响应的电梯指令

void  ImportUserCall(FILE  *fp) //将用户指令写入仿真结果文件的函数，应在响应结果存储模块
{
	//
	int item_num;
	fprintf(fp,"%d\n",usercall_list_len);
	for(item_num=0;item_num<usercall_list_len;item_num++)
	fprintf(fp,"%d,%d,%d\n",usercall_list[item_num].user_floor,usercall_list[item_num].user_target,usercall_list[item_num].call_time);
	fprintf(fp,"*********************\n");
}


void  ImportSimulateParam(FILE  *fp) //将仿真参数写入仿真结果文件的函，应在响应结果存储模块
{
	//		
	fseek(fp,0,SEEK_END);
	fprintf(fp,"3\n");
	fprintf(fp,"ElevatorHeight=%s\n",param_array[2].param_value);
	fprintf(fp,"ElevatorSpeed=1\n");
	fprintf(fp,"SimulateSpeed=1000\n");
	fprintf(fp,"*********************\n");
	
}

void  ImportSimulateResult(FILE *fp) //将电梯当前时刻状态写入仿真结果文件的函数，应在响应结果存储模块
{
	//

	fseek(fp,0,SEEK_END);
	fprintf(fp,"%d,A,%d,%c,B,%d,%c;",time,elevator_a.current_floor,elevator_a.run_state,elevator_b.current_floor,elevator_b.run_state);
	if(elevator_a.serve_list!=NULL&&elevator_b.serve_list!=NULL)
	{
		fprintf(fp,"<%d,%d,A>,<%d,%d,B>",elevator_a.serve_list->user_call->user_floor,elevator_a.serve_list->user_call->user_target,elevator_b.serve_list->user_call->user_floor,elevator_b.serve_list->user_call->user_target);
		if(response_list->head!=NULL)
		{
			fprintf(fp,"<%d,%d,N>\n",usercall_list[response_list->head->usercall_index].user_floor,usercall_list[response_list->head->usercall_index].user_target);
		}
		else
		{
			fprintf(fp,"\n");
		}
	}
	else
	{
		fprintf(fp,"\n");
	}




}
