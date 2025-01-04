#pragma once
#include "pch.h"

#define SLEEP(ms) \
    global::BootServices->Stall(ms * 1000);

#define INFINITY_LOOP() \
    while(true) {};

#define SET_BACKGROUND(x) \
    global::SystemTable->ConOut->SetAttribute(global::SystemTable->ConOut, x);

#define CLEAR_SCREEN() \
    global::SystemTable->ConOut->ClearScreen(global::SystemTable->ConOut);

#define Log(text, ...) \
    do \
    { \
        InitializeDebug(); \
        if (DbgPrint != nullptr) \
            DbgPrint(text "\n", ##__VA_ARGS__); \
        else \
            global::SystemTable->ConOut->OutputString(global::SystemTable->ConOut, (CHAR16*)text L"\r\n"); \
    } while (0)

#define Error(text, ...) \
    do \
    { \
        Log(text, ##__VA_ARGS__); \
        INFINITY_LOOP(); \
    } while (0)


typedef void (*DbgPrintFn)(const char* Format, ...);
extern DbgPrintFn DbgPrint;

void InitializeDebug();