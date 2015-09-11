#include "ConcreteTableBase.h"

void ConcreteTableBase::SetTable()
{
    for(int i = 0; i < m_colnum; i ++)
    {
        m_table.AddColumn(m_col[i]);
    }
}
