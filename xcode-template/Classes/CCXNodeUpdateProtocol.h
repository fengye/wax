//
//  CCXLayerUpdateProtocol.h
//  RapidWax
//
//  Created by Ye Feng on 10-9-18.
//

#import <UIKit/UIKit.h>
#import "cocos2d.h"

@protocol CCXNodeUpdateProtocol
@optional
-(void) update:(ccTime)time;

@end
