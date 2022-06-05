#include "stm32f1xx.h" // Biblioteca STM
#define CTE_AGUARDA 50
#define TEMPO_LED 200

//  Variaveis globais necessárias para o código
signed char aguarda = 0;   // Armazena a temporização em (aguarda x 10ms), quando 0 não aguarda
short int linha = 0, key = 0, isLedOn = 0;
char tecla_press[] = "0000";
const char matriz_teclas[4][4] = { {'1','2','3','A'},
                                   {'4','5','6','B'},
                                   {'7','8','9','C'},
                                   {'*','0','#','D'} };

// Funções utilizadas
void EXTI0_IRQHandler(void) // Coluna 0
{
    EXTI->PR |= EXTI_PR_PR0;          // Apaga flag sinalizadora de interrupção
    if ( GPIOA->IDR & GPIO_IDR_IDR0 )
    { 
        tecla_press[key] = matriz_teclas[linha][3];
        EXTI->IMR &= ~EXTI_IMR_IM0; // Deshab. a int. do EXTI0 a EXTI3
        aguarda = CTE_AGUARDA; // Variável para aguardar 1s para reab. int.
        ++key;
    }
}
void EXTI1_IRQHandler(void) // Coluna 1
{
    EXTI->PR |= EXTI_PR_PR1;    
    if ( GPIOA->IDR & GPIO_IDR_IDR1 )
    {
        tecla_press[key] = matriz_teclas[linha][2];
        EXTI->IMR &= ~EXTI_IMR_IM1; // Deshab. a int. do EXTI0 a EXTI3
        aguarda = CTE_AGUARDA;
        ++key;
    }
}
void EXTI2_IRQHandler(void) // Coluna 2
{
    EXTI->PR |= EXTI_PR_PR2;                                
    if ( GPIOA->IDR & GPIO_IDR_IDR2 )
    {
        tecla_press[key] = matriz_teclas[linha][1];
        EXTI->IMR &= ~EXTI_IMR_IM2; // Deshab. a int. do EXTI0 a EXTI3
        aguarda = CTE_AGUARDA;
        ++key;
    }      
}
void EXTI3_IRQHandler(void) // Coluna 3
{
    EXTI->PR |= EXTI_PR_PR3;                  
    if ( GPIOA->IDR & GPIO_IDR_IDR3 )
    {                     
        tecla_press[key] = matriz_teclas[linha][0];
        EXTI->IMR &= ~EXTI_IMR_IM3; // Deshab. a int. do EXTI0 a EXTI3
        aguarda = CTE_AGUARDA;
        ++key;
    }     
}

