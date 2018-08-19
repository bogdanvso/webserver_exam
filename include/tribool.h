#ifndef TRIBOOL_H
#define TRIBOOL_H


namespace logic
{
    class tribool
    {
        public:
        enum value_t { FALSE, TRUE, INDETERMINATE} value;

        tribool() noexcept : value(INDETERMINATE){}
        tribool(bool b) noexcept : value(static_cast<value_t>(b)){}
        tribool(value_t t) noexcept : value(t){}
        tribool(const tribool&) = default;
        tribool& operator=(const tribool&) = default;
        tribool(tribool&&) = default;
        tribool& operator=(tribool&&) = default;

        operator bool() const noexcept { return value == tribool::TRUE ? true : false; }
        tribool operator!() const noexcept
        {
            if(value == tribool::TRUE)
                return tribool(false);
            else if(value == tribool::FALSE)
                return tribool(true);
            return tribool(tribool::INDETERMINATE);
        }
    };
    
    tribool operator&&(tribool x, tribool y);
    tribool operator||(tribool x, tribool y);
    tribool operator==(tribool x, tribool y);
    tribool operator!=(tribool x, tribool y);
} // logic



#endif // !TRIBOOL_H