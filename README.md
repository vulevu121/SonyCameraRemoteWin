# SonyCameraRemoteWin

This is a command-line interface for controlling new Sony cameras such as Sony Alpha 7C. Based on Sony Camera Remote SDK v1.05.

To make it work, you need to follow libusb and environment setup instructions in RemoteSampleApp_IM_v1.05.00.pdf

Command Line Interface:

RemoteCli.exe --help

SYNOPSIS

        RemoteCli.exe capture [--dir <output dir>] [--verbose]        
        RemoteCli.exe get --prop <prop> [--verbose]        
        RemoteCli.exe set --prop <prop> --value <value> [--verbose]        
        RemoteCli.exe sdk [--verbose]        
        RemoteCli.exe --help [--verbose]        

OPTIONS

        capture     Capture an image        
        --dir       Output dir        
        get         Gets the value of a camera property        
        set         Sets the value of camera property        
        --prop      Property name        
        --value     Property value        
        sdk         Load the sample app from Sony Camera SDK        
        --help      This printed message        
        --verbose   Prints debugging messages
        
Please see source code for property names for get and set commands.
