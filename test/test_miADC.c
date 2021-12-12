#include "unity.h"
#include "mi_ADC.c"
#include "mi_ADC.h"
#include "mock_sapi.h"
#include "mock_mi_uart.h"

miADC_t miADCs;

void setUp(void)
{
    adcConfig_Ignore();
    ADCinicializarMEF(&miADCs);
}
/**
 * @brief Prueba la inicialización del modulo de ADC que se realiza con la función:
 * funcion a probar: void ADCinicializarMEF(miADC_t* ptrmiADCs);
 * 
 */
void test_inicializar_modulo_ADC(void)
{
    // Cargo la estructura con valores para probar que la función ponga los correctos
    miADCs.CH1 = true;
    miADCs.CH2 = true;
    miADCs.CH3 = true;
    miADCs.frecuencia=1;
    miADCs.FlagComandoRecibido = true;
    
    //Mock para la función adcConfig
    adcConfig_Expect(ADC_ENABLE);
    
    ADCinicializarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_STANDBY , miADCs.estado);
    TEST_ASSERT_EQUAL(false , miADCs.CH1);
    TEST_ASSERT_EQUAL(false , miADCs.CH2);
    TEST_ASSERT_EQUAL(false , miADCs.CH3);
    TEST_ASSERT_EQUAL(0 , miADCs.frecuencia);
    TEST_ASSERT_EQUAL(false , miADCs.FlagComandoRecibido);  
}

/**
 * @brief Prueba que cuando llega el comando CMD_HOLA estando en el estado STANDBY
 * responde con el comando CMD_OK y cambia al estado a CONECTADO
 * 
 */
void test_recibe_comando_CMD_Hola_en_standBy(void)
{
    cmd_t command = CMD_HOLA;
    UART_getCmd_ExpectAndReturn(0 , true);
    UART_getCmd_IgnoreArg_command();
    UART_getCmd_ReturnThruPtr_command(&command);
    UART_sendCmd_Expect(CMD_OK);
    ADCactualizarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_CONECTADO , miADCs.estado);
    TEST_ASSERT_EQUAL(true , miADCs.FlagComandoRecibido);
}

/**
 * @brief Prueba que cuando llega comando distinto a CMD_HOLA estando en el estado STANDBY
 * responde con el comando CMD_NO_OK, no cambian de estado y no prende el flag de que llegó un comando.
 * 
 */
void test_recibe_comando_invalido_en_standBy(void)
{
    cmd_t command = 0;
    UART_getCmd_ExpectAndReturn(0 , true);
    UART_getCmd_IgnoreArg_command();
    UART_getCmd_ReturnThruPtr_command(&command);
    UART_sendCmd_Expect(CMD_NO_OK);
    ADCactualizarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_STANDBY , miADCs.estado);
    TEST_ASSERT_EQUAL(false , miADCs.FlagComandoRecibido);
}

/**
 * @brief Prueba que cuando llega el comando CMD_CHAU, estando en el estado ESTADO_CONECTADO
 * responde con el comando CMD_OK, cambia al estado a STANDBY y prende el flag de que llegó un comando
 * 
 */
void test_recibe_comando_CMD_CHAU_en_CONECTADO(void)
{
    cmd_t command = CMD_CHAU;
    miADCs.estado = ESTADO_CONECTADO;

    UART_getCmd_ExpectAndReturn(0 , true);
    UART_getCmd_IgnoreArg_command();
    UART_getCmd_ReturnThruPtr_command(&command);
    UART_sendCmd_Expect(CMD_OK);
    ADCactualizarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_STANDBY , miADCs.estado);
    TEST_ASSERT_EQUAL(true , miADCs.FlagComandoRecibido);
}

/**
 * @brief Prueba que cuando llega el comando CMD_CONFIG_INICIAR, estando en el estado ESTADO_CONECTADO
 * responde con el comando CMD_OK, cambia al estado a ESTADO_CONFIGURANDO y prende el flag de que llegó un comando
 * 
 */
