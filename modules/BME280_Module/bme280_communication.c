/*
 * bme280_communication.c
 *
 *  Created on: 24.02.2022
 *      Author: Omar Alhammoud, Franck Brondon Mezatio Felefack
  *
  *@ das Programm läuft noch nicht, er geht immer in eine endlos-Schleife
 * *      @verbatim

 =============================================================================================
 	 	 	 	 	 	 #### Ressources used ####

  (#) GPIOB => GPIO_PIN_8 und 9, i2c1
 =============================================================================================
						#### Usage: ####
	...
 =============================================================================================
 @endverbatim
 *********************************************************************************************
 */
#include "stm32f4xx.h"
#include "bme280_communication.h"
#include<BME280_driver/bme280.h>
#include<BME280_driver/bme280_defs.h>


/******************************************************************************/
/*!                               Structures                                  */

/* Structure that contains identifier details used in example */
struct identifier
{
    /* Variable to hold device address */
    uint8_t dev_addr;

    /* Variable that contains file descriptor */
    int8_t fd;
};

/****************************************************************************/
/*!                         Functions                                       */

/*!
 *  @brief Function that creates a mandatory delay required in some of the APIs.
 *
 * @param[in] period              : Delay in microseconds.
 * @param[in, out] intf_ptr       : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *  @return void.
 *
 */
void user_delay_us(uint32_t period, void *intf_ptr);

/*!
 * @brief Function for print the temperature, humidity and pressure data.
 *
 * @param[out] comp_data    :   Structure instance of bme280_data
 *
 * @note Sensor data whose can be read
 *
 * sens_list
 * --------------
 * Pressure
 * Temperature
 * Humidity
 *
 */
void print_sensor_data(struct bme280_data *comp_data);

/*!
 *  @brief Function for reading the sensor's registers through I2C bus.
 *
 *  @param[in] reg_addr       : Register address.
 *  @param[out] data          : Pointer to the data buffer to store the read data.
 *  @param[in] len            : No of bytes to read.
 *  @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs.
 *
 *  @return Status of execution
 *
 *  @retval 0 -> Success
 *  @retval > 0 -> Failure Info
 *
 */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);

/*!
 *  @brief Function for writing the sensor's registers through I2C bus.
 *
 *  @param[in] reg_addr       : Register address.
 *  @param[in] data           : Pointer to the data buffer whose value is to be written.
 *  @param[in] len            : No of bytes to write.
 *  @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *
 *  @return Status of execution
 *
 *  @retval BME280_OK -> Success
 *  @retval BME280_E_COMM_FAIL -> Communication failure.
 *
 */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

/*!
 * @brief Function reads temperature, humidity and pressure data in forced mode.
 *
 * @param[in] dev   :   Structure instance of bme280_dev.
 *
 * @return Result of API execution status
 *
 * @retval BME280_OK - Success.
 * @retval BME280_E_NULL_PTR - Error: Null pointer error
 * @retval BME280_E_COMM_FAIL - Error: Communication fail error
 * @retval BME280_E_NVM_COPY_FAILED - Error: NVM copy failed
 *
 */
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev);

I2C_HandleTypeDef  i2c1_handle_struct;

void i2c1_gpio_init(void);
void i2c1_initialize(void);

void i2c1_init(void){
	i2c1_gpio_init();

	__HAL_RCC_FMPI2C1_CLK_ENABLE();

	i2c1_initialize();

}


void i2c1_gpio_init(void){
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_init_structure;
	GPIO_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_init_structure.Mode = GPIO_MODE_AF_OD;
	GPIO_init_structure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_init_structure);
}

void i2c1_initialize(void){
	i2c1_handle_struct.Instance = I2C1;
	i2c1_handle_struct.Init.ClockSpeed = 100000;
	i2c1_handle_struct.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c1_handle_struct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c1_handle_struct.Init.DutyCycle = I2C_DUTYCYCLE_2 ;
	i2c1_handle_struct.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c1_handle_struct.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2c1_handle_struct);
}

/******************************************************************************/
/*!                         Own header files                                  */

/*!
 * @brief This function starts execution of the program.
 */
