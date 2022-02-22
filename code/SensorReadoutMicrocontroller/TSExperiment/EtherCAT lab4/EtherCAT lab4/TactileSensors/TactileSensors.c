/*
 * TactileSensors.c
 *
 * Created: 08.02.2022 17:23:09
 *  Author: Sonja Gross
 */ 
#include "atmel_start.h"
#include "driver_init.h"
#include "TactileSensors.h"
#include "stdio.h"
#include "BMP280/bmp280.h"


void initBaro(void)
{
	spi_m_sync_get_io_descriptor(&SPI_1, &SPI_io);
	spi_m_sync_enable(&SPI_1);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&bar_read_chip_id,1);
	io_read(SPI_io,&buffer_Read_reg,1);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	chipID = buffer_Read_reg[0];
	
	bmp.dev_id = TS_SPI_SS_1;
	bmp.read = bar_reg_read;
	bmp.write = bar_reg_write;
	bmp.intf = BMP280_SPI_INTF;
	
    gpio_set_pin_level(TS_SPI_SS_1,false);
    io_write(SPI_io,&bar_reset,2);
    gpio_set_pin_level(TS_SPI_SS_1,true);
		
    /* Map the delay function pointer with the function responsible for implementing the delay */
    bmp.delay_ms = delay_ms;
	
    rslt = bmp280_init(&bmp);
	
	 /* Always read the current settings before writing, especially when
     * all the configuration is not modified
     */
    rslt = bmp280_get_config(&conf, &bmp);

    /* configuring the temperature oversampling, filter coefficient and output data rate */
    /* Overwrite the desired settings */
    conf.filter = BMP280_FILTER_OFF;

    /* Temperature oversampling set at 4x */
    conf.os_temp = BMP280_OS_16X;

    /* Pressure over sampling none (disabling pressure measurement) */
    conf.os_pres = BMP280_OS_16X;

    /* Setting the output data rate as 1HZ(1000ms) */
    conf.odr = BMP280_ODR_1000_MS;
    //rslt = bmp280_set_config(&conf, &bmp);
   // print_rslt(" bmp280_set_config status", rslt);

    /* Always set the power mode after setting the configuration */
    rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp);
    //print_rslt(" bmp280_set_power_mode status", rslt);
	
	       gpio_set_pin_level(TS_SPI_SS_1,false);
	       io_write(SPI_io,&bar_reset,2);
	       gpio_set_pin_level(TS_SPI_SS_1,true);

	       gpio_set_pin_level(TS_SPI_SS_1,false);
	       io_write(SPI_io,&bar_read_ctrl_meas,1);
	       io_read(SPI_io,&buffer_Read_reg,1);
	       gpio_set_pin_level(TS_SPI_SS_1,true);

       	gpio_set_pin_level(TS_SPI_SS_1,false);
       	io_write(SPI_io,&bar_write_ctrl_meas,2);
       	gpio_set_pin_level(TS_SPI_SS_1,true);
       	
       	gpio_set_pin_level(TS_SPI_SS_1,false);
       	io_write(SPI_io,&bar_read_ctrl_meas,1);
       	io_read(SPI_io,&buffer_Read_reg,1);
       	gpio_set_pin_level(TS_SPI_SS_1,true);
}

uint16_t getBaroTemp(void) ///Only for Data Vizualizer
{
	return temp32;
}

uint16_t readBaro(void) ///Only for Data Vizualizer
{
/* Reading the raw data from sensor */
rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);

/* Getting the compensated pressure using 32 bit precision */
rslt = bmp280_get_comp_pres_32bit(&pres32, ucomp_data.uncomp_press, &bmp);

/* Getting the compensated pressure using 64 bit precision */
rslt = bmp280_get_comp_pres_64bit(&pres64, ucomp_data.uncomp_press, &bmp);

/* Getting the compensated pressure as floating point value */
rslt = bmp280_get_comp_pres_double(&pres, ucomp_data.uncomp_press, &bmp);

