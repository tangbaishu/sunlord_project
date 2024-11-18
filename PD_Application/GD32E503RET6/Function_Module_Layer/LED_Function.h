#ifndef _LED_FUNCTION_H_
#define _LED_FUNCTION_H_

#ifndef uint8
#ifndef _DECA_UINT8_
#define _DECA_UINT8_
typedef unsigned char uint8;
#endif
#endif

typedef struct
{
	void (*LED_Function_Init) (void);
	void (*LED_Function_ON) (uint8 param);
	void (*LED_Function_OFF) (uint8 param);
	void (*LED_Function_Turn) (uint8 param);
}LED_Function_List_t;
extern LED_Function_List_t LED_Function_List;

void LED_Check(void);

#endif
