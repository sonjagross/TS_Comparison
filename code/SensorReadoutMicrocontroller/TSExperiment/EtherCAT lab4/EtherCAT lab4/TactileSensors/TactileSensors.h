/*
 * IncFile1.h
 *
 * Created: 08.02.2022 17:23:26
 *  Author: Sonja Gross
 */ 

#ifndef TactileSensors_H_
#define TactileSensors_H_

//I2C Init for All
struct io_descriptor *USART_io;
struct io_descriptor *I2C_io;
struct io_descriptor *SPI_io;

uint8_t buffer[2];
uint8_t Data_Streamer_buf_2[4] = {0x01,0x00,0x00,0xFE};
uint8_t Data_Streamer_buf_8[10] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE};
uint8_t Data_Streamer_buf_12[12] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE};
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
uint8_t bar_write_ctrl_meas[2] = {0x74,0x27};//{0x74,0x10};2F
uint8_t bar_read_F4 = {0xF4};
uint8_t bar_reset[2] = {0x60,0xB6};
uint8_t bar_write_F4[2] = {0x74,0x00};
uint8_t bar_write_reset[2] = {0x00, 0xFF};
uint32_t result = {0};
uint16_t bar_shift = 2800;
uint8_t bar_pressure[4];
uint8_t bar_temperature[4];

//----------Init PR



//----------Init Cap
uint8_t spi_read_S0CONF0[2] = {0xE4,0x80};
uint8_t spi_read_S0CONF1[2] = {0xE4,0x81};
uint8_t spi_read_S1CONF0[2] = {0xE4,0x88};
uint8_t spi_read_S1CONF1[2] = {0xE4,0x89};
uint8_t spi_read_S2CONF0[2] = {0xE4,0x90};
uint8_t spi_read_S2CONF1[2] = {0xE4,0x91};
uint8_t spi_read_S3CONF0[2] = {0xE4,0x98};
uint8_t spi_read_S3CONF1[2] = {0xE4,0x99};
uint8_t spi_read_SETUPCONTR[2] = {0xE4,0x00};

uint8_t spi_read_S0_data[2] = {0xE4,0x0B};
uint8_t spi_read_S1_data[2] = {0xE4,0x0C};
uint8_t spi_read_S2_data[2] = {0xE4,0x0D};
uint8_t spi_read_S3_data[2] = {0xE4,0x0E};

// For SETUPCONTR register
uint8_t cap_POWERMODE = 0b00; //Bit 1:0
uint8_t cap_LP_CONV_DELAY = 0b00; //Bit 3:2
uint8_t cap_SEQUENCESTAGENUM = 3; //Bit 7:4
uint8_t cap_DECIMATION = 0b11; //Bit 9:8
uint8_t cap_SW_RESET = 0b0; //Bit 9:8

// connections setup
uint8_t spi_write_S0CONF0[4] = {0xE0,0x80,0xFF,0xFF};
uint8_t spi_write_S0CONF1[4] = {0xE0,0x81,0x1B,0xFF};
uint8_t spi_write_S1CONF0[4] = {0xE0,0x88,0xFF,0xFF};
uint8_t spi_write_S1CONF1[4] = {0xE0,0x89,0x1E,0xFF};
uint8_t spi_write_S2CONF0[4] = {0xE0,0x90,0xFF,0xFF};
uint8_t spi_write_S2CONF1[4] = {0xE0,0x91,0x10,0xBF};
uint8_t spi_write_S3CONF0[4] = {0xE0,0x98,0xFF,0xFF};
uint8_t spi_write_S3CONF1[4] = {0xE0,0x99,0x10,0xEF};

uint8_t spi_write_SETUPCONTR[4] = {0xE0,0x00,0x83,0x30};
uint8_t spi_write_CALNSET0[4] = {0xE0,0x01,0x00,0x00};
uint8_t spi_write_CALNSET1[4] = {0xE0,0x02,0x32,0x30};
uint8_t spi_write_CALNSET2[4] = {0xE0,0x03,0x04,0x19};
uint8_t spi_write_CALNSET3[4] = {0xE0,0x04,0x08,0x32};
uint8_t spi_write_CALNSET0_2[4] = {0xE0,0x01,0x0F,0xFF};

// sensitivity and Offset
uint8_t	spi_write_S0CONF2[4] = {0xE0,0x82,0x00,0x8F}; //Offset
uint8_t	spi_write_S0CONF3[4] = {0xE0,0x83,0x0F,0x0F}; //Sensitivity
uint8_t	spi_write_S2CONF2[4] = {0xE0,0x8A,0x00,0x8F};
uint8_t	spi_write_S1CONF3[4] = {0xE0,0x8B,0x0F,0x0F};
//uint8_t	spi_write_S2CONF2[4] = {0xE0,0x92,0x00,0x8F};
uint8_t	spi_write_S2CONF3[4] = {0xE0,0x93,0x0F,0x0F};
uint8_t	spi_write_S3CONF2[4] = {0xE0,0x9A,0x00,0x8F};
//uint8_t	spi_write_S0CONF3[4] = {0xE0,0x9B,0x0F,0x0F};

uint8_t reg81[2] = {0};
uint8_t S0_conv[2] = {0};
uint8_t S1_conv[2] = {0};
uint8_t S2_conv[2] = {0};
uint8_t S3_conv[2] = {0};
	
uint16_t x_p = {0};
uint16_t x_n = {0};
uint16_t y_p = {0};
uint16_t y_n = {0};
uint16_t cap_result[4] = {0,0,0,0}; 

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

uint8_t hall_ZSER = 0b0; //Bit 7
uint8_t hall_GAINSEL = 0b000; //Bit 6:4
uint8_t hall_HALLCONF = 0b0000; //1100 //Bit 3:0
uint8_t hall_BURSTSEL_LSB = 0b00;
uint8_t hall_BURSTSEL_MSB = 0b00;
uint8_t hall_BURSTDATARATE = 0b000000;
uint8_t hall_TRIGINT = 0b0;
uint8_t hall_COMMMODE = 0b10;
uint8_t hall_WOCDIFF = 0b0;
uint8_t hall_EXTTRIG = 0b0;
uint8_t hall_TCMPEN = 0b1;
uint8_t hall_RESY_LSB = 0b0;
uint8_t hall_RESY_MSB = 0b0;
uint8_t hall_RESX = 0b00;
uint8_t hall_RESZ = 0b00;
uint8_t hall_DIFFILT = 0b000;//011
uint8_t hall_OSR = 0b00;//01
uint8_t hall_OSR2 = 0b10;

uint16_t hall_OFFSETX = 0x0000;
uint16_t hall_OFFSETY = 0x0000;
uint16_t hall_OFFSETZ = 0x0000;
uint16_t hall_WOXYTHRESHOLD = 0x0000;
uint16_t hall_WOZTHRESHOLD = 0x0000;
uint16_t hall_WOTTHRESHOLD = 0x0000;

void InitPE(void);
void initCap(void);
void InitPR(void);
void initBaro(void);
void initHall();


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