/* Getting the 32 bit compensated temperature */
rslt = bmp280_get_comp_temp_32bit(&temp32, ucomp_data.uncomp_temp, &bmp);

/* Getting the compensated temperature as floating point value */
//rslt = bmp280_get_comp_temp_double(&temp, ucomp_data.uncomp_temp, &bmp);

bar_pressure[0] = (pres32 & 0x000000ff);
bar_pressure[1] = (pres32 & 0x0000ff00) >> 8;
bar_pressure[2] = (pres32 & 0x00ff0000) >> 16;
bar_pressure[3] = (pres32 & 0xff000000) >> 24;
bar_temperature[0] = (temp32 & 0x000000ff);
bar_temperature[1] = (temp32 & 0x0000ff00) >> 8;
bar_temperature[2] = (temp32 & 0x00ff0000) >> 16;
bar_temperature[3] = (temp32 & 0xff000000) >> 24;

//bar_pressure[0] = (ucomp_data.uncomp_press & 0x000000ff);
//bar_pressure[1] = (ucomp_data.uncomp_press & 0x0000ff00) >> 8;
//bar_pressure[2] = (ucomp_data.uncomp_press & 0x00ff0000) >> 16;
//bar_pressure[3] = (ucomp_data.uncomp_press & 0xff000000) >> 24;
//bar_temperature[0] = (ucomp_data.uncomp_temp & 0x000000ff);
//bar_temperature[1] = (ucomp_data.uncomp_temp & 0x0000ff00) >> 8;
//bar_temperature[2] = (ucomp_data.uncomp_temp & 0x00ff0000) >> 16;
//bar_temperature[3] = (ucomp_data.uncomp_temp & 0xff000000) >> 24;

return (uint16_t*)pres32;
}

void readBaro_USART(void) ///Only for Data Vizualizer
{
	/* Reading the raw data from sensor */
	rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp);

	/* Getting the compensated pressure using 32 bit precision */
	rslt = bmp280_get_comp_pres_32bit(&pres32, ucomp_data.uncomp_press, &bmp);

	/* Getting the compensated pressure using 64 bit precision */
	rslt = bmp280_get_comp_pres_64bit(&pres64, ucomp_data.uncomp_press, &bmp);

	/* Getting the compensated pressure as floating point value */
	rslt = bmp280_get_comp_pres_double(&pres, ucomp_data.uncomp_press, &bmp);

	/* Getting the 32 bit compensated temperature */
	rslt = bmp280_get_comp_temp_32bit(&temp32, ucomp_data.uncomp_temp, &bmp);

	/* Getting the compensated temperature as floating point value */
	//rslt = bmp280_get_comp_temp_double(&temp, ucomp_data.uncomp_temp, &bmp);

	//io_write(USART_io,&ucomp_data.uncomp_temp,8);

	bar_pressure[0] = (pres32 & 0x000000ff);
	bar_pressure[1] = (pres32 & 0x0000ff00) >> 8;
	bar_pressure[2] = (pres32 & 0x00ff0000) >> 16;
	bar_pressure[3] = (pres32 & 0xff000000) >> 24;
	bar_temperature[0] = (temp32 & 0x000000ff);
	bar_temperature[1] = (temp32 & 0x0000ff00) >> 8;
	bar_temperature[2] = (temp32 & 0x00ff0000) >> 16;
	bar_temperature[3] = (temp32 & 0xff000000) >> 24;

	//bar_pressure[0] = (ucomp_data.uncomp_press & 0x000000ff);
	//bar_pressure[1] = (ucomp_data.uncomp_press & 0x0000ff00) >> 8;
	//bar_pressure[2] = (ucomp_data.uncomp_press & 0x00ff0000) >> 16;
	//bar_pressure[3] = (ucomp_data.uncomp_press & 0xff000000) >> 24;
	//bar_temperature[0] = (ucomp_data.uncomp_temp & 0x000000ff);
	//bar_temperature[1] = (ucomp_data.uncomp_temp & 0x0000ff00) >> 8;
	//bar_temperature[2] = (ucomp_data.uncomp_temp & 0x00ff0000) >> 16;
	//bar_temperature[3] = (ucomp_data.uncomp_temp & 0xff000000) >> 24;

	Data_Streamer_buf_8[1] = bar_pressure[0];
	Data_Streamer_buf_8[2] = bar_pressure[1];
	Data_Streamer_buf_8[3] = bar_pressure[2];
	Data_Streamer_buf_8[4] = bar_pressure[3];
	Data_Streamer_buf_8[5] = bar_temperature[0];
	Data_Streamer_buf_8[6] = bar_temperature[1];
	Data_Streamer_buf_8[7] = bar_temperature[2];
	Data_Streamer_buf_8[8] = bar_temperature[3];
	
	io_write(USART_io,&Data_Streamer_buf_8,10);
}

