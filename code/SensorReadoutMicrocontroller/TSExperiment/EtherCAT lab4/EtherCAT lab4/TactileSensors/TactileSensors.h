/*
 * IncFile1.h
 *
 * Created: 08.02.2022 17:23:26
 *  Author: Sonja Gross
 */ 

#ifndef INCFILE1_H_
#define INCFILE1_H_

//I2C Init for All
struct io_descriptor *USART_io;
struct io_descriptor *I2C_io;
struct io_descriptor *SPI_io;

uint8_t buffer[2];
uint8_t Data_Streamer_buf_2[4] = {0x03,0x00,0x00,0xFC};
uint8_t Data_Streamer_buf_8[10] = {0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC};
uint8_t Data_Streamer_buf_12[12] = {0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC};
uint16_t ADC_result = {0};
	
uint8_t buffer_Read_reg[3] = {0,0,0};
uint8_t buffer_Write_reg[3] = {0,0,0};

	

//I2C Init for PE
uint8_t MCP_addr = {0x2E};
uint8_t i2c_read_V0 = {0x0C};
uint8_t i2c_read_V1 = {0x1C};


uint8_t i2c_write_V0[2] = {0x00, 0xFF};
//uint8_t i2c_write_V1[2] = {0x10,0x20};
//-------------------------------------------------
//----------Init Barometer
struct bmp280_dev bmp;
int8_t bar_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
int8_t bar_reg_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
void print_rslt(const char api_name[], int8_t rslt);

uint8_t chipID = {0};
int8_t rslt;
struct bmp280_dev bmp;
struct bmp280_config conf;
struct bmp280_uncomp_data ucomp_data;
uint32_t pres32, pres64;
double pres;
int32_t temp32;
double temp;
uint8_t bar_read_chip_id = {0xD0};
uint8_t bar_read_temp = {0xFA};
uint8_t bar_read_pres = {0xF7};
uint8_t bar_read_ctrl_meas = {0xF4};
uint8_t bar_write_ctrl_meas[2] = {0x74,0x2F};//{0x74,0x10};
uint8_t bar_read_F4 = {0xF4};
uint8_t bar_reset[2] = {0x60,0xB6};
uint8_t bar_write_F4[2] = {0x74,0x00};
uint8_t bar_write_reset[2] = {0x00, 0xFF};
uint32_t result = {0};

uint8_t bar_pressure[4];
uint8_t bar_temperature[4];

//----------Init PR

//----------Init Hall
uint8_t buffer_Write_data = {0};
uint8_t buffer_Read_data[9] = {0,0,0,0,0,0,0,0,0};
uint8_t hall_status[2] = {0};
uint8_t hall_temp[2] = {0};
uint8_t hall_x[2] = {0};
uint8_t hall_y[2] = {0};
uint8_t hall_z[2] = {0};
uint16_t hall_x_16 = {0};
uint16_t hall_y_16 = {0};
uint16_t hall_z_16 = {0};
uint16_t hall_temp_16 = {0};
uint8_t hall_cmmd_measure = {0x3F}; // statxyzt
uint8_t hall_cmmd_exit = {0x80};
uint8_t hall_cmmd_read = {0x4F}; // statxyzt
uint8_t hall_cmmd_reset = {0xF0};
//uint8_t hall_cmmd_burst = {0x1F}; //x,y,z,t
//uint8_t spi_cmmd_burst = {0x11}; //Only t
uint8_t hall_read_reg_00[2] = {0x50,(0x00<<2)};
uint8_t hall_read_reg_01[2] = {0x50,(0x01<<2)};
uint8_t hall_read_reg_02[2] = {0x50,(0x02<<2)};
uint8_t hall_read_reg_03[2] = {0x50,(0x03<<2)};
uint8_t hall_read_reg_04[2] = {0x50,(0x04<<2)};
uint8_t hall_read_reg_05[2] = {0x50,(0x05<<2)};
uint8_t hall_read_reg_temp[2] = {0x50,(0x03<<2)};
uint8_t hall_read_reg_x[2] = {0x50,(0x04<<2)};
uint8_t hall_read_reg_y[2] = {0x50,(0x05<<2)};
uint8_t hall_read_reg_z[2] = {0x50,(0x06<<2)};
uint8_t hall_write_reg_01[4] = {0x60,0x44,0x00,(0x01<<2)};
uint8_t hall_write_reg_02[4] = {0x60,0xF1,0xE3,(0x02<<2)};

