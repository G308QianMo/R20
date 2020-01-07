#include "include.h"
//应该用不到

sRobotTypeDef G_Robot_Master;


void Robot_Speed_Upadte(void);
void Robot_Location_Update(void);

void Robot_Location_Update(void)
{
	nloc = GYRO_Get_Location();
}

/**
 * @function    : 
 * @Description : ??????????
**/
static void set_vect(VectorTypeDef *p_vect, PointTypeDef *p_start, PointTypeDef *p_end)
{
	memcpy(&p_vect->start, p_start, sizeof(PointTypeDef));
	memcpy(&p_vect->end, p_end,   sizeof(PointTypeDef));
}

////??????
static uint32_t get_vect_len(VectorTypeDef *p_vect)
{
	int64_t x, y, l;
	x = p_vect->end.coords.x - p_vect->start.coords.x;
	y = p_vect->end.coords.y - p_vect->start.coords.y;
	l = my_sqrt(square(x)+square(y));
//	printf("\r\n%d\t%d\t%d\t%d\r\n",p_vect->End.Coords.y,p_vect->Start.Coords.y,(int32_t)y,(int32_t)l);
	return l;
}


float Cycle_T_1;

// ?????????
void Robot_Speed_Upadte(void)
{
    // ??????
    set_vect(&G_Robot_Master.spd_vect, &G_Robot_Master.Last_Position, &G_Robot_Master.Now_Position);
	   /*??????*/
	Cycle_T_1 = Get_Cycle_T(1)/1000.f;
	   /*??????*/
    G_Robot_Master.cur_speed = (double)get_vect_len(&G_Robot_Master.spd_vect) / (double)Cycle_T_1;
//	
   //printf("Cycle_T_1 : \r\n%d\r\n", (int)(Cycle_T_1*1000) );
		memcpy(&G_Robot_Master.Last_Position, &G_Robot_Master.Now_Position, sizeof(PointTypeDef));
//    /*??????*/
//    G_Robot_Master.cur_speed = (double)get_vect_len(&G_Robot_Master.spd_vect) / (0.005);
}