void gpioConfig(void)   // Configura os registradores
{
    /*
     *  Portas da GPIOC 13 e 14 para LED de senha falhada ou bem sucedida respectivamente
     *  Portas da GPIOA 7, 6, 5 e 4 linhas do teclado
     *  Portas da GPIOA 3, 2, 1 e 0 colunas do teclado 
    */
    
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Habilita clock do barramento APB2 para GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //  Habilita funções alternativas APB2
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Habilita clock do barramento APB2 para GPIOC

    GPIOA->CRL |= GPIO_CRL_MODE7_0 | GPIO_CRL_MODE7_1;    // Configura pino PA7 como saída de 50 MHz
    GPIOA->CRL |= GPIO_CRL_MODE6_0 | GPIO_CRL_MODE6_1;    // Configura pino PA6 como saída de 50 MHz
    GPIOA->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1;    // Configura pino PA5 como saída de 50 MHz
    GPIOA->CRL |= GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1;    // Configura pino PA4 como saída de 50 MHz

    GPIOA->CRL &= ~(GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1);    // Configura pino PA3 como entrada
    GPIOA->CRL &= ~(GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1);    // Configura pino PA2 como entrada
    GPIOA->CRL &= ~(GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);    // Configura pino PA1 como entrada
    GPIOA->CRL &= ~(GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1);    // Configura pino PA0 como entrada

    GPIOA->CRL &= ~(GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1); // push-pull
    GPIOA->CRL &= ~(GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1); // push-pull
    GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1); // push-pull
    GPIOA->CRL &= ~(GPIO_CRL_CNF4_0 | GPIO_CRL_CNF4_1); // push-pull

    GPIOA->CRL &= ~GPIO_CRL_CNF3_0; // Seta primeiro bit como zero
    GPIOA->CRL &= ~GPIO_CRL_CNF2_0; // Seta primeiro bit como zero
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0; // Seta primeiro bit como zero
    GPIOA->CRL &= ~GPIO_CRL_CNF0_0; // Seta primeiro bit como zero

    GPIOA->CRL |= GPIO_CRL_CNF3_1; // pull-up e pull-down
    GPIOA->CRL |= GPIO_CRL_CNF2_1; // pull-up e pull-down
    GPIOA->CRL |= GPIO_CRL_CNF1_1; // pull-up e pull-down
    GPIOA->CRL |= GPIO_CRL_CNF0_1; // pull-up e pull-down

    GPIOA->ODR &= ~GPIO_ODR_ODR3;   // pull-down
    GPIOA->ODR &= ~GPIO_ODR_ODR2;   // pull-down
    GPIOA->ODR &= ~GPIO_ODR_ODR1;   // pull-down
    GPIOA->ODR &= ~GPIO_ODR_ODR0;   // pull-down

    //GPIOA->CRL = 0x33338888;    // 0x3 = 0b0011 saída push-pull 50MHz. E 0x8 entrada pull-up e pull-down.
    
    GPIOC->CRH |= GPIO_CRH_MODE13_1;    // Configura pino PC13 como saída de 2 MHz
    GPIOC->CRH |= GPIO_CRH_MODE14_1;    // Configura pino PC14 como saída de 2 MHz

    GPIOC->CRH |= GPIO_CRH_CNF13_0;    // open-drain
    GPIOC->CRH |= GPIO_CRH_CNF14_0;    // open-drain
}
void sysConfig(void)    // Config. SysTick com interrupção a cada 1O ms ->  100 Hz
{     
    SysTick->LOAD = (int)8e4;      // (default BluePill: HSI com 8MHz)
	SysTick->VAL = 0;		    // Limpa o valor da contagem
    SysTick->CTRL = 0b111; 	    // Clock do processador sem dividir, Hab. IRQ e SysTick */
}
void extiConfig(void)   // Config PAO interrupção no EXTI
{
    AFIO->EXTICR[1] = AFIO_EXTICR1_EXTI0_PA | AFIO_EXTICR1_EXTI1_PA | AFIO_EXTICR1_EXTI2_PA | AFIO_EXTICR1_EXTI3_PA;    // Seleciona PA0 à PA3 para EXTI0 á EXTI03

    EXTI->RTSR = EXTI_RTSR_RT0 | EXTI_RTSR_RT1 | EXTI_RTSR_RT2 | EXTI_RTSR_RT3; // Sensível na rampa de subida
    EXTI->IMR = EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3;  // Hab. máscara de interrup. do EXTI0 à EXTI3  

    NVIC->ISER[0] = (1 << EXTI0_IRQn) | (1 << EXTI1_IRQn) | (1 << EXTI2_IRQn) | (1 << EXTI3_IRQn);  // Hab. IRQ do EXTI0
}

void ligaDesligaLED(char op) // Liga ou desliga a LED por 2 segundos
{
    switch ( op )
    {
    case '0':
        GPIOC->ODR &= ~GPIO_ODR_ODR13;
        break;
    case '1':
        GPIOC->ODR &= ~GPIO_ODR_ODR14;
        break;
    }
}

char verificaSenha(char* strSenha)
{
    char strSenhaCorreta[] = "3532";
    isLedOn = TEMPO_LED;
    register short int i;
    for ( i = 0; i < 4; ++i )
        if ( strSenha[i] != strSenhaCorreta[i] )
            break;
    
    return ( i == 4 ) ? ('1') : ('0');
}

void SysTick_Handler(void) // Tratamento da interrupção do SysTick (ocorre a cada 10 ms)
{
    ++linha;
    linha %= 4;

    GPIOA->ODR = (GPIOA->ODR & ~(GPIO_ODR_ODR4 | GPIO_ODR_ODR5 | GPIO_ODR_ODR6 | GPIO_ODR_ODR7)) | (1 << (7-linha));

    if ( isLedOn > 0  )
    {
        --isLedOn;

        if ( isLedOn == 0 )
            GPIOC->ODR = GPIO_ODR_ODR13 | GPIO_ODR_ODR14; // Desliga LED
    }

    else
    {
        if ( key == 4 )
        {
            key = 0;
            ligaDesligaLED(verificaSenha(tecla_press)); // Liga LED
        }

        if ( aguarda > 0 )
        {
            --aguarda;

            if ( aguarda == 0 )
                EXTI->IMR = EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3; // Hab. a int. do EXTI0 a EXTI3
        }   
    }
}

// Função principal
int main(void)
{
    gpioConfig();
    sysConfig();

    GPIOC->ODR = GPIO_ODR_ODR13 | GPIO_ODR_ODR14; // Desliga LED

    extiConfig();

    __enable_irq(); // Hab. globalmente as IRQs (PRIMASK), no reset já vem habilitado

    while ( 1 ); // Loop infinito
    return 0;
}