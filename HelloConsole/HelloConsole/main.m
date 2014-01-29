//
//  main.m
//  HelloConsole
//
//  Created by SuperS on 15/01/14.
//  Copyright (c) 2014 SuperS. All rights reserved.
//


#import <Cocoa/Cocoa.h>

@interface MyClass:NSObject
{
    
}
-sayHello;
@end

@implementation MyClass
- sayHello
{
 	NSLog(@"Hello World!");
    
 	return 0;
}
@end




int main(int argc, char *argv[])
{
 	id pippo;
    
 	pippo=[[MyClass alloc] init];
 	[pippo sayHello];
 	
 	return 0;
}

