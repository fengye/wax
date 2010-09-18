Wax
===

Wax is a framework that lets you write iPhone apps in [Lua](http://www.lua.org/about.html). It bridges Objective-C and Lua using the Objective-C runtime. With Wax, anything you can do in Objective-C is **automatically** available in Lua! What are you waiting for, give it a shot!

RapidWax
===

RapidWax is made possible by Wax and some other Lua open source projects, namely LuaSocket, LuaFilesystem, RemDebug, LuaGL and cocos2d-iphone. They're integrated to make RapidWax more suitable for rapid game prototyping.

Setup
-----

1. Clone wax [http://github.com/probablycorey/wax/](http://github.com/probablycorey/wax/)

2. From the command line, **cd** into wax folder created above and type **rake install**. This will install an xcode project template.

3. Open up xcode and create a new **Wax** project, it should be located under the **User Tempates** section.

4. Now you have an Wax(actually RapidWax) project, but still the referenced cocos2d project is unavailable. You need to close the project first and copy the entire /external folder to your new project folder. So your cocos2d in the project folder should looks like: MyProject/external/cocos2d-iphone/.

5. Reopen your project, Build and Run! You've got lua running on the iPhone!

6. Start editing **wax/data/scripts/MyLayer.lua** to make your app!

Why?
----

I love writing iPhone apps, but would rather write them in a dynamic language than in Objective-C. Here are some reasons why many people prefer Lua + Wax over Objective-C...

* Automatic Garbage Collection! Gone are the days of release and autorelease.

* Less Code! No more header files, no more static types, array and dictionary literals! Lua enables you to get more power out of less lines of code.

* Access to every Cocoa, UITouch, Foundation, etc.. framework, if it's written in Objective-C, Wax exposes it to Lua automatically. All the frameworks you love are all available to you!

* Super easy HTTP requests. Interacting with a REST webservice has never been eaiser

* Lua has closures, also known as blocks! Anyone who has used these before knows how powerful they can be.

* Lua has a build in Regex-like pattern matching library.


You get all of this with Lua, with no downside!

Examples
--------

For some simple Wax apps, check out the [examples folder](http://github.com/probablycorey/wax/tree/master/examples/).

How would I create a UIView and color it red?

    -- forget about using alloc! Memory is automatically managed by Wax
    view = UIView:initWithFrame(CGRect(0, 0, 320, 100))

    -- use a colon when sending a message to an Objective-C Object
    -- all methods available to a UIView object can be accessed this way
    view:setBackgroundColor(UIColor:redColor())

What about methods with multiple arguments?

    -- Just add underscores to the method name, then write the arguments like
    -- you would in a regular C function
    UIApplication:sharedApplication():setStatusBarHidden_animated(true, false)

How do I send an array/string/dictionary

    -- Wax automatically converts array/string/dictionary objects to NSArray,
    -- NSString and NSDictionary objects (and vice-versa)
    images = {"myFace.png", "yourFace.png", "theirFace.png"}
    imageView = UIImageView:initWithFrame(CGRect(0, 0, 320, 460))
    imageView:setAnimationImages(images)

What if I want to create a custom UIViewController?

    -- Created in "MyController.lua"
    --
    -- Creates an Objective-C class called MyController with UIViewController
    -- as the parent. This is a real Objective-C object, you could even
    -- reference it from Objective-C code if you wanted to.
    waxClass{"MyController", UIViewController}

    function init()
      -- to call a method on super, simply use self.super
      self.super:initWithNibName_bundle("MyControllerView.xib", nil)
      return self
    end

    function viewDidLoad()
      -- Do all your other stuff here
    end

You said HTTP calls were easy, I don't believe you...

    url = "http://search.twitter.com/trends/current.json"

    -- Makes an asyncronous call, the callback function is called when a
    -- response is received
    wax.http.request{url, callback = function(body, response)
      -- request is just a NSHTTPURLResponse
      puts(response:statusCode())

      -- Since the content-type is json, Wax automatically parses it and places
      -- it into a Lua table
      puts(body)
    end}

Since Wax converts NSString, NSArray, NSDictionary and NSNumber to native Lua values, you have to force objects back to Objective-C sometimes. Here is an example.

    local testString = "Hello lua!"
    local bigFont = UIFont:boldSystemFontOfSize(30)
    local size = toobjc(testString):sizeWithFont(bigFont)
    puts(size)

Debugging
---------

RapidWax integrated [RemDebug](http://www.keplerproject.org/remdebug/) to enable debugging Lua script in command line.

By default the RemDebug server is already run on your app if you create your app from Wax template. So the remaining part is just to start the command line debugger:

$ cd MyProject/wax/lib/extensions/remdebug

$ lua controller.lua


And the debugger will block. As the screen indicates, run your app, and the debugger will break on the first line of your code. You can either set breakpoints by 'set' or just type 'run' to let it go. More information please visit the [RemDebug example](http://www.keplerproject.org/remdebug/example.html).

Tutorials
---------

[Setting up iPhone wax](http://probablyinteractive.com/2009/10/18/Setting-up-iPhone-Wax.html)

[How does iPhone Wax work?](http://probablyinteractive.com/2009/10/19/How-does-iPhone-Wax-work.html)

[Twitter client in Wax](http://probablyinteractive.com/2009/10/20/Wax-talks-to-twitter.html)

Fast prototyping development
----------------------------
If you want to get rid of XCode or even Mac OSX for 99% of time, and don't want to even build your app when you tweaking your Lua code, you can go on reading this section.

The basic idea for fast prototyping is to load Lua modules from an dedicated FTP server, via WiFi or other kind of network your device/simulator supports. So this assume you have an LAN network setup(if you don't, which is rare, you can just test on localhost).

Here's how you actually do this(again, assume you create the project from Wax application template):

1. Open an FTP server on your development machine. Details for Mac user is [here](http://www.macinstruct.com/node/152).

2. Put your Lua code inside the FTP directory you can reach. Please make sure they still have /data/scripts/ in the directory hierarchy.

3. On AppDelegate.lua, line 8, you should see something like '--require "customftploader"'. Remove the comment. 

4. You should also modify the customftploader.lua, line 8, to provide your FTP url and user/password(this time the url should omit /data/scripts). What it does is search the FTP server for specific Lua module.

5. Run your application. If you get crashed, the output message should give  out some clue. Most of the time it's the directory hierarchy on FTP server not configured correctly.

6. Close your app and tweak your code on the FTP server. Then re-run your app to see the effect! Your app should reflect what you changed just now.


Which API's are included?
-------------------------

They all are! I can't stress this enough, anything that is written in Objective-C (even external frameworks) will work automatically in Wax! UIAcceleration works like UIAcceleration, MapKit works like MapKit, GameKit works like GameKit, Snozberries taste like Snozberries!

Created By
----------
Corey Johnson (probablycorey at gmail dot com)

RapidWax integration by Ye Feng.(eygneph at gmail dot com)

Acknowledgements
----------------

[Aman Gupta](http://github.com/tmm1): For adding YAJL JSON support.

[Apple](http://www.apple.com/iphone/): For creating such an awesome development platform.

More
----
* [Feature Requests? Bugs?](http://github.com/probablycorey/wax/issues) - Issue tracking and release planning.
* [Mailing List](http://groups.google.com/group/iphonewax)
* [IRC: #wax](irc://chat.freenode.net/#wax) on http://freenode.net
* Quick question or problem? IM **probablyCorey** on AIM

Contribute
----------
Fork it, change it, commit it, push it, send pull request; instant glory!


The MIT License
---------------
Wax is Copyright (C) 2009 Corey Johnson See the file LICENSE for information of licensing and distribution.

RapidWax is forked from Wax.
RapidWax is Copyright (C) 2010 Ye Feng.
See the file LICENSE for information of licensing and distribution.
