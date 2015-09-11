#ifndef _GB19056_CALL_BACK_INTERFACE_H_
#define _GB19056_CALL_BACK_INTERFACE_H_

class GB19056CallBackInterface
{
public:
    virtual int FileExistNotify() = 0;
    virtual int ProgressNotify(int current_cmd, int total_cmd) = 0;
};

#endif
