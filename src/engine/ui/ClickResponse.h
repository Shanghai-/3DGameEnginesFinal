#ifndef CLICKRESPONSE_H
#define CLICKRESPONSE_H

class ClickResponse
{
public:
    virtual void onEnter() = 0;
    virtual void onHover() = 0;
    virtual void onExit() = 0;
    virtual void onPress() = 0;
    virtual void onHold() = 0;
    virtual void onRelease() = 0;
};

#endif // CLICKRESPONSE_H