int8_t bar_reg_write(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
	cs = TS_SPI_SS_1;
	
	gpio_set_pin_level(cs,false);
	io_write(SPI_io,&reg_addr,1);
	io_write(SPI_io,&reg_data,length);
	gpio_set_pin_level(cs,true);
    return 0;
}

/*!
 *  @brief Function for reading the sensor's registers through SPI bus.
 *
 *  @param[in] cs       : Chip select to enable the sensor.
 *  @param[in] reg_addr : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] length   : No of bytes to read.
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval >0 -> Failure Info
 *
 */
int8_t bar_reg_read(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
	
	cs = TS_SPI_SS_1;
	//io_write(USART_io, &reg_addr, 1);
	
    gpio_set_pin_level(cs,false);
	io_write(SPI_io,&reg_addr,1);
	io_read(SPI_io,reg_data,length);
	gpio_set_pin_level(cs,true);
	
	//io_write(USART_io, &reg_data, length);
	
    return 0;
}

void initHall(void)
{
	spi_m_sync_get_io_descriptor(&SPI_1, &SPI_io);
	spi_m_sync_enable(&SPI_1);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&hall_cmmd_exit,1);
	io_read(SPI_io,&buffer_Read_data,1);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&hall_cmmd_reset,1);
	io_read(SPI_io,&buffer_Read_data,1);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&hall_read_reg_02,2);
	io_read(SPI_io,&buffer_Read_data,3);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&hall_write_reg_02,4);
	io_read(SPI_io,&buffer_Read_data,1);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&hall_read_reg_02,2);
	io_read(SPI_io,&buffer_Read_data,3);
	gpio_set_pin_level(TS_SPI_SS_1,true);

		gpio_set_pin_level(TS_SPI_SS_1,false);
		io_write(SPI_io,&hall_read_reg_01,2);
		io_read(SPI_io,&buffer_Read_data,3);
		gpio_set_pin_level(TS_SPI_SS_1,true);

		gpio_set_pin_level(TS_SPI_SS_1,false);
		io_write(SPI_io,&hall_write_reg_01,4);
		io_read(SPI_io,&buffer_Read_data,3);
		gpio_set_pin_level(TS_SPI_SS_1,true);

		gpio_set_pin_level(TS_SPI_SS_1,false);
		io_write(SPI_io,&hall_read_reg_01,2);
		io_read(SPI_io,&buffer_Read_data,3);
		gpio_set_pin_level(TS_SPI_SS_1,true);
}