//----------Init Cap
uint8_t spi_read_reg80[2] = {0xE4,0x80};
uint8_t spi_read_reg81[2] = {0xE4,0x81};
uint8_t spi_read_reg82[2] = {0xE4,0x82};
uint8_t spi_read_reg83[2] = {0xE4,0x83};
uint8_t spi_read_reg84[2] = {0xE4,0x84};
uint8_t spi_read_reg85[2] = {0xE4,0x85};
uint8_t spi_read_reg86[2] = {0xE4,0x86};
uint8_t spi_read_reg87[2] = {0xE4,0x87};
uint8_t spi_read_reg88[2] = {0xE4,0x88};
uint8_t spi_read_reg89[2] = {0xE4,0x89};
uint8_t spi_read_reg90[2] = {0xE4,0x90};
uint8_t spi_read_reg91[2] = {0xE4,0x91};
uint8_t spi_read_reg00[2] = {0xE4,0x00};

uint8_t spi_read_S0_data[2] = {0xE4,0x0B};
uint8_t spi_read_S1_data[2] = {0xE4,0x0C};
uint8_t spi_read_S2_data[2] = {0xE4,0x0D};

uint8_t spi_write_reg80[4] = {0xE0,0x80,0x00,0x00};
uint8_t spi_write_reg81[4] = {0xE0,0x81,0x08,0x40};
uint8_t spi_write_reg88[4] = {0xE0,0x88,0x00,0x00};
uint8_t spi_write_reg89[4] = {0xE0,0x89,0x01,0x20};
uint8_t spi_write_reg90[4] = {0xE0,0x90,0x00,0x00};
uint8_t spi_write_reg91[4] = {0xE0,0x91,0x00,0x00};

uint8_t spi_write_reg00[4] = {0xE0,0x00,0x82,0xB2};
uint8_t spi_write_reg01[4] = {0xE0,0x01,0x00,0x00};
uint8_t spi_write_reg02[4] = {0xE0,0x02,0x32,0x30};
uint8_t spi_write_reg03[4] = {0xE0,0x03,0x04,0x19};
uint8_t spi_write_reg04[4] = {0xE0,0x04,0x08,0x32};
uint8_t spi_write_reg01_2[4] = {0xE0,0x01,0x0F,0xFF};

uint8_t	spi_write_reg82[4] = {0xE0,0x82,0x0F,0x0F};
uint8_t	spi_write_reg83[4] = {0xE0,0x83,0xFB,0xFE};
uint8_t	spi_write_reg8A[4] = {0xE0,0x8A,0x0F,0x0F};
uint8_t	spi_write_reg8B[4] = {0xE0,0x8B,0xFB,0xFE};
uint8_t	spi_write_reg92[4] = {0xE0,0x92,0x0F,0x0F};
uint8_t	spi_write_reg93[4] = {0xE0,0x93,0xFB,0xFE};

uint8_t reg81[2] = {0};
uint8_t S0_conv[2] = {0};
uint8_t S1_conv[2] = {0};
uint8_t S2_conv[2] = {0};
	
uint16_t x_p = {0};
uint16_t x_n = {0};
uint16_t y_p = {0};
uint16_t y_n = {0};
uint16_t cap_result[4] = {0,0,0,0}; 
	

//--------------------------------



void InitPE(void);
void initCap(void);
void InitPR(void);
void initBaro(void);
void InitHall(void);


uint16_t readPR(void);
void readPR_USART(void);

uint16_t readPE(void);
void readPE_USART(void);

void readCap_USART(void);
uint16_t readCap_xp(void);
uint16_t readCap_xn(void);
uint16_t readCap_yp(void);
uint16_t readCap_yn(void);

uint16_t readBaro(void);
void readBaro_USART(void);
uint16_t getBaroTemp(void);

void readHall_USART(void);
void readHall(void);
uint16_t getHall_x(void);
uint16_t getHall_y(void);
uint16_t getHall_z(void);
uint16_t getHall_temp(void);


#endif /* INCFILE1_H_ */