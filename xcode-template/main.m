//
//  main.m
//  ___PROJECTNAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//  Copyright ___ORGANIZATIONNAME___ ___YEAR___. All rights reserved.
//
// This where the magic happens!
// Wax doesn't use nibs to load the main view, everything is done within the
// AppDelegate.lua file

#import <UIKit/UIKit.h>

#import "ProtocolLoader.h"

#import "wax.h"
#import "wax_http.h"
#import "wax_json.h"
#import "wax_xml.h"

#import "lfs.h"
#import "luasocket.h"
#import "mime.h"
#import "luagl.h"
#import "cocos2d.h"

int main(int argc, char *argv[]) {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
	// make Obj-C runtime know CCDirector class before Wax initialized
	[CCDirector class];
    wax_startWithExtensions(luaopen_lfs, luaopen_socket_core, luaopen_mime_core, luaopen_luagl, 
							luaopen_wax_http, luaopen_wax_json, luaopen_wax_xml, nil);
    
    int retVal = UIApplicationMain(argc, argv, nil, @"AppDelegate");
    [pool release];
    return retVal;
}