void readHall_USART(void) ///Only for Data Vizualizer
{
	//io_write(USART_io,&hall_cmmd_measure, 1);
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&hall_cmmd_measure,1);
	io_read(SPI_io,&buffer_Read_data,1);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	hall_status[0] = buffer_Read_data[0];
	
	//io_write(USART_io, &buffer_Read_data, 1);
	delay_ms(5);
	
	//io_write(USART_io,&spi_cmmd_read, 1);
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&hall_cmmd_read,1);
	io_read(SPI_io,&buffer_Read_data,9);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	//io_write(USART_io, &hall_Read_data, 9);

	
	hall_status[0] = buffer_Read_data[0];
	
	hall_temp[0] = buffer_Read_data[2];
	hall_temp[1] = buffer_Read_data[1];
	
	hall_x[0] = buffer_Read_data[4];
	hall_x[1] = buffer_Read_data[3];
	
	hall_y[0] = buffer_Read_data[6];
	hall_y[1] = buffer_Read_data[5];
	
	hall_z[0] = buffer_Read_data[8];
	hall_z[1] = buffer_Read_data[7];
	
	Data_Streamer_buf_12[1] = hall_status[0];
	Data_Streamer_buf_12[2] = hall_status[0];
	Data_Streamer_buf_12[3] = hall_temp[0];
	Data_Streamer_buf_12[4] = hall_temp[1];
	Data_Streamer_buf_12[5] = hall_x[0];
	Data_Streamer_buf_12[6] = hall_x[1];
	Data_Streamer_buf_12[7] = hall_y[0];
	Data_Streamer_buf_12[8] = hall_y[1];
	Data_Streamer_buf_12[9] = hall_z[0];
	Data_Streamer_buf_12[10] = hall_z[1];
	
	io_write(USART_io, Data_Streamer_buf_12, 12);
}

void readHall(void) ///Only for Data Vizualizer
{
	//io_write(USART_io,&hall_cmmd_measure, 1);
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&hall_cmmd_measure,1);
	io_read(SPI_io,&buffer_Read_data,1);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	delay_ms(2);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&hall_cmmd_read,1);
	io_read(SPI_io,&buffer_Read_data,9);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	hall_x_16 = ((uint16_t)buffer_Read_data[3]) << 8 | ((uint16_t)buffer_Read_data[4]);
	
	hall_y_16 = ((uint16_t)buffer_Read_data[5]) << 8 | ((uint16_t)buffer_Read_data[6]);
	
	hall_z_16 = ((uint16_t)buffer_Read_data[7]) << 8 | ((uint16_t)buffer_Read_data[8]);
	
	hall_temp_16 = ((uint16_t)buffer_Read_data[1]) << 8 | ((uint16_t)buffer_Read_data[2]);

}

uint16_t getHall_z(void)
{
	return hall_z_16;
}

uint16_t getHall_x(void)
{
return hall_x_16;
}

uint16_t getHall_y(void)
{
return hall_y_16;
}

uint16_t getHall_temp(void)
{
return hall_temp_16;
}

void initCap(void)
{
	spi_m_sync_get_io_descriptor(&SPI_1, &SPI_io);
	spi_m_sync_enable(&SPI_1);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg80,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_read_reg80,2);
	io_read(SPI_io,&buffer,2);
	gpio_set_pin_level(TS_SPI_SS_1,true);

	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg81,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_read_reg81,2);
	io_read(SPI_io,&buffer,2);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg82,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);

	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg83,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg88,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg89,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg8A,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg8B,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);

	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg90,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg91,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg92,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg93,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg00,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg01,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg02,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg03,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg04,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_write_reg01_2,4);
	gpio_set_pin_level(TS_SPI_SS_1,true);
}

void readCap_USART(void) ///Only for Data Vizualizer
{
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_read_reg88,2);
	io_read(SPI_io,&buffer,2);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	reg81[0] = buffer[0];
	reg81[1] = buffer[1];
	x_p =((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_read_reg81,2);
	io_read(SPI_io,&buffer,2);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	reg81[0] = buffer[0];
	reg81[1] = buffer[1];
	x_p =((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);
	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_read_S0_data,2);
	io_read(SPI_io,&buffer,2);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	S0_conv[0] = buffer[0];
	S0_conv[1] = buffer[1];
	x_n =((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);

	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_read_S1_data,2);
	io_read(SPI_io,&buffer,2);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	S1_conv[0] = buffer[0];
	S1_conv[1] = buffer[1];	
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_read_S2_data,2);
	io_read(SPI_io,&buffer,2);
	gpio_set_pin_level(TS_SPI_SS_1,true);
	
	S2_conv[0] = buffer[0];
	S2_conv[1] = buffer[1];
	y_p =((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);

	Data_Streamer_buf_8[1] = reg81[1];
	Data_Streamer_buf_8[2] = reg81[0];
	Data_Streamer_buf_8[3] = S0_conv[1];
	Data_Streamer_buf_8[4] = S0_conv[0];
	Data_Streamer_buf_8[5] = S1_conv[1];
	Data_Streamer_buf_8[6] = S1_conv[0];
	Data_Streamer_buf_8[7] = S2_conv[1];
	Data_Streamer_buf_8[8] = S2_conv[0];
	io_write(USART_io,&Data_Streamer_buf_8,10);
}

