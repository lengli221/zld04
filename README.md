# zld04

#### 介绍
{**以下是 Gitee 平台说明，您可以替换此简介**
Gitee 是 OSCHINA 推出的基于 Git 的代码托管平台（同时支持 SVN）。专为开发者提供稳定、高效、安全的云端软件开发协作平台
无论是个人、团队、或是企业，都能够用 Gitee 实现代码托管、项目管理、协作开发。企业项目请看 [https://gitee.com/enterprises](https://gitee.com/enterprises)}

#### 软件架构
软件架构说明


#### 安装教程

1.  xxxx
2.  xxxx
3.  xxxx

#### 使用说明

20210720 18:38
1.采用GD32官方库文件创建工程,CORE采用KEIL5自带
2.相关功能：
	2.1 PGIO输出功能
	2.2 SysTick
	2.3 USART1-->DMA中断发送,基本的中断接收
	2.4 USART2-->printf
	
20210721 17:54
1.CAN0t采用Loopback测试通过
 --》存在问题是:中断中检测空邮件标记,无法成功
 2.CAN0采用Normal待板子再行测试	
 

20210722 17:44
1.见Usart2修改：发送DMA中断和接受DMA中断,在485逻辑下,限制发送数据之后限制接受的数据数量,直接排错
2.usart采用DMA接受中断,用于发送,通过中断判定数据是否接受完成,并直接解析,避免循环轮捡shuju,等待数据接受,再行解析

20210723 17:46 --》补
1.将usart1从中断接收模式修改成“MDA中断接收模式”
2.实现接收标志,数据项指针提供App接收使用

20210727 17:27 --》补(20210726)
1.上层协议modbus(框架协议)
2.bms/charger上层接口协议
3.通用接口函数:大小端转换
4.上层协议接口,解析表格函数
5.上层协议配置信息
6.上层协议状态信息
7.关闭usart1接收端轮询配置
8.关闭usart2发送端和接收端配置

20210728 --》补(20210727)
1.电池状态信息变更-->接口函数
2.电池离线--》清电池数据
3.get SOC阈值接口函数,get SOC实时接口函数
4.修改定义电池ID长度,从uint8修改成uint16
5.上层接口:开仓,电池信息,充电器信息 更新舱门状态实时检测逻辑
6.修改定义变量:分控故障信息Err修改为err
7.调整CAN接收,发送邮箱缓存深度
8.调整CAN发送任务定时器时间:由5ms修改为2ms
9.增加开仓,回检硬件接口函数(未实现底层硬件)

20210805--》补(20210730)
1.新增:开仓结果上报逻辑及处理逻辑
2.依据相应时间调整：控制仓门接触时间调整

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
