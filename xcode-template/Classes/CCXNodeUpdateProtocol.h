//
//  CCXLayerUpdateProtocol.h
//  TestRapdWax
//
//  Created by Feng Ye on 10-9-18.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "cocos2d.h"

@protocol CCXNodeUpdateProtocol
@optional
-(void) update:(ccTime)time;

@end
