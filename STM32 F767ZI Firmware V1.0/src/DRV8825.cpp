#include "DRV8825.h"

DRV8825::DRV8825(PinName Enable, PinName M0, PinName M1, PinName M2, PinName STEP, PinName DIR, int minStepDelay) : _Enable(Enable), _M0(M0), _M1(M1), _M2(M2), _STEP(STEP), _DIR(DIR)
{

    _minStepDelay = minStepDelay;
    _Enable = 1;
    _M0 = 0;
    _M1 = 0;
    _M2 = 0;
    _previousStepDelay = 2000;
    StepDelay_ =800;
    _stepDelay = 800;
}

//***********************************************************************************
void DRV8825::setDestination(int numStep, int resolution, int direction, int stepDelay)
{
    _DIR = direction;
    _numStep = numStep * 2;
    _previousStepDelay = 3000;
    setResolution(resolution);
    if (stepDelay < _minStepDelay)
        _stepDelay = _minStepDelay;
    else
        _stepDelay = stepDelay;
}

void DRV8825::setResolution(int resolution)
{

    switch (resolution)
    {
    case 1:
        _M0 = 0;
        _M1 = 0;
        _M2 = 0;
        break;
    case 2:
        _M0 = 1;
        _M1 = 0;
        _M2 = 0;
        break;
    case 4:
        _M0 = 0;
        _M1 = 1;
        _M2 = 0;
        break;
    case 8:
        _M0 = 1;
        _M1 = 1;
        _M2 = 0;
        break;
    case 16:
        _M0 = 0;
        _M1 = 0;
        _M2 = 1;
        break;
    case 32:
        _M0 = 1;
        _M1 = 0;
        _M2 = 1;
        break;
    }
}
//************************************************************************************
void DRV8825::update(unsigned long currentMillis)
{
     
     if (_previousStepDelay > _stepDelay && _previousStepDelay - _stepDelay > 2)
    {
        if (currentMillis >= _LastmillisStepCtrl + 100)
            {
                //printf("%d \n",StepDelay_);
                StepDelay_ = _previousStepDelay - 1;
                _LastmillisStepCtrl=currentMillis;
            }
    }
    else
    {
        StepDelay_ = _stepDelay;
    }
    if (currentMillis >= _Lastmillis + StepDelay_)
    {
        doStep();
        _numStep--;
        _Lastmillis = currentMillis;
       // printf("%d \n",StepDelay_);
    }
    
    _previousStepDelay = StepDelay_;
}
//**************************************************************************************
void DRV8825::freeMove(unsigned long currentMillis, int StepDelay, int Direction)
{
   

    if (_previousStepDelay > StepDelay && _previousStepDelay - StepDelay > 2)
    {
        if (currentMillis >= _LastmillisStepCtrl + 50)
            {
                StepDelay_ = _previousStepDelay - 1;
                _LastmillisStepCtrl=currentMillis;
            }
    }
    else
    {
        StepDelay_ = StepDelay;
    }

    if (currentMillis >= _Lastmillis + StepDelay_)
    {
        //printf("%d\n",StepDelay_);
        doStep();
        _Lastmillis = currentMillis;
        _DIR = Direction;
    }
    _previousStepDelay = StepDelay_;
}

int DRV8825::stepToGo()
{
    return _numStep;
}


void DRV8825::enable(int en)
{
    _Enable = en;
}
void DRV8825::doStep()
{
    _STEP = !_STEP;
}

//************************************************************************************