#include "led.h"

/*
 * Function: LED_Init
 * ----------------------------
 * 初始化PB5和PE5为输出模式，并确保LED在初始化时关闭。
 *
 * 参数: 无
 *
 * 返回值: 无
 *
 * 示例用法:
 *     LED_Init();
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 启用PB和PE端口的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);

    // 配置PB.5为推挽输出
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;        // 选择引脚PB.5
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  // 设置为推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 设置IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);             // 应用配置
    GPIO_SetBits(GPIOB, GPIO_Pin_5);                   // 使PB.5引脚输出高电平（LED关闭）

    // 配置PE.5为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  // 选择引脚PE.5
    GPIO_Init(GPIOE, &GPIO_InitStructure);     // 应用配置
    GPIO_SetBits(GPIOE, GPIO_Pin_5);           // 使PE.5引脚输出高电平（LED关闭）
}
