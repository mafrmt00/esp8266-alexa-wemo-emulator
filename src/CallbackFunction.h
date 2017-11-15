#ifndef CALLBACKFUNCTION_H
#define CALLBACKFUNCTION_H

#include <Arduino.h>

typedef std::function<void(void)> CallbackFunction;
typedef std::function<bool(void)> StatusCallbackFunction;

#endif