uint16_t readCap_xp(void)
{
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_read_S1_data,2);
	io_read(SPI_io,&buffer,2);
	gpio_set_pin_level(TS_SPI_SS_1,true);

	x_p =((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);
	
	return x_p;
}

uint16_t readCap_xn(void)
{
	//gpio_set_pin_level(TS_SPI_SS_1,false);
	//io_write(SPI_io,&spi_read_S0_data,2);
	//io_read(SPI_io,&buffer,2);
	//gpio_set_pin_level(TS_SPI_SS_1,true);
	
	//x_n =((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);
	x_n = 255;
	return x_n;
}

uint16_t readCap_yp(void)
{
	gpio_set_pin_level(TS_SPI_SS_1,false);
	io_write(SPI_io,&spi_read_S0_data,2);
	io_read(SPI_io,&buffer,2);
	gpio_set_pin_level(TS_SPI_SS_1,true);

	y_p =((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);
	return y_p;
}

uint16_t readCap_yn(void)
{
	//gpio_set_pin_level(TS_SPI_SS_1,false);
	//io_write(SPI_io,&spi_read_S2_data,2);
	//io_read(SPI_io,&buffer,2);
	//gpio_set_pin_level(TS_SPI_SS_1,true);

	//y_n =((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);
	y_n = 255;
	return y_n;
}

void InitPR(void)
{
	ADC_MCP_Init();
}


uint16_t readPR(void)
{
	ADC_MCP_Init();
	adc_async_start_conversion(&ADC_0);
	adc_async_read_channel(&ADC_0,0,buffer,2);

	ADC_result=((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);
	
	return ADC_result;
}

void readPR_USART(void)
{
	ADC_MCP_Init();
	adc_async_start_conversion(&ADC_0);
	adc_async_read_channel(&ADC_0,0,buffer,2);
	
	Data_Streamer_buf_2[1] = buffer[0];
	Data_Streamer_buf_2[2] = buffer[1]+0x7F;
	io_write(USART_io, &Data_Streamer_buf_2, 4); //PROBLEMATIC CAUSES ERROR IN ETHERCAT TRANSMISSION
}

void InitPE(void)
{
	ADC_MCP_Init();
	
	i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_io);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, MCP_addr, I2C_M_SEVEN);
	
	io_write(I2C_io, i2c_write_V0,2);
	i2c_m_sync_cmd_read(&I2C_0, i2c_read_V0, &buffer, 2);
}

uint16_t readPE(void)
{
	adc_async_start_conversion(&ADC_0);
	adc_async_read_channel(&ADC_0,0,buffer,2);
	
	ADC_result=((uint16_t)buffer[1]) << 8 | ((uint16_t)buffer[0]);
	return ADC_result;
}
void readPE_USART(void)
{
	adc_async_start_conversion(&ADC_0);
	adc_async_read_channel(&ADC_0,0,buffer,2);
	
	Data_Streamer_buf_2[1] = buffer[0];
	Data_Streamer_buf_2[2] = buffer[1]+0x7F;
	io_write(USART_io, &Data_Streamer_buf_2, 4);
}


static void convert_cb_ADC(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

/**
 * Example of using ADC_0 to generate waveform.
 */

void ADC_MCP_Init(void)
{
	adc_async_register_callback(&ADC_0, 0, ADC_ASYNC_CONVERT_CB, convert_cb_ADC);
	adc_async_enable_channel(&ADC_0, 0);
	adc_async_start_conversion(&ADC_0);
}