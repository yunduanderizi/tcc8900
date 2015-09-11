#include "GB19056Base.h"

int GB19056Base::ProcessRequest(GB19056Buffer &buffer)
{
    if(buffer.Check() == false)
    {
        GenerateError(buffer);
        buffer.GenerateChecksum();
        return -1;
    }
    
    PacketHead(buffer);

    buffer.PushByte(buffer.GetCmd());
    buffer.PushWord(0x0); // Dummy length
    PacketReserve(buffer);

    if(ProcessCmd(buffer) < 0)
    {
        GenerateError(buffer);
        buffer.GenerateChecksum();
        return -1;
    }

    buffer.UpdateLength();
    buffer.GenerateChecksum();
    return 0;
}

int GB19056Base::ProcessRequest_JT808(GB19056Buffer &buffer)
{
    if(buffer.Check() == false)
    {
        GenerateError(buffer);
        buffer.GenerateChecksum();
        return -1;
    }
    
    PacketHead(buffer);

    buffer.PushByte(buffer.GetCmd());
    buffer.PushWord(0x0); // Dummy length
    PacketReserve(buffer);

    if(ProcessCmd_JT808(buffer) < 0)
    {
        GenerateError(buffer);
        buffer.GenerateChecksum();
        return -1;
    }

    buffer.UpdateLength();
    buffer.GenerateChecksum();
    return 0;
}

void GB19056Base::GenerateQueryErrorResponse(GB19056Buffer &buffer)
{
    buffer.PushByte(GB19056_QUERY_ERROR_FLAG);
    PacketReserve(buffer);
}

void GB19056Base::GenerateSetErrorResponse(GB19056Buffer &buffer)
{
    buffer.PushByte(GB19056_SET_ERROR_FLAG);
    PacketReserve(buffer);
}
