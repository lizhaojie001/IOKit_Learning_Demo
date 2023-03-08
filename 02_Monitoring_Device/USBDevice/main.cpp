//
//  main.cpp
//  USBDevice
//
//  Created by macbook pro on 2023/3/8.
//

#include <iostream>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

void DeviceAdded(void * refCon,io_iterator_t iterator);

int main(int argc, const char * argv[]) {
    CFDictionaryRef matchingDict = NULL;
    io_iterator_t iter = 0;
    IONotificationPortRef notificationPort = NULL;
    CFRunLoopSourceRef runloopSource = NULL;
    io_service_t service = 0;
    kern_return_t kr;
    
    //创建一个匹配字典 查找任意设备
    
    matchingDict = IOServiceMatching("IOUSBHostDevice");
    
    //设置通信通道,I/OKit 通过该通道向用户空间应用程序传递通知消息
    notificationPort = IONotificationPortCreate(kIOMainPortDefault);
   
    runloopSource = IONotificationPortGetRunLoopSource(notificationPort);
    
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runloopSource, kCFRunLoopDefaultMode);
    
    kr = IOServiceAddMatchingNotification(notificationPort, kIOFirstMatchNotification, matchingDict, DeviceAdded, NULL, &iter);
    
    DeviceAdded(NULL,iter);
    
    CFRunLoopRun();
    
    IONotificationPortDestroy(notificationPort);
    //释放迭代器
    IOObjectRelease(iter);
    
    
    return 0;
}


void DeviceAdded(void * refCon,io_iterator_t iterator){
    io_service_t service = 0;
    //迭代所有匹配对象
    while ((service = IOIteratorNext(iterator)) != 0) {
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
    
}