void bme280_dev_init(void){
    struct bme280_dev dev;

    struct identifier id;

    /* Variable to define the result */
    int8_t rslt = BME280_OK;

    /* Make sure to select BME280_I2C_ADDR_PRIM or BME280_I2C_ADDR_SEC as needed */
    id.dev_addr = BME280_I2C_ADDR_PRIM;

    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    /* Update interface pointer with the structure that contains both device address and file descriptor */
    dev.intf_ptr = &id;

    /* Initialize the bme280 */
    rslt = bme280_init(&dev);
    if (rslt != BME280_OK)
    {
      //  fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
        exit(1);
    }

    rslt = stream_sensor_data_forced_mode(&dev);
    if (rslt != BME280_OK)
    {
      //  fprintf(stderr, "Failed to stream sensor data (code %+d).\n", rslt);
        exit(1);
    }

    return 0;
}


int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
	uint16_t dev_addr = *((uint16_t*)intf_ptr);

	HAL_StatusTypeDef rslt;
	rslt = HAL_I2C_Master_Transmit(&i2c1_handle_struct, dev_addr << 1, &reg_addr, 1, HAL_MAX_DELAY);
	if(rslt != HAL_OK)
		return 1;
	rslt = HAL_I2C_Master_Receive(&i2c1_handle_struct, (dev_addr << 1) | 1, data, len, HAL_MAX_DELAY);
	if(rslt != HAL_OK)
		return 1;
   return 0;
}

/*!
* @brief This function provides the delay for required time (Microseconds) as per the input provided in some of the
* APIs
*/
void user_delay_us(uint32_t period, void *intf_ptr)
{
	HAL_Delay(period);
}

/*!
* @brief This function for writing the sensor's registers through I2C bus.
*/
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
	HAL_StatusTypeDef rslt;
	rslt = HAL_I2C_Master_Transmit(&i2c1_handle_struct, BME280_I2C_ADDR_PRIM << 1, &reg_addr, 1, HAL_MAX_DELAY);
	if(rslt != HAL_OK)
		return 1;
	rslt = HAL_I2C_Master_Transmit(&i2c1_handle_struct, BME280_I2C_ADDR_PRIM << 1,  (uint8_t*)data, len, HAL_MAX_DELAY);
	if(rslt != HAL_OK)
		return 1;

   return BME280_OK;
}

/*!
 * @brief This API used to print the sensor temperature, pressure and humidity data.
 */
void print_sensor_data(struct bme280_data *comp_data)
{


#ifdef BME280_FLOAT_ENABLE
    temp = comp_data->temperature;
    press = 0.01 * comp_data->pressure;
    hum = comp_data->humidity;
#else
#ifdef BME280_64BIT_ENABLE
    temp = 0.01f * comp_data->temperature;
    press = 0.0001f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#else
    temp = 0.01f * comp_data->temperature;
    press = 0.01f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#endif
#endif
  //  printf("%0.2lf deg C, %0.2lf hPa, %0.2lf%%\n", temp, press, hum);
}

/*!
 * @brief This API reads the sensor temperature, pressure and humidity data in forced mode.
 */
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev)
{
    /* Variable to define the result */
    int8_t rslt = BME280_OK;

    /* Variable to define the selecting sensors */
    uint8_t settings_sel = 0;

    /* Variable to store minimum wait time between consecutive measurement in force mode */
    uint32_t req_delay;

    /* Structure to get the pressure, temperature and humidity values */
    struct bme280_data comp_data;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    /* Set the sensor settings */
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    if (rslt != BME280_OK)
    {
    //    fprintf(stderr, "Failed to set sensor settings (code %+d).", rslt);

        return rslt;
    }

    printf("Temperature, Pressure, Humidity\n");

    /*Calculate the minimum delay required between consecutive measurement based upon the sensor enabled
     *  and the oversampling configuration. */
    req_delay = bme280_cal_meas_delay(&dev->settings);

    /* Continuously stream sensor data */
    while (1)
    {
        /* Set the sensor to forced mode */
        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
        if (rslt != BME280_OK)
        {
          //  fprintf(stderr, "Failed to set sensor mode (code %+d).", rslt);
            break;
        }

        /* Wait for the measurement to complete and print data */
        dev->delay_us(req_delay, dev->intf_ptr);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        if (rslt != BME280_OK)
        {
           // fprintf(stderr, "Failed to get sensor data (code %+d).", rslt);
            break;
        }

        print_sensor_data(&comp_data);
    }

    return rslt;
}