void test_recibe_comando_CMD_CONFIG_INICIAR_en_CONECTADO(void)
{
    cmd_t command = CMD_CONFIG_INICIAR;
    miADCs.estado = ESTADO_CONECTADO;

    UART_getCmd_ExpectAndReturn(0 , true);
    UART_getCmd_IgnoreArg_command();
    UART_getCmd_ReturnThruPtr_command(&command);
    UART_sendCmd_Expect(CMD_OK);
    ADCactualizarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_CONFIGURANDO , miADCs.estado);
    TEST_ASSERT_EQUAL(true , miADCs.FlagComandoRecibido);
}

/**
 * @brief Prueba que cuando llega comando distinto a CMD_CONFIG_INICIAR o CMD_CHAU estando en el
 * estado ESTADO_CONECTADO responde con el comando CMD_NO_OK, no cambian de estado y no prende el
 * flag de que llegó un comando.
 * 
 */
void test_recibe_comando_invalido_en_CONECTADO(void)
{
    cmd_t command = 0;
    miADCs.estado = ESTADO_CONECTADO;

    UART_getCmd_ExpectAndReturn(0 , true);
    UART_getCmd_IgnoreArg_command();
    UART_getCmd_ReturnThruPtr_command(&command);
    UART_sendCmd_Expect(CMD_NO_OK);
    ADCactualizarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_CONECTADO , miADCs.estado);
    TEST_ASSERT_EQUAL(false , miADCs.FlagComandoRecibido);
}

/**
 * @brief Prueba que si está listo para adquirir en el estado ESTADO_CONECTADO cambia al 
 * estado ESTADO_LISTO_PARA_ADQUIRIR
 * 
 */
void test_listo_para_adquirir_en_CONECTADO(void)
{
    miADCs.CH1 = true;
    miADCs.frecuencia = 1;
    miADCs.estado = ESTADO_CONECTADO;

    UART_getCmd_ExpectAndReturn(0 , false);
    UART_getCmd_IgnoreArg_command();
    ADCactualizarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_LISTO_PARA_ADQUIRIR , miADCs.estado);
}

/**
 * @brief Prueba que cuando llega comando distinto a alguno de configuracion estando en el
 * estado ESTADO_CONFIGURANDO responde con el comando CMD_NO_OK, no cambian de estado y no prende el
 * flag de que llegó un comando.
 * 
 */
void test_recibe_comando_invalido_en_CONFIGURANDO(void)
{
    cmd_t command = 0;
    miADCs.estado = ESTADO_CONFIGURANDO;

    UART_getCmd_ExpectAndReturn(0 , true);
    UART_getCmd_IgnoreArg_command();
    UART_getCmd_ReturnThruPtr_command(&command);
    UART_sendCmd_Expect(CMD_NO_OK);
    ADCactualizarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_CONFIGURANDO , miADCs.estado);
    TEST_ASSERT_EQUAL(false , miADCs.FlagComandoRecibido);
}

/**
 * @brief Prueba que cuando llega comando CMD_CONFIG_TERMINAR estando en el
 * estado ESTADO_CONFIGURANDO actualiza el estado y prende el flag de que llegó un comando.
 * 
 */
void test_recibe_comando_TERMINAR_en_CONFIGURANDO(void)
{
    cmd_t command = CMD_CONFIG_TERMINAR;
    miADCs.estado = ESTADO_CONFIGURANDO;

    UART_getCmd_ExpectAndReturn(0 , true);
    UART_getCmd_IgnoreArg_command();
    UART_getCmd_ReturnThruPtr_command(&command);
    ADCactualizarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_CONECTADO , miADCs.estado);
    TEST_ASSERT_EQUAL(true , miADCs.FlagComandoRecibido);
}

/**
 * @brief Prueba que cuando llega comando para CONFIGURAR los canales o la frecuencia de muestreo
 * estando en el estado ESTADO_CONFIGURANDO llama a configurar el ADC, prende el flag de que llegó un comando
 * y responde con comando CMD_OK.
 * 
 */
