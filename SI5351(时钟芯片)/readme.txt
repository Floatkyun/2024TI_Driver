HAL库版本
使用硬件IIC 需要在CUBE中配置硬件IIC
在SI5351硬件模块没有连接到单片机上时，请在主函数中去除所有该驱动相关函数 否则一定会引起单片机卡死！（硬件IIC特性，单片机等待IIC通信应答位所致）