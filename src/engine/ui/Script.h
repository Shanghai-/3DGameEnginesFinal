#ifndef SCRIPT_H
#define SCRIPT_H

class Script
{
public:
    Script() {}
    ~Script() {}

    virtual void onTick(float seconds) = 0;
    virtual void onDraw() = 0;
    virtual void onResize(int w, int h) = 0;
};

#endif // SCRIPT_H