void test_recibe_comando_CONFIG_en_CONFIGURANDO(void)
{
    cmd_t command = CMD_CONFIG_CH1_ON;
    miADCs.estado = ESTADO_CONFIGURANDO;

    UART_getCmd_ExpectAndReturn(0 , true);
    UART_getCmd_IgnoreArg_command();
    UART_getCmd_ReturnThruPtr_command(&command);
    UART_sendCmd_Expect(CMD_OK);
    ADCactualizarMEF(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_CONFIGURANDO , miADCs.estado);
    TEST_ASSERT_EQUAL(true , miADCs.FlagComandoRecibido);
    TEST_ASSERT_EQUAL(true , miADCs.CH1);
}

/**
 * @brief Prueba el llamado a la funcion ADClisto cuando no se tiene configurada
 * ninguna frecuencia de adquisición.
 * 
 */
void test_ADClisto_sin_frecuencia_configurada(void)
{
    TEST_ASSERT_FALSE( ADClisto(&miADCs) );
}

/**
 * @brief Prueba el llamado a la función ADClisto cuando solo se tiene configurado un canal.
 * 
 */
void test_ADClisto_solo_canal_configurado(void)
{
    miADCs.CH1 = true;
    TEST_ASSERT_FALSE( ADClisto(&miADCs) );
}

/**
 * @brief Prueba el llamado a la función ADClisto cuando se tiene configurada la frecuencia
 * de adquisición y configurado al menos un canal.
 * 
 */
void test_ADClisto_con_un_canal_mas_frecuencia_configurado(void)
{
    miADCs.CH1 = true;
    miADCs.frecuencia = 1;
    TEST_ASSERT_TRUE( ADClisto(&miADCs) );
}

/**
 * @brief Prueba que se lea el valor de los ADCs configurados.
 * 
 */
void test_adquirir_canal_configurado(void)
{
    miADCs.CH1 = true;
    miADCs.CH2 = true;
    miADCs.CH3 = true;
    adcRead_ExpectAndReturn(CH1 , 0xAAAA);
    adcRead_ExpectAndReturn(CH2 , 0x5555);
    adcRead_ExpectAndReturn(CH3 , 0xF0F0);
    ADCadquirir(&miADCs);

    TEST_ASSERT_EQUAL_UINT16(0xAAAA , miADCs.paquete.data_CH1 );
    TEST_ASSERT_EQUAL_UINT16(0x5555 , miADCs.paquete.data_CH2 );
    TEST_ASSERT_EQUAL_UINT16(0xF0F0 , miADCs.paquete.data_CH3 );
}

/**
 * @brief Prueba si se intenta leer de los ADCs sin estar configurados
 * 
 */
void test_adquirir_sin_canal_configurado(void)
{
    miADCs.paquete.data_CH1 = 0x0000;
    miADCs.paquete.data_CH2 = 0x0000;
    miADCs.paquete.data_CH3 = 0x0000;

    ADCadquirir(&miADCs);
    
    TEST_ASSERT_EQUAL(ESTADO_STANDBY , miADCs.estado);
    TEST_ASSERT_EQUAL(false , miADCs.CH1);
    TEST_ASSERT_EQUAL(false , miADCs.CH2);
    TEST_ASSERT_EQUAL(false , miADCs.CH3);
    TEST_ASSERT_EQUAL(0 , miADCs.frecuencia);
    TEST_ASSERT_EQUAL(false , miADCs.FlagComandoRecibido);
    TEST_ASSERT_EQUAL_UINT16(0x0000 , miADCs.paquete.data_CH1 );
    TEST_ASSERT_EQUAL_UINT16(0x0000 , miADCs.paquete.data_CH2 );
    TEST_ASSERT_EQUAL_UINT16(0x0000 , miADCs.paquete.data_CH3 );
}
