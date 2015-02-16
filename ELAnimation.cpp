//
//  ELAnimation.cpp
//
//  Copyright (c) 2015 David Robles
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#include "Arduino.h"
#include "ELAnimation.h"
#include <Metro.h>

ELAnimation::ELAnimation(Metro *timer, byte channel) {
  _timer = timer;
  _channel = channel;
}

void ELAnimation::runLoop() {
   if (_timer->check() == 1) {
        if (_done) {
            switch (_state) {
                case ELAnimationStateOff:
                    // Serial.println("off -> trans on");
                    _state = ELAnimationStateTransitionOn;
                    break;
                case ELAnimationStateTransitionOn:
                    // Serial.println("trans on -> on");
                    _state = ELAnimationStateOn;   
                    break;
                case ELAnimationStateOn:
                    // Serial.println("on -> trans off");
                    _state = ELAnimationStateTransitionOff;    
                    break;
                case ELAnimationStateTransitionOff:
                    // Serial.println("trans off -> off");
                    _state = ELAnimationStateOff;   
                    break;
            }

            _done = false;
        }


       switch (_state) {
        case ELAnimationStateOff:
            turnOff();
            break;
        case ELAnimationStateTransitionOn:
            fadeIn();    
            break;
        case ELAnimationStateOn:
            turnOn();
            break;
        case ELAnimationStateTransitionOff:
            fadeOut();
            break;
        }
    }
}

#define PulseWidth 5

void ELAnimation::turnOff() {
    digitalWrite(_channel, LOW);
    _timer->interval(SolidDuration);
    _done = true;
}

void ELAnimation::turnOn() {
    digitalWrite(_channel, HIGH);
    _timer->interval(SolidDuration);
    _done = true;
}

void ELAnimation::fadeIn() {
    if (_brightness > PulseWidth) {
        _brightness = 5;
        _duration = 0;
        _done = true;
        return;
    }

    if (digitalRead(_channel) == LOW) {
        digitalWrite(_channel, HIGH);
        _timer->interval(_brightness);
    }
    else {
        digitalWrite(_channel, LOW);
        _timer->interval(PulseWidth - _brightness);
    }

    if (_duration >= TransitionDuration) {
        _duration = 0;
        _brightness++;
    }
    else {
        _duration++;
    }
}

void ELAnimation::fadeOut() {
    if (_brightness < 0) {
        _brightness = 0;
        _duration = 0;
        _done = true;
        return;
    }

    if (digitalRead(_channel) == LOW) {
        digitalWrite(_channel, HIGH);
        _timer->interval(_brightness);
    }
    else {
        digitalWrite(_channel, LOW);
        _timer->interval(PulseWidth - _brightness);
    }

    if (_duration >= TransitionDuration) {
        _duration = 0;
        _brightness--;
    }
    else {
        _duration++;
    }
}
