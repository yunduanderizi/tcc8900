/**
*<FH+>***************************************************************************
*
* 版权所有 (C)2011, 远特科技有限公司
* 
* 文件名称： InputControl.h
* 文件标识： (见配置管理计划书)
* 内容摘要： 
* 其它说明： 
* 当前版本： V1.0
* 作    者： 于震洋
* 完成日期： [2011/10/19]
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容： 
*<FH+>***************************************************************************
*/ 
#include <QThread>
/**
*<CLASS+>********************************************************************
* 类 名 称: InputControlC
* 功能描述: 输入控制线程
* 其它说明: 
* 修改记录: 
* 修改日期			版 本		修改人		修改摘要
* -------------------------------------------------------------------------
* [2011/10/19]	    V1.0		于震洋		创建类
*<CLASS->*********************************************************************
*/
class InputControlC : public QThread
{
public:
    InputControlC();
    virtual ~InputControlC();

    int SendKey();
};
