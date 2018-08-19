#include "tribool.h"
namespace logic
{
    tribool operator&&(tribool x, tribool y)
    {
        if(x.value == tribool::TRUE && y.value == tribool::TRUE)
            return tribool(true);
        if((x.value == tribool::INDETERMINATE && y.value == tribool::INDETERMINATE)
        || (x.value == tribool::INDETERMINATE && y.value == tribool::TRUE)
        || (x.value == tribool::TRUE && y.value == tribool::INDETERMINATE))
            return tribool(tribool::INDETERMINATE);
        return tribool(false);
    }
    tribool operator||(tribool x, tribool y)
    {
        if(x.value == tribool::FALSE && y.value == tribool::FALSE)
            return tribool(false);
        if((x.value == tribool::INDETERMINATE && y.value == tribool::INDETERMINATE)
        || (x.value == tribool::INDETERMINATE && y.value == tribool::FALSE)
        || (x.value == tribool::FALSE && y.value == tribool::INDETERMINATE))
            return tribool(tribool::INDETERMINATE);
        return tribool(true);
    }
    tribool operator==(tribool x, tribool y)
    {
        if(x.value == tribool::INDETERMINATE || y.value == tribool::INDETERMINATE)
            return tribool(tribool::INDETERMINATE);
        return x.value == y.value;
    }
    tribool operator!=(tribool x, tribool y)
    {
        return !(x==y);
    }
}