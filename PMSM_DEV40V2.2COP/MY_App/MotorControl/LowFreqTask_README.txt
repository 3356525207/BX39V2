/**
 * 低频率任务调度器使用说明
 *
 * 本调度器基于40kHz的高速PWM中断，实现不同频率的任务调度。
 *
 * 支持的频率：
 * - 500Hz：每80个高速中断调用一次 (40kHz / 80 = 500Hz)
 * - 100Hz：每400个高速中断调用一次 (40kHz / 400 = 100Hz)
 * - 10Hz：每4000个高速中断调用一次 (40kHz / 4000 = 10Hz)
 *
 * 使用步骤：
 * 1. 在Sensoruse_Init()中调用LowFreqTask_Init()初始化调度器
 * 2. 定义任务函数，例如：
 *    void MyTask_100Hz(void)
 *    {
 *        // 任务代码
 *    }
 * 3. 在初始化时注册任务：
 *    LowFreqTask_Register(TASK_FREQ_100HZ, MyTask_100Hz);
 * 4. 调度器会在TMR1_OVF_TMR10_IRQHandler中断中自动调用
 *
 * 示例代码：
 * // 在main.c或初始化函数中
 * LowFreqTask_Register(TASK_FREQ_500HZ, Task_500Hz_Example);
 * LowFreqTask_Register(TASK_FREQ_100HZ, Task_100Hz_Example);
 * LowFreqTask_Register(TASK_FREQ_10HZ, Task_10Hz_Example);
 *
 * 注意事项：
 * - 任务函数应尽量简短，避免阻塞
 * - 高频率任务(500Hz)适合快速响应需求
 * - 中等频率任务(100Hz)适合控制算法
 * - 低频率任务(10Hz)适合监控、通信等
 */</content>
<parameter name="filePath">e:\BUildingMCU\PMSM_DEV40V2.2\MY_App\MotorControl\LowFreqTask_README.txt