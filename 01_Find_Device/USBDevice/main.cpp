//
//  main.cpp
//  USBDevice
//
//  Created by macbook pro on 2023/3/8.
//

#include <iostream>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
int main(int argc, const char * argv[]) {
    CFDictionaryRef matchingDict = NULL;
    io_iterator_t iter = 0;
    io_service_t service = 0;
    kern_return_t kr;
    
    //创建一个匹配字典 查找任意设备
    
    matchingDict = IOServiceMatching("IOUSBHostDevice");
    
    //为所有匹配对象创建一个迭代器
    kr = IOServiceGetMatchingServices(kIOMainPortDefault, matchingDict, &iter);
    if (kr != KERN_SUCCESS) {
        return  -1;
    }
    
    while ((service = IOIteratorNext(iter)) != 0) {
        CFStringRef className;
        io_name_t name;
        //列出所有对象,忽略IOUSBDevice子类对象
        className = IOObjectCopyClass(service);
        if (CFEqual(className, CFSTR("IOUSBHostDevice"))) {
            IORegistryEntryGetName(service, name);
            printf("Found device with name %s\n",name);
        }
        CFRelease(className);
        IOObjectRelease(service);
    }
    
    //释放迭代器
    IOObjectRelease(iter);
    
    
    return 0;
}
