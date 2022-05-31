#include <atmel_start.h>
#include <EtherCAT/ethercat_e54.h>
//#include <TactileSensors/TactileSensors.h>
#include <driver_init.h>
#include "stdio.h"

struct io_descriptor *USART_io;

static struct timer_task TIMER_task1;
static struct timer_task TIMER_task2;
uint16_t capacitive_sensor_result[4] = {0,0,0,0};

uint8_t counter = {0};
uint8_t test={0};

//write	
volatile	uint8_t *mu_controller__status		=&ram_buffer[ram_wr_start];
volatile	uint8_t *sensor__status				=(((uint8_t *)&ram_buffer[ram_wr_start])+1);

volatile uint16_t  *resistive_sensor__value		=(((uint16_t *)&ram_buffer[ram_wr_start])+1);

volatile uint16_t  *piezoelectric_sensor__value	=(((uint16_t *)&ram_buffer[ram_wr_start+1]));

volatile uint16_t  *capacitive_sensor__x_p		=(((uint16_t *)&ram_buffer[ram_wr_start+1])+1);
volatile uint16_t  *capacitive_sensor__x_n		=(((uint16_t *)&ram_buffer[ram_wr_start+2]));
volatile uint16_t  *capacitive_sensor__y_n		=(((uint16_t *)&ram_buffer[ram_wr_start+2])+1);
volatile uint16_t  *capacitive_sensor__y_p		=(((uint16_t *)&ram_buffer[ram_wr_start+3]));

volatile uint16_t  *hall_sensor__x				=(((uint16_t *)&ram_buffer[ram_wr_start+3])+1);
volatile uint16_t  *hall_sensor__y				=(((uint16_t *)&ram_buffer[ram_wr_start+4]));
volatile uint16_t  *hall_sensor__z				=(((uint16_t *)&ram_buffer[ram_wr_start+4])+1);

volatile uint16_t  *barometer_sensor__value		=(((uint16_t *)&ram_buffer[ram_wr_start+5]));
volatile uint16_t  *barometer_temp__value		=(((uint16_t *)&ram_buffer[ram_wr_start+5])+1); ///Needs to be added to xml etc

//read
volatile	uint8_t *mu_controller__mode		=(((uint8_t *)&ram_buffer[ram_rd_start]));	
volatile	uint8_t *sensor__mode				=(((uint8_t *)&ram_buffer[ram_rd_start])+1);
volatile uint8_t usart_en = 0; // 0 without USART 1 with USART


static void TIMER_1_read_sensor(const struct timer_task *const timer_task)
{
	switch (*sensor__status){
		case 0b00010000: //resistive
			*resistive_sensor__value	= readPR();
			break;
		case 0b10010000:
			readPR_USART();
			break;
		case 0b00001000: //PE
			*piezoelectric_sensor__value = readPE();
			break;
		case 0b10001000:
			readPE_USART();
			break;
		case 0b00000100: //cap
			*capacitive_sensor__x_p = readCap_xp();
			*capacitive_sensor__x_n = readCap_xn();
			*capacitive_sensor__y_p = readCap_yp();
			*capacitive_sensor__y_n = readCap_yn();
			break;
		case 0b10000100:
			readCap_USART();
			break;
		case 0b00000010: //hall
			readHall();
			*hall_sensor__x	= getHall_x();
			*hall_sensor__y	= getHall_y();
			*hall_sensor__z	= getHall_z();
			//*hall_sensor__temp	= readHall_temp(); TBD
			break;
		case 0b10000010:
			readHall_USART();
			break;
		case 0b10000001: //Baro
			readBaro_USART();
			break;
		case 0b00000001:
			*barometer_sensor__value = readBaro();
			*barometer_temp__value = getBaroTemp();
			break;
		default:
			*mu_controller__status = 0xF;
			break;
	}
}

static void TIMER_1_counter(const struct timer_task *const timer_task)
{
	counter=counter+1;	
	if(counter>100){
			counter=1;			
	};
	*mu_controller__status=counter;
}

void USART_EDBG_init(void)
{
	usart_sync_get_io_descriptor(&USART_0, &USART_io);
	usart_sync_enable(&USART_0);
}


void TIMER_1ms_init(void)
{
	TIMER_task1.interval = 2;
	TIMER_task1.cb       = TIMER_1_read_sensor;
	TIMER_task1.mode     = TIMER_TASK_REPEAT; //TIMER_TASK_ONE_SHOT;

	TIMER_task2.interval = 10;
	TIMER_task2.cb       = TIMER_1_counter;
	TIMER_task2.mode     = TIMER_TASK_REPEAT; //TIMER_TASK_ONE_SHOT;

	timer_add_task(&TIMER_1, &TIMER_task1);
	timer_add_task(&TIMER_1, &TIMER_task2);
	timer_start(&TIMER_1);
}

int main(void)
{
	/* Initialize
	 MCU, drivers and middleware */
	*sensor__status=0b10000010; //INITIALIZE FOR (USART_en,free,free,res,PE,cap,hall,baro)
	
	atmel_start_init();
	usart_en = *sensor__status >>7;
	if (usart_en == 0x01){
		USART_EDBG_init();
	}
	else{
		configure_ethercat_dma_descriptors();
	}
	switch (*sensor__status){
		case 0b00010000: //resistive
		case 0b10010000:
		  InitPR();
		  break;
		case 0b00001000: //PE
		case 0b10001000:
		  InitPE();
		  break;
		case 0b00000100: //cap
		case 0b10000100:
			initCap();
			break;
		case 0b00000010: //hall
		case 0b10000010:
			initHall();
		break;
		case 0b00000001: //Baro
		case 0b10000001:
		initBaro();
		break;
		default:
		  *mu_controller__status = 0xF;
		  break;
	}
	
	TIMER_1ms_init();
	
	
while (1) {
	 
		}
}

